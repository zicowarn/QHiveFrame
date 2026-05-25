#include "QHFViewMenu.h"

#include "QHFGuiAction.h"
#include "QHFGuiEvent.h"
#include "QHFGuiNotifier.h"
#include "QHFMenuFactory.h"

namespace QHF {

    /// --- QHFViewMenu implementation ---
    QHF_REGISTER_MENU("View", QHFViewMenu, QHFViewMenu::SUGGESTED_ORDER)
    QHFViewMenu::QHFViewMenu(QObject* parent) : QHFMenuBase(parent) {}

    QString QHFViewMenu::menuName() const { return "View"; }

    QString QHFViewMenu::transName() const { return tr("View"); }

    void QHFViewMenu::initializeImpl() {
        // 准备menu项
        auto& registry = QHFMenuRegistry::instance();
        int   index    = 0;
        int   nextID   = 0;
        // 添加“newwindow”menu项
        nextID = registry.getSubOrder("View", index++);
        addSubMenuItem("NewSimpleWindow", nextID, tr("New Window"),
                       new NotifierAction<QHFNotifierType::NEW_SIMPLE_WINDOW>(this),
                       QHFNotifierType::NEW_SIMPLE_WINDOW);
        // 添加“new主window”menu项
        nextID = registry.getSubOrder("View", index++);
        addSubMenuItem("NewMainWindow", nextID, tr("New Main Window"),
                       new NotifierAction<QHFNotifierType::NEW_MAIN_WINDOW>(this),
                       QHFNotifierType::NEW_MAIN_WINDOW);
        // 添加“全屏”menu项
        nextID = registry.getSubOrder("View", index++);
        addSubMenuItem("ToggleFullScreen", nextID, tr("Toggle Full Screen"),
                       new NotifierAction<QHFNotifierType::TOGGLE_FULL_SCREEN>(this),
                       QHFNotifierType::TOGGLE_FULL_SCREEN);
        // 添加split线
        nextID = registry.getSubOrder("View", index++);
        addSeparator(nextID);
        // 添加“切换workspace”menu项
        nextID                    = registry.getSubOrder("View", index++);
        auto* nextWorkspaceAction = new NotifierAction<QHFNotifierType::NEXT_WORKSPACE>(this);
        nextWorkspaceAction->setShortcut(
            QKeySequence(Qt::CTRL + Qt::Key_PageDown)); // 设置快捷键 Ctrl + PageDown
        addSubMenuItem("NextWorkspace", nextID, tr("Next Workspace"), nextWorkspaceAction,
                       QHFNotifierType::NEXT_WORKSPACE);
        // 添加“切换workspace”menu项
        nextID                    = registry.getSubOrder("View", index++);
        auto* prevWorkspaceAction = new NotifierAction<QHFNotifierType::PREV_WORKSPACE>(this);
        prevWorkspaceAction->setShortcut(
            QKeySequence(Qt::CTRL + Qt::Key_PageUp)); // 设置快捷键 Ctrl + PageUp
        addSubMenuItem("PrevWorkspace", nextID, tr("Previous Workspace"), prevWorkspaceAction,
                       QHFNotifierType::PREV_WORKSPACE);
        // 添加分隔符
        nextID = registry.getSubOrder("View", index++);
        addSeparator(nextID);
        // 添加“切换status bar”menu项
        nextID                      = registry.getSubOrder("View", index++);
        auto* toggleStatusBarAction = new NotifierAction<QHFNotifierType::TOGGLE_STATUS_BAR>(this);
        toggleStatusBarAction->setCheckable(true);
        toggleStatusBarAction->setChecked(true);
        addSubMenuItem("ToggleStatusBar", nextID, tr("Toggle Status Bar"), toggleStatusBarAction,
                       QHFNotifierType::TOGGLE_STATUS_BAR);
        // 添加“截屏”menu项
        nextID = registry.getSubOrder("View", index++);
        addSubMenuItem("SaveScreenshot", nextID, tr("Save Screenshot"),
                       new NotifierAction<QHFNotifierType::SAVE_SCREENSHOT>(this),
                       QHFNotifierType::SAVE_SCREENSHOT);
        // 添加“截屏 (editor)”menu项
        nextID = registry.getSubOrder("View", index++);
        addSubMenuItem("SaveScreenshotEditor", nextID, tr("Save Screenshot (Editor)"),
                       new NotifierAction<QHFNotifierType::SAVE_SCREENSHOT_EDITOR>(this),
                       QHFNotifierType::SAVE_SCREENSHOT_EDITOR);
        // 添加split线
        nextID = registry.getSubOrder("View", index++);
        addSeparator(nextID);
        // 添加“调试window”menu项
        nextID = registry.getSubOrder("View", index++);
        addSubMenuItem("ToggleDebugWindow", nextID, tr("Toggle Debug Window"),
                       new NotifierAction<QHFNotifierType::TOGGLE_DEBUG_WINDOW>(this),
                       QHFNotifierType::TOGGLE_DEBUG_WINDOW);
    }
} // namespace QHF
