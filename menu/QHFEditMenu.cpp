#include "QHFEditMenu.h"

#include "QHFGuiAction.h"
#include "QHFGuiEvent.h"
#include "QHFGuiNotifier.h"
#include "QHFMenuFactory.h"

namespace QHF {

    // --- QHFEditMenu implementation ---
    QHF_REGISTER_MENU("Edit", QHFEditMenu, QHFEditMenu::SUGGESTED_ORDER)
    QHFEditMenu::QHFEditMenu(QObject* parent) : QHFMenuBase(parent) {}

    QString QHFEditMenu::menuName() const { return "Edit"; }

    QString QHFEditMenu::transName() const { return tr("Edit"); }

    void QHFEditMenu::initializeImpl() {
        // 准备menu项
        auto& registry = QHFMenuRegistry::instance();
        int   index    = 0;
        int   nextID   = 0;
        // 添加“撤销”
        nextID           = registry.getSubOrder("Edit", index++);
        auto* undoAction = new NotifierAction<QHFNotifierType::UNDO>(this);
        undoAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
        addSubMenuItem("Undo", nextID, tr("Undo"), undoAction, QHFNotifierType::UNDO);
        // 添加“重做”
        nextID           = registry.getSubOrder("Edit", index++);
        auto* redoAction = new NotifierAction<QHFNotifierType::REDO>(this);
        redoAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y));
        addSubMenuItem("Redo", nextID, tr("Redo"), redoAction, QHFNotifierType::REDO);
        // 添加“重做历史”
        nextID = registry.getSubOrder("Edit", index++);
        addSubMenuItem("RedoHistory", nextID, tr("Redo History"),
                       new NotifierCustomAction<QHFNotifierType::REDO_HISOTRY>(nullptr, true),
                       QHFNotifierType::REDO_HISOTRY);
        // 添加split线
        nextID = registry.getSubOrder("Edit", index++);
        addSeparator(nextID);
        // 添加“重复”
        nextID             = registry.getSubOrder("Edit", index++);
        auto* repeatAction = new NotifierAction<QHFNotifierType::REPEAT>(this);
        repeatAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
        addSubMenuItem("Repeat", nextID, tr("Repeat"), repeatAction, QHFNotifierType::REPEAT);
        // 添加“重复历史”
        nextID = registry.getSubOrder("Edit", index++);
        addSubMenuItem("RepeatHistory", nextID, tr("Repeat History"),
                       new NotifierCustomAction<QHFNotifierType::REPEAT_HISOTRY>(nullptr, true),
                       QHFNotifierType::REPEAT_HISOTRY);
        // 添加split线
        nextID = registry.getSubOrder("Edit", index++);
        addSeparator(nextID);
        // 添加“set”
        nextID = registry.getSubOrder("Edit", index++);
        auto* preferencesSettingsAction =
            new NotifierAction<QHFNotifierType::PREFERENCES_SETTINGS>(this);
        // 这里set快捷键为“Ctrl+, ”, 因为“Ctrl+P”被“打印”占用了
        preferencesSettingsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Comma));
        addSubMenuItem("PreferencesSetting", nextID, tr("Preferences Settings"),
                       preferencesSettingsAction, QHFNotifierType::PREFERENCES_SETTINGS);
    }

} // namespace QHF
