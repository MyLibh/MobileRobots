// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "MobileRobots.hpp"
#include "EnvironmentDescriptor.hpp"
#include "MapLoader.hpp"
#include "AI.hpp"
#include "Sensor.hpp"
#include "PowerGenerator.hpp"

#include <QMouseEvent>
#include <QScrollBar>
#include <QScreen>

inline constexpr auto CONFIG_NAME{ ":/cfg/map.json" };

namespace MobileRobots
{
    void MobileRobots::initWidgets()
    {
        m_ui->setupUi(this);

        const int SCALED_CANVAS_WIDTH = m_envDescr->getWidth() * m_graphics.getXScale();
        const int SCALED_CANVAS_HEIGHT = m_envDescr->getHeight() * m_graphics.getYScale();

        auto canvas = m_ui->canvas;
        canvas->resize(SCALED_CANVAS_WIDTH, SCALED_CANVAS_HEIGHT);
        canvas->horizontalScrollBar()->blockSignals(true);
        canvas->verticalScrollBar()->blockSignals(true);
        
        auto& scene = m_graphics.getScene();
        scene->setParent(canvas);
        canvas->setScene(scene.get());
        scene->setSceneRect(0, 0, canvas->width(), canvas->height());

        m_ui->infoWidget->resize(MobileRobots::INFO_WIDTH, SCALED_CANVAS_HEIGHT);
        m_ui->infoWidget->move(SCALED_CANVAS_WIDTH, 0);

        // const auto size = qApp->screens()[0]->size().height();
        QMainWindow::setWindowIcon(QIcon(":/assets/icon.ico"));      
        QMainWindow::setFixedSize(SCALED_CANVAS_WIDTH + MobileRobots::INFO_WIDTH, SCALED_CANVAS_HEIGHT);
        // QMainWindow::showFullScreen();
        QMainWindow::move(qApp->screens()[0]->size().width() / 4, 0);

        updateInfo({ 0, 0 });
    }

    void MobileRobots::updateInfo(const Coord& coord)
    {
        QString text;
        text.append(QString("<b>width:</b> %1<br/><b>height:</b> %2<br/><br/>").arg(m_envDescr->getWidth()).arg(m_envDescr->getHeight()));
    
        if (const auto object = m_envDescr->getObject(coord))
        {
            auto str = std::move(object->toString());
            text.append(str.c_str());
        }
        else
            text.append(QString("<b>x:</b> %1<br/><b>y:</b> %2<br/><b>type:</b> Grass<br/>").arg(coord.x).arg(coord.y));

        auto infoWidget = m_ui->infoWidget;
        infoWidget->setPlainText("");
        infoWidget->appendHtml(text);
        infoWidget->moveCursor(QTextCursor::Start);
    }

    void MobileRobots::resizeEvent(QResizeEvent* event)
    {
        auto canvas = m_ui->canvas;
        canvas->resize(QMainWindow::width() - MobileRobots::INFO_WIDTH, QMainWindow::height());

        m_ui->infoWidget->resize(MobileRobots::INFO_WIDTH, QMainWindow::height());
        m_ui->infoWidget->move(canvas->width(), 0);

        m_graphics.resize(canvas->width(), canvas->height());

        QMainWindow::resizeEvent(event);
    }

    void MobileRobots::mousePressEvent(QMouseEvent* event)
    {
        if (event->x() <= m_ui->canvas->width() && event->y() <= m_ui->canvas->height())
        {
            auto x{ static_cast<uint32_t>(event->x() / m_graphics.getXScale()) };
            auto y{ static_cast<uint32_t>(event->y() / m_graphics.getYScale()) };
            updateInfo({ x,  y});

            m_graphics.setCurrentTilePos(x, y);
        }
        
        QMainWindow::mousePressEvent(event);
    }

    void MobileRobots::keyPressEvent(QKeyEvent* event)
    {
        if (event->key() == Qt::Key_Escape)
            QMainWindow::close();

        QMainWindow::keyPressEvent(event);
    }

    MobileRobots::MobileRobots(QWidget* parent /* = nullptr */) :
        QMainWindow(parent),
        m_ui(std::make_unique<Ui::MobileRobotsClass>()),
        m_timer(std::make_unique<QTimer>(this)),
        m_envDescr(MapLoader::load(QString(CONFIG_NAME))),
        m_ai(std::make_shared<AI>(m_envDescr)),
        m_graphics()
    {
        initWidgets();
        m_graphics.createMap(m_envDescr->getWidth(), m_envDescr->getHeight(), m_envDescr->getObjects());

        connect(m_timer.get(), &QTimer::timeout, this, &MobileRobots::update);
        m_timer->start(50);

        ManagerModule::setAI(m_ai->shared_from_this());

        m_graphics.draw(m_envDescr);
    }

    void MobileRobots::update()
    {
        m_ai->work();

        m_graphics.draw(m_envDescr, std::move(m_ai->getMapUpdates()));

        if (m_ai->finished())
            ; // TODO: analyze results
    }
} // namespace MobileRobots