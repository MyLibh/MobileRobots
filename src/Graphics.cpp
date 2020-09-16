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

	void Graphics::createGrid(const uint32_t width, const uint32_t height)
	{
		for (uint32_t x{}; x <= width; ++x)
			m_grid[0].emplace_back(m_scene->addLine(x * m_xScale, 0., x * m_xScale, height * m_yScale, QPen(Qt::black)));

		for (uint32_t y{}; y <= height; ++y)
			m_grid[1].emplace_back(m_scene->addLine(0., y * m_yScale, width * m_xScale, y * m_yScale, QPen(Qt::black)));
	}

	void Graphics::createModules(const std::shared_ptr<ObservationCenter>& object)
	{
        static auto createEllipse = [&](const Coord& coord, const unsigned r, Qt::GlobalColor color, Qt::PenStyle penStyle, Qt::BrushStyle brushStyle)
        {
            auto item = new QGraphicsEllipseItem(
                coord.x,
                coord.y,
                r * 2. * m_xScale,
                r * 2. * m_yScale);

            QPen pen(color);
            pen.setStyle(penStyle);
            item->setPen(pen);
            item->setBrush(QBrush(color, brushStyle));

            return item;
        };

        for (const auto& module : object->getModules())
        {
            if (typeid(*module) == typeid(Sensor))
            {
                const auto& sensor = std::dynamic_pointer_cast<Sensor>(module);
                const auto& r = sensor->getRadius();

                auto item = createEllipse(object->getPos(), r, Qt::blue, Qt::PenStyle::DashLine, Qt::BrushStyle::BDiagPattern);
                if (typeid(*object) == typeid(ObservationCenter) || typeid(*object) == typeid(CommandCenter))
                {
                    item->setStartAngle(sensor->getDirection() * 90 * 16);
                    item->setSpanAngle(sensor->getAngle() * 16);
                }

                m_scene->addItem(item);
                item->setPos((object->getX() + .5 - r) * m_xScale, (object->getY() + .5 - r) * m_yScale);

                auto& [it, _] = m_modules.try_emplace(object);
                it->second.emplace_back(item, r);
            }
            else if (typeid(*module) == typeid(ManagerModule))
            {
                const auto& manager = std::dynamic_pointer_cast<ManagerModule>(module);
                const auto& r = manager->getRadius();

                auto item = createEllipse(object->getPos(), r, Qt::red, Qt::PenStyle::DotLine, Qt::BrushStyle::Dense7Pattern);

                m_scene->addItem(item);
                item->setPos((object->getX() + .5 - r) * m_xScale, (object->getY() + .5 - r) * m_yScale);

                auto& [it, _] = m_modules.try_emplace(object);
                it->second.emplace_back(item, r);
            }
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
    }

	Graphics::Graphics() :
        m_xScale(Graphics::IMAGE_SIZE),
        m_yScale(Graphics::IMAGE_SIZE),
		m_scene(std::make_unique<QGraphicsScene>()),
		m_map(),
		m_scouts(),
		m_images(),
		m_modules(),
		m_grid(),
        m_currentTile{}
	{ }

    void Graphics::draw(std::shared_ptr<EnvironmentDescriptor> envDescr, std::set<Coord> mapUpdates /* = { } */) const
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

    void Graphics::resize(const qreal width, const qreal height)
    {
        m_scene->setSceneRect(0., 0., width, height);

        m_xScale = width / 10.;
        m_yScale = height / 10.;

        for (auto& [coord, item] : m_map)
        {
            item->setPixmap(item->pixmap().scaled(m_xScale, m_yScale));
            item->setPos(coord.x * m_xScale, coord.y * m_yScale);
        }

        for (size_t i{}; i < m_grid[0].size(); ++i)
            m_grid[0][i]->setLine(i * m_xScale, 0., i * m_xScale, height);

        for (size_t i{}; i < m_grid[1].size(); ++i)
            m_grid[1][i]->setLine(0., i * m_yScale, width, i * m_yScale);

        for (auto& [scout, pixmap] : m_scouts)
        {
            pixmap->setPixmap(pixmap->pixmap().scaled(m_xScale, m_yScale));
            pixmap->setPos(scout->getX() * m_xScale, scout->getY() * m_yScale);
        }

        for (auto& [owner, modules] : m_modules)
            for (auto& [item, r] : modules)
                item->setRect(owner->getX(), owner->getY(), r * 2. * m_xScale, r * 2. * m_yScale);

    }
} // namespace MobileRobots