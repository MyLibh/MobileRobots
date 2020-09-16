// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "ObservationCenter.hpp"
#include "PowerGenerator.hpp"
#include "Sensor.hpp"
#include "EnvironmentDescriptor.hpp"

#include <algorithm>

namespace detail
{
    inline bool isInSector(const MobileRobots::Coord& origin, const MobileRobots::Coord& coord, const unsigned radius, const unsigned startAngle, const unsigned angle) noexcept
    {
        double vecX = (int)coord.x - (int)origin.x;
        double vecY = (int)origin.y - (int)coord.y;

        auto ang = Math::rad2deg(std::acos((vecX * (double)radius) / (Math::hypot(vecX, vecY) * Math::hypot((double)radius, 0.l))));
        if (vecY < 0)
            ang = 360 - ang;

        if (ang == 0)
            return origin.distanceTo(coord) <= radius && (startAngle == ang || ang == startAngle + angle || ang == (startAngle + angle) % 360);

        return origin.distanceTo(coord) <= radius && startAngle <= ang && ang <= startAngle + angle;
    }
} // namespace detail

namespace MobileRobots
{
    void ObservationCenter::initModules()
    {
        unsigned powerSupplySum{};
        unsigned powerUsageSum{ m_powerUsage };

        for (const auto& module : m_modules)
            if (typeid(*module) == typeid(PowerGenerator))
                powerSupplySum += std::dynamic_pointer_cast<PowerGenerator>(module)->getPowerSupplyLevel();
            else
                powerUsageSum += std::dynamic_pointer_cast<EnergyConsumer>(module)->getPowerUsage();

        std::sort(std::begin(m_modules), std::begin(m_modules), [](const auto& lhs, const auto& rhs) { return lhs->getPriority() > rhs->getPriority(); });

        for (size_t i{}; i < m_modules.size() && powerUsageSum > powerSupplySum; ++i)
            if (typeid(*m_modules[i]) != typeid(PowerGenerator))
            {
                m_modules[i]->turnOff();

                powerUsageSum -= std::dynamic_pointer_cast<EnergyConsumer>(m_modules[i])->getPowerUsage();
            }

        // TODO: check bad conditions
    }

    std::string ObservationCenter::toStringHelper(std::string&& type) const
    {
        auto str = std::move(MapObject::toStringHelper(std::move(type)));
        str +=
            "<b>description:</b> " + m_descr +
            "<br/><b>powerUsage:</b> " + std::to_string(m_powerUsage) +
            "<br/><b>maxSlots:</b> " + std::to_string(m_maxSlots) +
            "<br/><b>price:</b> " + std::to_string(m_price) +
            "<br/><b>modules:</b><br/><br>";

        for (const auto& module : m_modules)
            str += module->toString() + "<br/>";

        return std::move(str);
    }

    std::map<Coord, std::shared_ptr<MapObject>> ObservationCenter::getObjectsAround() const
    {
        std::map<Coord, std::shared_ptr<MapObject>> objectsAround;
        for (const auto& module : m_modules)
            if (typeid(*module) == typeid(Sensor))
            {
                auto sensor{ std::dynamic_pointer_cast<Sensor>(module) };

                const auto r = sensor->getRadius();

                auto x = getX();
                const auto xMin = x < r ? 0U : x - r;
                const auto xMax = std::min(x + r, sEnvDesc->getWidth() - 1);

                auto y = getY();
                const auto yMin = y < r ? 0U : y - r;
                const auto yMax = std::min(y + r, sEnvDesc->getHeight() - 1);

                for (x = xMin; x <= xMax; ++x)
                    for (y = yMin; y <= yMax; ++y)
                        if (const Coord coord{ x, y }; coord != m_pos && detail::isInSector(m_pos, coord, r, sensor->getDirection() * 90, sensor->getAngle()))
                            objectsAround.emplace(coord, sEnvDesc->getObject(coord));
            }

        return std::move(objectsAround);
    }
}