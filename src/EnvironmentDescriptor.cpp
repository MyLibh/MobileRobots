// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "EnvironmentDescriptor.hpp"

namespace MobileRobots
{
    std::shared_ptr<MapObject> EnvironmentDescriptor::getObject(const Coord& coord) const noexcept
    {
        if (m_objectsCache.empty()) // load cache
            for (const auto& object : m_objects)
                m_objectsCache.emplace(object->getPos(), object);

        if (auto&& it = m_objectsCache.find(coord); it != std::end(m_objectsCache))
            return it->second;

        return nullptr;
    }
} // namespace MobileRobots
