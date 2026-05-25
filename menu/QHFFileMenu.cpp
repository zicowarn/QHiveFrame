#include "QHFFileMenu.h"

#include <QKeySequence>

#include "QHFGuiAction.h"
#include "QHFGuiEvent.h"
#include "QHFGuiNotifier.h"
#include "QHFMenuFactory.h"

namespace QHF {

    /// --- QHFFileMenu implementation ---
    QHF_REGISTER_MENU("File", QHFFileMenu, QHFFileMenu::SUGGESTED_ORDER)
    QHFFileMenu::QHFFileMenu(QObject* parent) : QHFMenuBase(parent) {}

    QString QHFFileMenu::menuName() const { return "File"; }

    QString QHFFileMenu::transName() const { return tr("File"); }

    void QHFFileMenu::initializeImpl() {
        // 准备menu项
        auto& registry = QHFMenuRegistry::instance();
        int   index    = 0;
        int   nextID   = 0;
        // 添加“new”menu项
        nextID          = registry.getSubOrder("File", index++);
        auto* newAction = new NotifierAction<QHFNotifierType::NEW_FILE>(this);
        newAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
        addSubMenuItem("New", nextID, tr("New"), newAction, QHFNotifierType::NEW_FILE);
        // 添加“open”menu项
        nextID           = registry.getSubOrder("File", index++);
        auto* openAction = new NotifierAction<QHFNotifierType::OPEN_FILE>(this);
        openAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
        addSubMenuItem("Open", nextID, tr("Open"), openAction, QHFNotifierType::OPEN_FILE);
        // 添加“最近open”menu项
        nextID = registry.getSubOrder("File", index++);
        addSubMenuItem("OpenRecently", nextID, tr("Open Recently"),
                       new NotifierCustomAction<QHFNotifierType::OPEN_RECENTLY>(this, false),
                       QHFNotifierType::OPEN_RECENTLY);
        // split
        nextID = registry.getSubOrder("File", index++);
        addSeparator(nextID);
        // 添加“save”menu项
        nextID           = registry.getSubOrder("File", index++);
        auto* saveAction = new NotifierAction<QHFNotifierType::SAVE_FILE>(this);
        saveAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
        addSubMenuItem("Save", nextID, tr("Save"), saveAction, QHFNotifierType::SAVE_FILE);
        // 添加“另存为”menu项
        nextID             = registry.getSubOrder("File", index++);
        auto* saveAsAction = new NotifierAction<QHFNotifierType::SAVE_AS>(this);
        saveAsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
        addSubMenuItem("SaveAs", nextID, tr("Save As"), saveAsAction, QHFNotifierType::SAVE_AS);
        // 添加“closefile”menu项
        nextID            = registry.getSubOrder("File", index++);
        auto* closeAction = new NotifierAction<QHFNotifierType::CLOSE_FILE>(this);
        closeAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_W));
        addSubMenuItem("Close", nextID, tr("Close file"), closeAction, QHFNotifierType::CLOSE_FILE);
        // split
        nextID = registry.getSubOrder("File", index++);
        addSeparator(nextID);
        // 添加“导入”menu项
        nextID = registry.getSubOrder("File", index++);
        addSubMenu("Import", nextID, tr("Import"));
        // 添加“导入1”menu子项
        nextID = registry.getSubOrder("File", index++);
        addSubMenuItem("Import/Import1", nextID, tr("Import1"),
                       new NotifierAction<QHFNotifierType::IMPORT_FILE_1>(this),
                       QHFNotifierType::IMPORT_FILE_1);
        // 添加“导入2”menu子项
        nextID = registry.getSubOrder("File", index++);
        addSubMenuItem("Import/Import2", nextID, tr("Import2"),
                       new NotifierAction<QHFNotifierType::IMPORT_FILE_2>(this),
                       QHFNotifierType::IMPORT_FILE_2);
        // 添加”导出“menu项
        nextID = registry.getSubOrder("File", index++);
        addSubMenu("Export", nextID, tr("Export"));
        // 添加”导出1“menu子项
        nextID = registry.getSubOrder("File", index++);
        addSubMenuItem("Export/Export1", nextID, tr("Export1"),
                       new NotifierAction<QHFNotifierType::EXPORT_FILE_1>(this),
                       QHFNotifierType::EXPORT_FILE_1);
        // 添加”导出2“menu子项
        nextID = registry.getSubOrder("File", index++);
        addSubMenuItem("Export/Export2", nextID, tr("Export2"),
                       new NotifierAction<QHFNotifierType::EXPORT_FILE_2>(this),
                       QHFNotifierType::EXPORT_FILE_2);
        // split
        nextID = registry.getSubOrder("File", index++);
        addSeparator(nextID);
        // 添加“退出”menu项
        nextID           = registry.getSubOrder("File", index++);
        auto* exitAction = new NotifierAction<QHFNotifierType::SYSTEM_EXIT>(this);
        exitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
        addSubMenuItem("Exit", nextID, tr("Exit"), exitAction, QHFNotifierType::SYSTEM_EXIT);
    }
} // namespace QHF
