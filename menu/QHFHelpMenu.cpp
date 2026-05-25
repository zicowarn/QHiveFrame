#include "QHFHelpMenu.h"

#include "QHFGuiAction.h"
#include "QHFGuiEvent.h"
#include "QHFGuiNotifier.h"
#include "QHFMenuFactory.h"

namespace QHF {
    QHF_REGISTER_MENU("Help", QHFHelpMenu, QHFHelpMenu::SUGGESTED_ORDER)
    QHFHelpMenu::QHFHelpMenu(QObject* parent) : QHFMenuBase(parent) {}

    QString QHFHelpMenu::menuName() const { return "Help"; }

    QString QHFHelpMenu::transName() const { return tr("Help"); }

    void QHFHelpMenu::initializeImpl() {
        // 准备menu项
        auto& registry = QHFMenuRegistry::instance();
        int   index    = 0;
        int   nextID   = 0;
        // 添加“get帮助”
        nextID = registry.getSubOrder("Help", index++);
        addSubMenuItem("GetHelp", nextID, tr("Get Help"),
                       new NotifierAction<QHFNotifierType::GET_HELP>(this), QHFNotifierType::GET_HELP);
        // 添加“使用手册”
        nextID = registry.getSubOrder("Help", index++);
        addSubMenuItem("MemualDoc", nextID, tr("Memual Doc"),
                       new NotifierAction<QHFNotifierType::MEMUAL_DOC>(this),
                       QHFNotifierType::MEMUAL_DOC);
        // 添加“About我们”
        nextID = registry.getSubOrder("Help", index++);
        addSubMenuItem("AboutUs", nextID, tr("About Us"),
                       new NotifierAction<QHFNotifierType::ABOUT_US>(this), QHFNotifierType::ABOUT_US);
    }

} // namespace QHF
