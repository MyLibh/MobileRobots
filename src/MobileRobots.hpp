#ifndef __MOBILE_ROBOTS_HPP_INCLUDED__
#define __MOBILE_ROBOTS_HPP_INCLUDED__

#include <QtWidgets/QMainWindow>
#include <QTimer>

#include "ui_MobileRobots.h"

#include "Coord.hpp"
#include "RobotScout.hpp"

namespace MobileRobots
{
    class EnvironmentDescriptor;
    class AI;

    class MobileRobots : public QMainWindow
    {
        Q_OBJECT

    private:
        using pixmap_map_t = std::unordered_map<Coord, QGraphicsPixmapItem*>;
        using scouts_map_t = std::unordered_map<std::shared_ptr<RobotScout>, QGraphicsPixmapItem*>;
        using modules_map_t = std::unordered_map<std::shared_ptr<ObservationCenter>, std::vector<std::pair<QGraphicsEllipseItem*, unsigned>>>;

    private:
        inline static constexpr auto IMAGE_SIZE{ 64U }; //-V112
        inline static constexpr auto INFO_WIDTH{ 200U }; //-V112

    private:
        void initWidgets();

        void loadImages();

        void drawGrid(const uint32_t width, const uint32_t height);

        void drawModules();

        void initMap();

        void updateInfo(const Coord& coord);

        void resizeEvent(QResizeEvent* event) override;

        void mousePressEvent(QMouseEvent* event) override;

        void keyPressEvent(QKeyEvent* event) override;

    public:
        MobileRobots(QWidget *parent = nullptr);

        inline ~MobileRobots() noexcept = default;

        void draw() const;

        void update();

    private:
        std::unique_ptr<Ui::MobileRobotsClass> m_ui;
        std::shared_ptr<EnvironmentDescriptor> m_envDescr;
        std::shared_ptr<AI>                    m_ai;
        std::unique_ptr<QTimer>                m_timer;
        std::unique_ptr<QGraphicsScene>        m_scene;
        pixmap_map_t                           m_map;
        scouts_map_t                           m_scouts;
        modules_map_t                          m_modules;
        std::map<std::string, QPixmap>         m_images;
        Coord                                  m_scaleFactor;
        std::vector<QGraphicsLineItem*>        m_grid[2];
    };
} // namespace MobileRobots

#endif /* !__MOBILE_ROBOTS_HPP_INCLUDED__ */