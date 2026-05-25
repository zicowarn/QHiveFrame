#ifndef QHFUIIMENUINTERFACE_H
#define QHFUIIMENUINTERFACE_H

#include <QAction>
#include <QString>

#include "QHFGuiEvent.h"
#include "QHFGuiNotifier.h"

namespace QHF {

    enum class MenuItemType : uint8_t {
        NONE,         ///< 空menu项
        ACTION,///< 普通menu项 (带 QAction)
        CUSTOMACTION,///< 自定义menu项 (带 QWidgetAction)
        SUBMENU,///< 子menu (不带 QAction, Represents 一个可展开 menu)
        SEPARATOR ///< separator (no QAction, visual divider only)
    };

    /**
     * @brief Menu (see description)
     */
    struct MenuItem {
        QString       path;                           ///< e.g."Export/Export1"
        QString       transName;                      ///< e.g."Export1"
        int           order;                          ///< e.g.1020
        QAction*      action    = nullptr;            ///< 动作 (nullptrRepresents 子menu)
        MenuItemType  type= MenuItemType::NONE; ///< menu项type
        EventType     eventType= EventType::NONE;    ///< QHFGuiEventBus事件
        QHFNotifierType notifierType =
            QHFNotifierType::NONE; ///< QHFNotifierCenter事件, 如"ComplexExport"
/**
 * @brief Constructor
         * @param path path* @param order 显示编号* @param transName name* @param action 动作* @param type menu项type* @param eventType QHFGuiEventBusEvent type
         * @param notifierType QHFNotifierCenterEvent type
         */
        MenuItem(QString path, int order, QString transName, QAction* action, MenuItemType type,
                 EventType eventType, QHFNotifierType notifierType)
            : path(std::move(path)), transName(std::move(transName)), order(order), action(action),
              type(type), eventType(eventType), notifierType(notifierType) {}

        /**
         * @brief  (see description)
         * path, 显示编号, type为 SEPARATOR
         * @param order 显示编号* @param type type as  SEPARATOR*/
        MenuItem(int order, MenuItemType type = MenuItemType::SEPARATOR)
            : order(order), type(type) {
            // 对于分隔线, action、transName、eventType、notifierType default为其初始value, 不需要extraset
            assert(type ==
                   MenuItemType::SEPARATOR); // 这里 assert可以移除, 因为Constructor直接设置了类型
        }

        /**
         * @brief  (see description)
         * path, name, 显示编号, type为
         * @param path path* @param order 显示编号* @param transName name* @param type type as  SUBMENU*/
        MenuItem(QString path, int order, QString transName,
                 MenuItemType type = MenuItemType::SUBMENU)
            : path(std::move(path)), transName(std::move(transName)), order(order), type(type) {
            assert(type == MenuItemType::SUBMENU);
            // 对于子menu, action、eventType、notifierType default为其初始value
        }

        /**
         * @brief  (see description)
         * path, name, 显示编号, QAction, EventType, type为 ACTION
         * @param path path* @param order 显示编号* @param transName name* @param action QAction
         * @param eventType EventType
         * @param type type as  ACTION*/
        MenuItem(QString path, int order, QString transName, QAction* action, EventType eventType,
                 MenuItemType type = MenuItemType::ACTION)
            : path(std::move(path)), transName(std::move(transName)), order(order), action(action),
              type(type), eventType(eventType) {
            assert(type == MenuItemType::ACTION);
            assert(action != nullptr); // Action 类型必须有 QAction
        }

        /**
         * @brief  (see description)
         * path, name, 显示编号, QAction, QHFNotifierType, type为
         * @param path path* @param order 显示编号* @param transName name* @param action QAction
         * @param notifierType QHFNotifierType
         * @param type type as  ACTION*/
        MenuItem(QString path, int order, QString transName, QAction* action,
                 QHFNotifierType notifierType, MenuItemType type = MenuItemType::ACTION)
            : path(std::move(path)), transName(std::move(transName)), order(order), action(action),
              type(type), notifierType(notifierType) {
            assert(type == MenuItemType::ACTION);
            assert(action != nullptr); // Action 类型必须有 QAction
        }
    };

    /**
     * @brief Menu (see description)
     */
    class IMenuProvider {
      public:
        /**
         * @brief Destructor
         */
        virtual ~IMenuProvider() = default;

        /**
         * @brief  (see description)
         * @return menuname* @note 该namefor在menu栏显示, subclass必须implementation
         */
        [[nodiscard]] virtual QString menuName() const = 0; // 菜单标识, 如"File"

        /**
         * @brief  (see description)
         * @return menu项list* @note subclass必须implementation, 与REGISTER_MENU宏配合使用
         */
        [[nodiscard]] virtual QString transName() const = 0; // 翻译后 显示名, 如tr("File")

        /**
         * @brief  (see description)
         * @return menu项list* @note subclass必须implementation
         */
        [[nodiscard]] virtual QList<MenuItem> menuItems() const = 0;
    };

} // namespace QHF

#endif // HUIIMENUINTERFACE_H
