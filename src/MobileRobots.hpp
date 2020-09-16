#ifndef __MOBILE_ROBOTS_HPP_INCLUDED__
#define __MOBILE_ROBOTS_HPP_INCLUDED__

#include <QtWidgets/QMainWindow>

#include <chrono>
using namespace std::chrono_literals;

#include "ui_MobileRobots.h"

class QTimer;

namespace MobileRobots
{
    class Coord;
    class EnvironmentDescriptor;
    class AI;
    class Graphics;

    class MobileRobots : public QMainWindow
    {
        Q_OBJECT

    private:
        inline static constexpr auto INFO_WIDTH{ 200U }; //-V112
        inline static constexpr auto TIMER_INTERVAL_DEFAULT{ 150ms }; //-V112
        inline static constexpr auto TIMER_INTERVAL_STEP{ 25ms }; //-V112

    private:
        void initWidgets();

        void updateInfo(const Coord& coord);

        void resizeEvent(QResizeEvent* event) override;

        void mousePressEvent(QMouseEvent* event) override;

        void keyPressEvent(QKeyEvent* event) override;

    public:
        MobileRobots(QWidget *parent = nullptr);

        ~MobileRobots() noexcept;

        void update();

    private:
        std::unique_ptr<Ui::MobileRobotsClass> m_ui;
        std::unique_ptr<QTimer>                m_timer;
        std::shared_ptr<EnvironmentDescriptor> m_envDescr;
        std::shared_ptr<AI>                    m_ai;
        std::unique_ptr<Graphics>              m_graphics;
    };
} // namespace MobileRobots

#endif /* !__MOBILE_ROBOTS_HPP_INCLUDED__ */