#include "QHFAppMainWindow.h"

#include "QHFAboutUsDialog.h"
#include "QHFAppSimpleWindow.h"
#include "QHFContactUsDialog.h"
#include "QHFFileDialog.h"
#include "QHFGuiNotifierCenter.h"
#include "QHFMessageBox.h"
#include "QHFPreferenceDialog.h"
#include "QHFStateManager.h"
#include "QHFThemeManager.h"

#ifdef ENABLE_RUNTIME
#include "rna/BindingManager.h"
#include "rna/RuntimeUpdater.h"
#include "rna/UIUpdater.h"
#include "runtime/export/PdfExporter.h"
#include "runtime/global/GlobalRuntimeContext.h"
#include "runtime/render/RenderSystem.h"
#include "runtime/render/camera/trackballcameracontroller.h"
#include "runtime/render/glwidget/ModelViewWid.h"
#include "runtime/undo/LambdaCommand.h"
#endif

#include <QDebug>
#include <QDir>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>
#include <QWindow>
#include <filesystem>

namespace QHF {

    constexpr QSize defaultSize(1024, 800);
    constexpr int   subWindowOffset = 100;

    QHFAppMainWindow::QHFAppMainWindow(QWidget* parent, bool firstCreation)
        : QMainWindow(parent), m_isFirstCreation(firstCreation) {
        // Initializesstate
        QHFStateManager::instance().addWindow(this);
        // setname
        setObjectName("QHFAppMainWindow");
        // Set theme
        initTheme(QHFThemeManager::instance(), &QHFThemeManager::themeChanged);
        // Initializes
        initialize();
    }

