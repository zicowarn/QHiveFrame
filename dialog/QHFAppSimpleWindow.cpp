#include "QHFAppSimpleWindow.h"

#include "QHFGuiNotifierCenter.h"
#include "QHFStateManager.h"
#include "QHFThemeManager.h"

#include <QDebug>
#include <QLabel>
#include <QMainWindow>

namespace QHF {

    constexpr QSize defaultSize(800, 600);

    ///--- QHFAppSimpleWindow ---///
    QHFAppSimpleWindow::QHFAppSimpleWindow(QHFWorkspaceType workspaceType, QWidget* parent)
        : QMainWindow(parent), m_workspaceType(workspaceType) {
        // Initializesstate
        QHFStateManager::instance().addWindow(this);
        // setname
        setObjectName("QHFAppSimpleWindow");
        // Set theme
        initTheme(QHFThemeManager::instance(), &QHFThemeManager::themeChanged);
        // Initializes
        initialize();
    }

    void QHFAppSimpleWindow::initialize() {
        // 尺寸策略
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        // Layout
        createWorkspace();
        // Set window size
        setMinimumSize(defaultSize);
        // window居中
        resize(defaultSize);
        // updatetitle
        updateTitle();
        // 监听event
        connect(&QHF::QHFNotifierCenter::instance(), &QHF::QHFNotifierCenter::notify, this,
                &QHFAppSimpleWindow::handleNotify);
    }

    void QHFAppSimpleWindow::updateTitle() {
        // 应用name
        QString appName = tr("My App");
        // settitle
        setWindowTitle(appName);
    }

    void QHFAppSimpleWindow::applyTheme() {}

    void QHFAppSimpleWindow::handleNotify(const QHFNotifier& notifier) {}

    void QHFAppSimpleWindow::createWorkspace() {
        if (m_workspaceType == QHFWorkspaceType::NONE) {
            qDebug() << "No workspace type specified!";
            return;
        }
        // 测试
        auto* label = new QLabel("Simple Window", this);
        setCentralWidget(label);
    }

    void QHFAppSimpleWindow::changeEvent(QEvent* event) {
        if (event->type() == QEvent::WindowStateChange) {
            WindowState state = QHFStateManager::instance().getWindowState(this);
            state.isFullScreen = isMaximized() || isFullScreen();
            QHFStateManager::instance().setWindowState(this, state);
        } else if (event->type() == QEvent::ActivationChange) {
            if (isActiveWindow()) {
                auto* activeWin = QHFStateManager::instance().activeWindow();
                if (activeWin != this || activeWin == nullptr) {
                    QHFStateManager::instance().setActiveWindow(this);
                    qDebug() << "Window activated:" << this;
                }
            } else {
                // window失活
                QHFStateManager::instance().setActiveWindow(nullptr);
                qDebug() << "Window deactivated:" << this;
            }
        }
        QMainWindow::changeEvent(event);
    }

    void QHFAppSimpleWindow::closeEvent(QCloseEvent* event) {
        // 移除Window state
        QHFStateManager::instance().removeWindow(this);
        // closewindow
        QMainWindow::closeEvent(event);
    }

    void QHFAppSimpleWindow::moveEvent(QMoveEvent* event) {
        qDebug() << "QHFAppSimpleWindow moved to:" << this->pos();
        if (isActiveWindow()) {
            QHFNotifier noti(QHFNotifierType::APP_WINDOW_MOVED);
            noti.set("window", this);
            publishEvent(this, noti, 150);
        }
        QMainWindow::moveEvent(event);
    }

} // namespace QHF
