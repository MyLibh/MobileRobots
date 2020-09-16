// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "MobileRobots.hpp"
#include "MapLoader.hpp"
#include "EnvironmentDescriptor.hpp"
#include "AI.hpp"
#include "Graphics.hpp"
#include "Utility.hpp"

#include <QTimer>
#include <QMouseEvent>
#include <QScrollBar>
#include <QScreen>
#include <QFileDialog>

inline constexpr auto CONFIG_NAME{ ":/cfg/map.json" };

namespace MobileRobots
{
    void MobileRobots::initWidgets()
    {
        m_ui->setupUi(this);

        auto canvas = m_ui->canvas;
        canvas->horizontalScrollBar()->blockSignals(true);
        canvas->verticalScrollBar()->blockSignals(true);
        
        auto& scene = m_graphics->getScene();
        scene->setParent(canvas);
        canvas->setScene(scene.get());

        QMainWindow::setWindowIcon(QIcon(":/assets/icon.ico"));
        QMainWindow::setFixedSize(m_envDescr->getWidth() * m_graphics->getXScale() + MobileRobots::INFO_WIDTH, m_envDescr->getHeight() * m_graphics->getYScale() + MobileRobots::MENU_HEIGHT);
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
        canvas->resize(QMainWindow::width() - MobileRobots::INFO_WIDTH, QMainWindow::height() - MobileRobots::MENU_HEIGHT);

        m_ui->infoWidget->resize(MobileRobots::INFO_WIDTH, QMainWindow::height() - MobileRobots::MENU_HEIGHT);
        m_ui->infoWidget->move(canvas->width(), 0);

        m_graphics->resize(canvas->width(), canvas->height());

        QMainWindow::resizeEvent(event);
    }

    void MobileRobots::mousePressEvent(QMouseEvent* event)
    {
        if (event->x() <= m_ui->canvas->width() && event->y() <= m_ui->canvas->height() + MobileRobots::MENU_HEIGHT)
        {
            auto x{ static_cast<uint32_t>(event->x() / m_graphics->getXScale()) };
            auto y{ static_cast<uint32_t>(event->y() / m_graphics->getYScale()) };
            updateInfo({ x,  y});

            m_graphics->setCurrentTilePos(x, y);
        }
        
        QMainWindow::mousePressEvent(event);
    }

    void MobileRobots::keyPressEvent(QKeyEvent* event)
    {
        if (event->key() == Qt::Key_Escape)
            QMainWindow::close();
        switch (event->key())
        {
        case Qt::Key_Escape: QMainWindow::close(); break;

        case Qt::Key_Equal: m_timer->setInterval(m_timer->intervalAsDuration() + MobileRobots::TIMER_INTERVAL_STEP); break;

        case Qt::Key_Minus: 
        {
            auto duration = m_timer->intervalAsDuration() - MobileRobots::TIMER_INTERVAL_STEP;
            m_timer->setInterval(duration <= 0ms ? MobileRobots::TIMER_INTERVAL_STEP : duration);
            
            break;
        }

        default: break;
        }

        QMainWindow::keyPressEvent(event);
    }

    MobileRobots::MobileRobots(QWidget* parent /* = nullptr */) :
        QMainWindow(parent),
        m_ui(std::make_unique<Ui::MobileRobotsClass>()),
        m_timer(std::make_unique<QTimer>(this)),
        m_envDescr(MapLoader::load(QString(CONFIG_NAME))),
        m_ai(std::make_shared<AI>(m_envDescr)),
        m_graphics(std::make_unique<Graphics>())
    {
        initWidgets();
        m_graphics->createMap(m_envDescr->getWidth(), m_envDescr->getHeight(), m_envDescr->getObjects());

        QObject::connect(m_timer.get(), &QTimer::timeout, this, &MobileRobots::update);
        m_timer->start(MobileRobots::TIMER_INTERVAL_DEFAULT);

        ManagerModule::setAI(m_ai->shared_from_this());

        QObject::connect(m_ui->actionOpen, &QAction::triggered,
            [&]()
            {
                auto interval = m_timer->interval();
                m_timer->stop();

                auto path = QFileDialog::getOpenFileName(this, "Open Config", "cfg/", "JSON(*.json)");
                if (!path.isEmpty())
                {
                    if (auto newEnvDescr = MapLoader::load(path); newEnvDescr)
                    {
                        m_ui->infoWidget->setPlainText("");

                        m_envDescr = std::move(newEnvDescr);

                        QMainWindow::setFixedSize(m_envDescr->getWidth() * m_graphics->getXScale() + MobileRobots::INFO_WIDTH, m_envDescr->getHeight() * m_graphics->getYScale() + MobileRobots::MENU_HEIGHT);

                        m_ai->clear();
                        m_graphics->clear();

                        auto canvas = m_ui->canvas;
                        m_graphics->getScene()->setSceneRect(0, 0, canvas->width(), canvas->height());
                        m_graphics->createMap(m_envDescr->getWidth(), m_envDescr->getHeight(), m_envDescr->getObjects());
                        m_ai->reset(m_envDescr);

                        updateInfo({ 0, 0 });
                    }
                    else
                        Utility::showError("Wrong config file");
                }

                m_timer->start();
            });

        m_graphics->draw(m_envDescr);
    }

    MobileRobots::~MobileRobots() noexcept = default;

    void MobileRobots::update()
    {
        m_ai->work();

        m_graphics->draw(m_envDescr, std::move(m_ai->getMapUpdates()));

        if (m_ai->finished())
            ; // TODO: analyze results
    }
} // namespace MobileRobots