    void QHFAppMainWindow::changeEvent(QEvent* event) {
        if (event->type() == QEvent::WindowStateChange) {
            WindowState state  = QHFStateManager::instance().getWindowState(this);
            state.isFullScreen = isMaximized() || isFullScreen();
            QHFStateManager::instance().setWindowState(this, state);
        } else if (event->type() == QEvent::ActivationChange) {
            if (isActiveWindow()) {
                // Returns current activatewindow
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

    void QHFAppMainWindow::closeEvent(QCloseEvent* event) {
        // 移除Window state
        QHFStateManager::instance().removeWindow(this);
        // closeall子window
        for (auto* child : findChildren<QWidget*>()) {
            if (child->isWindow()) {
                child->close();
            }
        }
        // closewindow
        QMainWindow::closeEvent(event);
    }

    void QHFAppMainWindow::moveEvent(QMoveEvent* event) {
        qDebug() << "MainWindow moved to:" << this->pos();
        if (isActiveWindow()) {
            QHFNotifier noti(QHFNotifierType::APP_WINDOW_MOVED);
            noti.set("window", this);
            publishEvent(this, noti, 150);
        }
        QMainWindow::moveEvent(event);
    }

    void QHFAppMainWindow::initialize() {
        // menu栏
        m_header = new QHFAppHeader(this);
        setMenuWidget(m_header);
        // status bar
        m_statusBar = new QHFAppStatusBar(this);
        setStatusBar(m_statusBar);

        // loadworkspace (主要部分)
        auto workspaceInfos = QHFWorkspaceRegistry::instance().workspaces();
        if (!workspaceInfos.empty()) {
            m_workspaceContainer = new QStackedWidget(this);
            for (const auto& wsinfo : workspaceInfos) {
                auto* workspace = QHFWorkspaceRegistry::instance().createWorkspace(wsinfo.type, this,
                                                                                 m_isFirstCreation);
                m_workspaces.append(workspace);
                m_workspaceContainer->addWidget(workspace);
            }
            int currentIndex = QHFStateManager::instance().getMainWinWorkspaceIndex(this);
            m_workspaceContainer->setCurrentIndex(currentIndex);
            setCentralWidget(m_workspaceContainer);
        } else { // 没有工作区
            qDebug() << "No workspace found!";
            auto* emptyWidget = new QLabel(this);
            emptyWidget->setText("No workspace found!");
            setCentralWidget(emptyWidget);
        }
        // Set window size
        setMinimumSize(defaultSize);
        // window居中
        resize(defaultSize);

        // determinestatusbar 是否需要显示
        if (!QHFStateManager::instance().getStatusBarVisible(this)) {
            m_statusBar->hide();
        }
        // updatetitle
        updateTitle();
        // 监听event
        connect(&QHF::QHFNotifierCenter::instance(), &QHF::QHFNotifierCenter::notify, this,
                &QHFAppMainWindow::handleNotify);
    }

    void QHFAppMainWindow::updateTitle() {
        // Returns current File state
        const auto& fileState = QHFStateManager::instance().getFileState();
        // updatetitle
        QString titleString = "";
        // determine是否有workspace
        if (fileState.isDirty) {
            titleString += " *";
        }
        // determine是否有file名
        if (fileState.name == "Untitled") {
            QString transStr = tr("Untitled");
            if (!transStr.isEmpty()) {
                titleString += transStr;
            } else {
                titleString += "Untitled";
            }
        } else {
            qDebug() << "File name: " << fileState.name;
            qDebug() << "File path: " << fileState.path;
            QString fileName = QString("%1[%2]").arg(fileState.name).arg(fileState.path);
            titleString += fileName;
        }
        // 加入应用name
        QString appName = tr("My App");
        titleString     = titleString + " - " + appName;
        // settitle
        setWindowTitle(titleString);
    }

    /// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    bool QHFAppMainWindow::newFile() {
        // Subclasses should implement new-file creation logic
        emit fileNewRequested();
        return true;
    }

    /// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    bool QHFAppMainWindow::openFile(FileInfo& openedFile) {
        QString caption = tr("Open File");
        auto    filter  = QString("%1 (*.qhm);;%2 (*.*)")
                          .arg(tr("QHiveFrame Document"))
                          .arg(tr("All Files"));
        QString selectedFilter;

        QString filePath = QHFFileDialog::getOpenFileName(this,
                                                        caption,
                                                        QDir::homePath(),
                                                        filter,
                                                        &selectedFilter,
                                                        QHFFDOption::ReadOnly);

        if (!filePath.isEmpty()) {
            openedFile.fileName = QFileInfo(filePath).fileName();
            openedFile.filePath = filePath;
            emit fileOpened(filePath);
            return true;
        }
        return false;
    }

    /// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    bool QHFAppMainWindow::saveFile(FileInfo& savedFile) {
        QString caption = tr("Save File");
        auto    filter  = QString("%1 (*.himb);;%2 (*.himt);;%3 (*.*)")
                          .arg(tr("QHiveFrame File"))
                          .arg(tr("QHiveFrame Theme File"))
                          .arg(tr("All Files"));
        QString selectedFilter; // Returns 用户最终选择 过滤器

        // 使用static方法open一个file对话框
        QString filePath = QHFFileDialog::getSaveFileName(this,             // 父窗口
                                                        caption,          // 对话框标题
                                                        QDir::homePath(), // 初始目录
                                                        filter,           // 文件过滤器
                                                        &selectedFilter, // Returns 用户选择 过滤器
                                                        QHFFDOption::ReadOnly // 额外选项: 只读
        );

        if (!filePath.isEmpty()) {
            qDebug() << "Selected file path: " << filePath;
        }
        return true;
    }

