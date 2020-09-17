// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Graphics.hpp"
#include "EnvironmentDescriptor.hpp"
#include "Sensor.hpp"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDir>

namespace MobileRobots
{
	void Graphics::loadImages()
	{
        QString path = ":/assets/images/scaled/";
        for (const auto& file : QDir(path).entryList())
            m_images.emplace(file.section(".", 0, 0).toStdString(), QPixmap(path + file));
	}

    void Graphics::unloadUnnessesaryImages()
    {
        for (auto it = std::begin(m_images); it != std::end(m_images); )
            if (!QString(it->first.c_str()).startsWith("light"))
                m_images.erase(it++);
            else
                it++;
    }

	void Graphics::createGrid(const uint32_t width, const uint32_t height)
	{
		for (uint32_t x{}; x <= width; ++x)
			m_grid[0].emplace_back(m_scene->addLine(x * m_xScale, 0., x * m_xScale, height * m_yScale, QPen(Qt::black)));

		for (uint32_t y{}; y <= height; ++y)
			m_grid[1].emplace_back(m_scene->addLine(0., y * m_yScale, width * m_xScale, y * m_yScale, QPen(Qt::black)));
	}

	void Graphics::createModules(std::shared_ptr<ObservationCenter> object)
	{
        static auto createEllipse = [&](const Coord& coord, const unsigned r, Qt::GlobalColor color, Qt::PenStyle penStyle, Qt::BrushStyle brushStyle)
        {
            QPen pen(color);
            pen.setStyle(penStyle);

            auto item = m_scene->addEllipse(
                0.,
                0.,
                r * 2. * m_xScale,
                r * 2. * m_yScale,
                pen,
                QBrush(color, brushStyle));

            item->setPos((coord.x + .5 - r) * m_xScale, (coord.y + .5 - r) * m_yScale);

            return item;
        };

        for (const auto& module : object->getModules())
            if (const auto& id = typeid(*module); module->isActive() && (id == typeid(Sensor) || id == typeid(ManagerModule)))
            {
                auto energyConsumer = std::dynamic_pointer_cast<EnergyConsumer>(module);

                bool isSensor = id == typeid(Sensor);
                auto item = isSensor ?
                    createEllipse(object->getPos(), energyConsumer->getRadius(), Qt::blue, Qt::PenStyle::DashLine, Qt::BrushStyle::BDiagPattern) :
                    createEllipse(object->getPos(), energyConsumer->getRadius(), Qt::red, Qt::PenStyle::DotLine, Qt::BrushStyle::Dense7Pattern);

                if (isSensor && (typeid(*object) == typeid(ObservationCenter) || typeid(*object) == typeid(CommandCenter)))
                {
                    auto sensor = std::dynamic_pointer_cast<Sensor>(energyConsumer);
                    item->setStartAngle(sensor->getDirection() * 90 * 16);
                    item->setSpanAngle(sensor->getAngle() * 16);
                }

                auto& [it, _] = m_modules.try_emplace(object);
                it->second.emplace_back(item, energyConsumer->getRadius());
            }
	}

    void Graphics::createObjects(const std::vector<std::shared_ptr<MapObject>>& objects)
    {
        for (const auto& object : objects)
        {
            std::string name;
            const auto& id = typeid(*object);
            if (id == typeid(Barrier))                name = "dark_barrier";
            else if (id == typeid(InterestingObject)) name = "dark_interesting_object";
            else if (id == typeid(RobotScout))        name = "robot_scout";
            else if (id == typeid(RobotCommander))    name = "robot_commander";
            else if (id == typeid(ObservationCenter)) name = "observation_center";
            else if (id == typeid(CommandCenter))     name = "command_center";

            if (id != typeid(RobotScout) && id != typeid(RobotCommander))
                m_map.at(object->getPos())->setPixmap(m_images.at(name));
            else
            {
                auto& [scout, pixmap] = *m_scouts.emplace(std::dynamic_pointer_cast<RobotScout>(object), m_scene->addPixmap(m_images.at(name))).first;
                pixmap->setPos(scout->getX() * m_xScale, scout->getY() * m_yScale);
            }

            if (id != typeid(Barrier) && id != typeid(InterestingObject))
                createModules(std::dynamic_pointer_cast<ObservationCenter>(object));
        }
    }

