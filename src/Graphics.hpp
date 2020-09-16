#ifndef __GRAPHICS_HPP_INCLUDED__
#define __GRAPHICS_HPP_INCLUDED__

#include "Coord.hpp"
#include "RobotScout.hpp"

#include <QGraphicsScene>
#include <QGraphicsRectItem>

#include <set>

namespace MobileRobots
{
	class Graphics final
	{
	private:
		using pixmap_map_t = std::unordered_map<Coord, QGraphicsPixmapItem*>;
		using scouts_map_t = std::unordered_map<std::shared_ptr<RobotScout>, QGraphicsPixmapItem*>;
		using modules_map_t = std::unordered_map<std::shared_ptr<ObservationCenter>, std::vector<std::pair<QGraphicsEllipseItem*, unsigned>>>;

	private:
		inline static constexpr auto IMAGE_SIZE{ 64U }; //-V112

	private:
		void loadImages();

		void createGrid(const uint32_t width, const uint32_t height);

		void createModules(const std::shared_ptr<ObservationCenter>& object);

		void createObjects(const std::vector<std::shared_ptr<MapObject>>& objects);

	public:
		Graphics();

		inline ~Graphics() noexcept = default;

		[[nodiscard]]
		inline constexpr auto getXScale() const noexcept { return m_xScale; }

		[[nodiscard]]
		inline constexpr auto getYScale() const noexcept { return m_yScale; }

		[[nodiscard]]
		inline auto& getScene() const noexcept { return m_scene; }

		void setCurrentTilePos(const qreal x, const qreal y) noexcept { if (m_currentTile) m_currentTile->setPos(x * m_xScale, y * m_yScale); }

		void createMap(const uint32_t width, const uint32_t height, const std::vector<std::shared_ptr<MapObject>>& objects);

		void draw(std::shared_ptr<EnvironmentDescriptor> envDescr, std::set<Coord> mapUpdates = { }) const;

		void resize(const qreal width, const qreal height);

	private:
		qreal                           m_xScale;
		qreal                           m_yScale;
		std::unique_ptr<QGraphicsScene> m_scene;
		pixmap_map_t                    m_map;
		scouts_map_t                    m_scouts;
		modules_map_t                   m_modules;
		std::map<std::string, QPixmap>  m_images;
		std::vector<QGraphicsLineItem*> m_grid[2];
		QGraphicsRectItem*              m_currentTile;
	};
} // namespace MobileRobots

#endif /* !__GRAPHICS_HPP_INCLUDED__ */