    /// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    bool QHFAppMainWindow::saveAsFile() {
        // TODO: implement
        return true;
    }

    /// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    bool QHFAppMainWindow::importFile() {
        // TODO: implement
        return true;
    }

    /// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    bool QHFAppMainWindow::exportFile() {
        // TODO: implement
        return true;
    }

    /// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    bool QHFAppMainWindow::closeFile() {
        // Subclasses should implement file close logic
        emit fileClosed();
        return true;
    }

    void QHFAppMainWindow::handleNotify(const QHFNotifier& notifier) {
        switch (notifier.type) {
        case QHFNotifierType::NEW_FILE:
            handleNewFile(notifier);
            break;
        case QHFNotifierType::OPEN_FILE:
            handleOpenFile(notifier);
            break;
        case QHFNotifierType::SAVE_FILE:
            handleSaveFile(notifier);
            break;
        case QHFNotifierType::SAVE_AS:
            handleSaveAsFile(notifier);
            break;
        case QHFNotifierType::CLOSE_FILE:
            handleCloseFile(notifier);
            break;
        case QHFNotifierType::FILE_STATE_MODIFIED:
        case QHFNotifierType::FILE_STATE_CLOSED:
        case QHFNotifierType::FILE_STATE_OPENED:
        case QHFNotifierType::FILE_STATE_SAVED:
        case QHFNotifierType::FILE_STATE_IMPORTED:
        case QHFNotifierType::FILE_STATE_EXPORTED:
            handleFileStateChanged(notifier);
            break;
        case QHFNotifierType::IMPORT_FILE_1:
            handleImportFile(notifier);
            break;
        case QHFNotifierType::EXPORT_FILE_1:
            handleExportFile(notifier);
            break;
        case QHFNotifierType::SET_WORKSPACE_INDXE:
            handleSetWorkspaceIndex(notifier);
            break;
        case QHFNotifierType::TOGGLE_FULL_SCREEN:
            handleToggleFullScreen(notifier);
            break;
        case QHFNotifierType::SYSTEM_EXIT:
            handleSystemExit(notifier);
            break;
        case QHFNotifierType::TOGGLE_STATUS_BAR:
            handleToggleStatusBar(notifier);
            break;
        case QHFNotifierType::NEW_SIMPLE_WINDOW:
            handleNewSimpleWindow(notifier);
            break;
        case QHFNotifierType::NEW_MAIN_WINDOW:
            handleNewMainWindow(notifier);
            break;
        case QHFNotifierType::REPORT_ACTIVATION:
            handleReportActivation(notifier);
            break;
        case QHFNotifierType::PREFERENCES_SETTINGS:
            handlePreferencesSettings(notifier);
            break;
        case QHFNotifierType::GET_HELP:
            handleGetHelp(notifier);
            break;
        case QHFNotifierType::ABOUT_US:
            handleAboutUs(notifier);
            break;
        default:
            break;
        }
    }

    void QHFAppMainWindow::handleNewFile(const QHFNotifier& notifier) {
        if (isActiveWindow()) {
            // Returns current File state
            const auto& fileState = QHFStateManager::instance().getFileState();
            // determine是否有file名
            if (fileState.name != "Untitled") {
                // 已经为新file, 不需要new
                return;
            }

            // determine是否有file名
            if (fileState.isDirty) {
                // Tip: 示是否save
                QHFMessageBox::QHFMsgBoxButton result = QHFMessageBox::question(
                    this, tr("Confirm"), tr("Do you want to save changes?"),
                    QHFMessageBox::YES | QHFMessageBox::NO | QHFMessageBox::CANCEL, QHFMessageBox::YES);
                if (result == QHFMessageBox::YES) {
                    qDebug() << "User clicked Yes";
                } else if (result == QHFMessageBox::NO) {
                    qDebug() << "User clicked No";
                } else {
                    qDebug() << "User clicked Cancel";
                }

                // 开始savefile
                FileInfo savedFile;
                if (saveFile(savedFile)) {
                    // updatefilelist
                    FileState newFileState = fileState;
                    // savesuccess, updateFile state
                    newFileState.name    = savedFile.fileName;
                    newFileState.path    = savedFile.filePath;
                    newFileState.isDirty = false;
                    // Update state
                    QHFStateManager::instance().setFileState(newFileState);
                    // updatefilelist
                    QHFStateManager::instance().updateRecentFiles();
                }
            }
            // closecurrentfile
            closeFile();
            // resetFile state
            QHFStateManager::instance().resetFileState();
            // open新file
            newFile();
            // Publishes notification
            QHFNotifier noti(QHFNotifierType::FILE_STATE_OPENED);
            noti.set("source", this);
            publishEvent(this, noti);
        }
    }

    void QHFAppMainWindow::handleOpenFile(const QHFNotifier& notifier) {
        if (isActiveWindow()) {
            // Returns current File state
            const auto& fileState = QHFStateManager::instance().getFileState();
            // determine是否有file名
            if (fileState.name == "Untitled" && fileState.isDirty) {
                // Tip: 示是否需要save
                // savefile
                FileInfo savedFile;
                if (saveFile(savedFile)) {
                    FileState newFileState = fileState;
                    // savesuccess, updateFile state
                    newFileState.name    = savedFile.fileName;
                    newFileState.path    = savedFile.filePath;
                    newFileState.isDirty = false;
                    // Update state
                    QHFStateManager::instance().setFileState(newFileState);
                    // updatefilelist
                    QHFStateManager::instance().updateRecentFiles();
                }
            }
            // closecurrentfile
            closeFile();
            // open新file
            FileInfo openedFile;
            if (openFile(openedFile)) {
                // opensuccess, updateFile state
                FileState newFileState = fileState;
                newFileState.name      = openedFile.fileName;
                newFileState.path      = openedFile.filePath;
                newFileState.isDirty   = false;
                // Update state
                QHFStateManager::instance().setFileState(newFileState);
                QHFStateManager::instance().updateRecentFiles();
                QHFStateManager::instance().saveState();
                // Publishes notification
                QHFNotifier noti(QHFNotifierType::FILE_STATE_OPENED);
                noti.set("source", this);
                publishEvent(this, noti);
            }
        }
    }

    void QHFAppMainWindow::handleSaveFile(const QHFNotifier& notifier) {
        if (isActiveWindow()) {
            // Returns current File state
            const auto& fileState = QHFStateManager::instance().getFileState();
            // determine是否有file名
            if (fileState.isDirty) {
                // savefile
                FileInfo savedFile;
                if (saveFile(savedFile)) {
                    FileState newFileState = fileState;
                    // savesuccess, updateFile state
                    newFileState.name    = savedFile.fileName;
                    newFileState.path    = savedFile.filePath;
                    newFileState.isDirty = false;
                    // Update state
                    QHFStateManager::instance().setFileState(newFileState);
                    // Publishes notification
                    QHFNotifier noti(QHFNotifierType::FILE_STATE_SAVED);
                    noti.set("source", this);
                    publishEvent(this, noti);
                }
            }
        }
    }

    void QHFAppMainWindow::handleSaveAsFile(const QHFNotifier& notifier) {
        if (isActiveWindow()) {
            // Returns current File state
            const auto& fileState = QHFStateManager::instance().getFileState();
            // savefile作为副本
            FileInfo savedFile;
            if (saveFile(savedFile)) {
                FileState newFileState = fileState;
                // save副本success, updateFile state
                newFileState.isDirty = false; // 当前不需要标记为已保存
                // Update state
                QHFStateManager::instance().setFileState(newFileState);
            }
        }
    }

    void QHFAppMainWindow::handleCloseFile(const QHFNotifier& notifier) {
        if (isActiveWindow()) {
            // Returns current File state
            const auto& fileState = QHFStateManager::instance().getFileState();
            // determine是否有file名
            if (fileState.name == "Untitled" && fileState.isDirty) {
                // Tip: 示是否需要save

                // savefile
                FileInfo savedFile;
                if (saveFile(savedFile)) {
                    FileState newFileState = fileState;
                    // savesuccess, updateFile state
                    newFileState.name    = savedFile.fileName;
                    newFileState.path    = savedFile.filePath;
                    newFileState.isDirty = false;
                    // Update state
                    QHFStateManager::instance().setFileState(newFileState);
                    // updatefilelist
                    QHFStateManager::instance().updateRecentFiles();
                }
            }
            // closecurrentfile
            closeFile();
            // open新file
            newFile();
            // resetFile state
            QHFStateManager::instance().resetFileState();
            // Publishes notification
            QHFNotifier noti(QHFNotifierType::FILE_STATE_CLOSED);
            noti.set("source", this);
            publishEvent(this, noti);
        }
    }

    void QHFAppMainWindow::handleFileStateChanged(const QHFNotifier& notifier) {
        switch (notifier.type) {
        case QHFNotifierType::FILE_STATE_MODIFIED:
            handleFileStateModified(notifier);
            break;
        case QHFNotifierType::FILE_STATE_CLOSED:
        case QHFNotifierType::FILE_STATE_OPENED:
        case QHFNotifierType::FILE_STATE_SAVED:
            handleFileStateOtherCases(notifier);
            break;
        default:
            break;
        }
    }

    void QHFAppMainWindow::handleFileStateModified(const QHFNotifier& notifier) {
        if (isActiveWindow()) {
            // Returns current File state
            const auto& fileState = QHFStateManager::instance().getFileState();
            // 新 File state
            FileState newFileState = fileState;
            // Get notification知中 File state
            auto isDirtyOpt = notifier.get<bool>("isDirty");
            if (isDirtyOpt) {
                auto isDirty = isDirtyOpt.value();
                // 判读
                if (isDirty != fileState.isDirty) {
                    // 新 File state
                    newFileState.isDirty = true;
                    // Update state
                    QHFStateManager::instance().setFileState(newFileState);
                }
            }
            updateTitle();
        }
    }

    void QHFAppMainWindow::handleFileStateOtherCases(const QHFNotifier& notifier) { updateTitle(); }

    void QHFAppMainWindow::handleImportFile(const QHFNotifier& notifier) {
        if (isActiveWindow()) {
            // Returns current File state
            const auto& fileState = QHFStateManager::instance().getFileState();
            // 导入file
            if (importFile()) {
                FileState newFileState = fileState;
                // save副本success, updateFile state
                newFileState.isDirty = false; // 当前不需要标记为已保存
                // Update state
                QHFStateManager::instance().setFileState(newFileState);
            }
            // Publishes notification
        }
    }

    void QHFAppMainWindow::handleExportFile(const QHFNotifier& notifier) {
        if (isActiveWindow()) {
            if (exportFile()) {
                // 导出filesuccess, 不updatefile
            }
        }
    }

    void QHFAppMainWindow::handleSetWorkspaceIndex(const QHFNotifier& notifier) {
        if (isActiveWindow()) {
            auto indexOpt = notifier.get<int>("index");
            if (indexOpt) {
                const auto index = indexOpt.value();
                m_workspaceContainer->setCurrentIndex(index);
            }
        }
    }

    void QHFAppMainWindow::handleToggleFullScreen(const QHFNotifier& notifier) {
        // determinecurrent是否activatewindow
        if (isActiveWindow()) {
            if (isFullScreen()) {
                showNormal();
            } else {
                showFullScreen();
            }
        }
    }

    void QHFAppMainWindow::handleSystemExit(const QHFNotifier& notifier) {
        if (isActiveWindow()) { // 判断当前是否激活窗口
            // Returns current File state
            const auto& fileState = QHFStateManager::instance().getFileState();
            // determinefile是否需要save
            if (fileState.isDirty && fileState.name != "Untitled") {
                // Tip: 示是否需要save

                // savefile
                FileInfo savedFile;
                if (saveFile(savedFile)) {
                    FileState newFileState = fileState;
                    // savesuccess, updateFile state
                    newFileState.name    = savedFile.fileName;
                    newFileState.path    = savedFile.filePath;
                    newFileState.isDirty = false;
                    // Update state
                    QHFStateManager::instance().setFileState(newFileState);
                }
            }
            // closecurrentwindow
            close();
        } else {
            close();
        }
    }

    void QHFAppMainWindow::handleToggleStatusBar(const QHFNotifier& notifier) {
        // determinecurrent是否activatewindow
        if (isActiveWindow()) {
            if (m_statusBar->isVisible()) {
                // 隐藏status bar
                m_statusBar->hide();
                // Update state
                QHFStateManager::instance().setStatusBarVisible(this, false);
            } else {
                // 显示status bar
                m_statusBar->show();
                // Update state
                QHFStateManager::instance().setStatusBarVisible(this, true);
            }
        }
    }

    void QHFAppMainWindow::handleNewSimpleWindow(const QHFNotifier& notifier) {
        constexpr int defaultNofifierDelayMs = 10;
        //
        if (isActiveWindow()) {
            // Returns current workspace  index
            int currentIndex = m_workspaceContainer->currentIndex();
            // Returns current workspace
            auto* curWorkspace = m_workspaces.at(currentIndex);
            // 得到currentworkspace type
            QHFWorkspaceType curType = curWorkspace->workspaceType();
            //
            auto* simpleWindow = new QHFAppSimpleWindow(curType, nullptr);
            // leftbottom偏移current this 位置
            auto pos = this->geometry().topLeft();
            //
            simpleWindow->move(pos.x() + subWindowOffset, pos.y() + subWindowOffset);
            // 显示window
            simpleWindow->show();
            // Publishes notification
            QHFNotifier noti(QHFNotifierType::NEW_APP_WINDOW_INFO_WINDOW);
            noti.set("window", simpleWindow);
            publishEvent(this, noti, defaultNofifierDelayMs);
            //
            qDebug() << "Created new simple window:" << simpleWindow;
        }
    }

    void QHFAppMainWindow::handleNewMainWindow(const QHFNotifier& notifier) {
        constexpr int defaultNofifierDelayMs = 10;
        //
        if (isActiveWindow()) {
            // create新window
            auto* newWindow = new QHFAppMainWindow(nullptr, false);
            //
            auto pos = this->geometry().topLeft();
            // leftbottom偏移current this 位置
            newWindow->move(pos.x() + subWindowOffset, pos.y() + subWindowOffset);
            // 复制workspaceLayout
            auto& newWorkspaces = newWindow->m_workspaces;
            if (newWorkspaces.size() == m_workspaces.size()) {
                for (int idx = 0; idx < newWorkspaces.size(); ++idx) {
                    auto* newWorkspace = newWorkspaces.at(idx);
                    auto* curWorkspace = m_workspaces.at(idx);
                    if (newWorkspace->workspaceType() == curWorkspace->workspaceType()) {
                        LayoutNode* cloneRoot = curWorkspace->cloneRoot();
                        if (cloneRoot != nullptr) {
                            newWorkspace->setRoot(cloneRoot);
                        }
                    }
                }
            }
            // 显示新window
            newWindow->show();
            // Publishes notification
            QHFNotifier noti(QHFNotifierType::NEW_APP_WINDOW_INFO_WINDOW);
            noti.set("window", newWindow);
            publishEvent(this, noti, defaultNofifierDelayMs);
            // 记录新window
            qDebug() << "Created new main window:" << newWindow;
        }
    }

    void QHFAppMainWindow::handleReportActivation(const QHFNotifier& notifier) {
        if (isActiveWindow()) {

            auto eventOpt = notifier.get<QHFActivationEvent>("event");
            if (eventOpt) {
                const auto& event = *eventOpt;
                if (event.type == QHFActivationEvent::Type::WORKSPACE_ACTIVE) {
                    auto* workspace = qobject_cast<QHFWorkspaceBase*>(event.source);
                    // checkcurrentworkspace是否exist
                    if (workspace != nullptr) {
                        // Returns current workspace index
                        int index = m_workspaces.indexOf(workspace);
                        if (index != -1) {
                            // Sets current workspace
                            m_activeWorkspace = workspace;
                        }
                    }
                }
            }
        }
    }

    void QHFAppMainWindow::handlePreferencesSettings(const QHFNotifier& notifier) {
        if (isActiveWindow()) {
            // openset对话框
            auto* settingsDialog = new QHFPreferenceDialog(this);
            settingsDialog->show();
        }
    }

    void QHFAppMainWindow::handleGetHelp(const QHFNotifier& notifier) {
        if (isActiveWindow()) {
            auto* contactUsDialog = new QHFContactUsDialog(this);
            contactUsDialog->show();
        }
    }

    void QHFAppMainWindow::handleAboutUs(const QHFNotifier& notifier) {
        if (isActiveWindow()) {
            // openAbout对话框
            auto* aboutUsDialog = new QHFAboutUsDialog(this);
            aboutUsDialog->show();
        }
    }

} // namespace QHF