    void Graphics::setCurrentTilePos(const qreal x, const qreal y) noexcept
    {
        if (m_currentTile)
            m_currentTile->setPos(x * m_xScale, y * m_yScale);
    }

    void Graphics::createMap(const uint32_t width, const uint32_t height, const std::vector<std::shared_ptr<MapObject>>& objects)
    {
        loadImages();

        for (uint32_t x{}; x < width; ++x)
            for (uint32_t y{}; y < height; ++y)
            {
                Coord coord(x, y);
                m_map.emplace(coord, m_scene->addPixmap(m_images.at("dark_grass")));
                m_map.at(coord)->setPos(x * m_xScale, y * m_yScale);
            }

        createObjects(objects);

        createGrid(width, height);

        m_currentTile = m_scene->addRect(0., 0., m_xScale, m_yScale, QPen(Qt::cyan));

        unloadUnnessesaryImages();
    }

    void Graphics::draw(std::shared_ptr<EnvironmentDescriptor> envDescr, std::set<Coord> mapUpdates /* = { } */) const //-V813
    {
        for (auto& coord : mapUpdates)
            if (auto& object = envDescr->getObject(coord); !object || typeid(*object) == typeid(RobotScout) || typeid(*object) == typeid(RobotCommander))
                m_map.at(coord)->setPixmap(m_images.at("light_grass").scaled(m_xScale, m_yScale));
            else if (typeid(*object) == typeid(Barrier))
                m_map.at(coord)->setPixmap(m_images.at("light_barrier").scaled(m_xScale, m_yScale));
            else if (typeid(*object) == typeid(InterestingObject))
                m_map.at(coord)->setPixmap(m_images.at("light_interesting_object").scaled(m_xScale, m_yScale));

        for (const auto& [scout, pixmap] : m_scouts)
        {
            pixmap->setPos(scout->getX() * m_xScale, scout->getY() * m_yScale);

            for (const auto& [item, r] : m_modules.at(std::static_pointer_cast<ObservationCenter>(scout)))
                item->setPos((scout->getX() + .5 - r) * m_xScale, (scout->getY() + .5 - r) * m_yScale);
        }
    }

    void Graphics::resize(const qreal width, const qreal height, const qreal mapWidth, const qreal mapHeight)
    {
        m_scene->setSceneRect(0., 0., width, height);

        m_xScale = width / mapWidth;
        m_yScale = height / mapHeight ;

        for (auto& [coord, item] : m_map)
        {
            item->setPixmap(item->pixmap().scaled(m_xScale, m_yScale));
            item->setPos(coord.x * m_xScale, coord.y * m_yScale);
        }

        for (size_t x{}; x < m_grid[0].size(); ++x)
            m_grid[0][x]->setLine(static_cast<qreal>(x) * m_xScale, 0., static_cast<qreal>(x) * m_xScale, height);

        for (size_t y{}; y < m_grid[1].size(); ++y)
            m_grid[1][y]->setLine(0., static_cast<qreal>(y) * m_yScale, width, static_cast<qreal>(y) * m_yScale);

        for (auto& [scout, pixmap] : m_scouts)
        {
            pixmap->setPixmap(pixmap->pixmap().scaled(m_xScale, m_yScale));
            pixmap->setPos(scout->getX() * m_xScale, scout->getY() * m_yScale);
        }

        for (auto& [owner, modules] : m_modules)
            for (auto& [item, r] : modules)
                item->setRect(owner->getX(), owner->getY(), r * 2. * m_xScale, r * 2. * m_yScale);

    }
    
    void Graphics::clear() noexcept
    {
        m_xScale = m_yScale = Graphics::IMAGE_SIZE;

        m_scene->clear();
        m_map.swap(decltype(m_map)());
        m_scouts.swap(decltype(m_scouts)());
        m_modules.swap(decltype(m_modules)());
        m_images.swap(decltype(m_images)());
        m_grid[0].swap(std::vector<QGraphicsLineItem*>());
        m_grid[1].swap(std::vector<QGraphicsLineItem*>());

        m_currentTile = nullptr;
    }
} // namespace MobileRobots