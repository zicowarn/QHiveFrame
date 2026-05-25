/**
 * menu项type: 
 *   - ACTION: 普通menu项 addSubMenuItem("New", registry.getSubOrder("File", 1), tr("New"),
 *       new EventAction<EventType::OPENFILE>(this), EventType::OPENFILE);
 *   - SUBMENU: 子menu项  addSubMenu("Export", registry.getSubOrder("File", 2), tr("Export"));
 *   - SEPARATOR: split线  addSeparator(registry.getSubOrder("File", 3));
 *
 * Action type: 
 *   - Event type: EventType::OPENFILE
 *    处理方式 GuiEventBus::instance().connectEvent(EventType::EXPORT_FILE, this, [=]() {});
 *     或 connect(&QHFNotifierCenter::instance(), &QHFNotifierCenter::notify, this, [=](const QHFNotifier&
 * n){});
 *   - notificationtype: QHFNotifierType::COMPLEX_EXPORT,  处理 connect(&QHFNotifierCenter::instance(),
 * &QHFNotifierCenter::notify, this, [](){});
 */
#ifndef QHFMENUBASE_H
#define QHFMENUBASE_H

#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QObject>
#include <QWidgetAction>

#include "QHFGuiEventBus.h"
#include "QHFUIIMenuProvider.h"

namespace QHF {

    /**
     * @brief Menu (see description)
     * @details (see source)
     */
    class QHFMenuBase : public QObject, public IMenuProvider {
        Q_OBJECT

      protected:
        QList<MenuItem> m_items; ///< menu项list

      public:/**
         * @brief Constructor
         * @param parent Parent object
         */
        explicit QHFMenuBase(QObject* parent = nullptr);
        ~QHFMenuBase() override = default; ///< Destructor
/**
 * @brief Initializes (see description)
         */
        void initialize();

        /**
         * @brief  (see description)
         * @return menu项list*/
        [[nodiscard]] QList<MenuItem> menuItems() const override;

      protected:
        /**
         * @brief  (see description)
         * @param path menupath* @param order menuorder* @param transName menu翻译名*/
        void addSubMenu(const QString& path, int order, const QString& transName);

        /**
         * @brief  (see description)
         * @param order menuorder*/
        void addSeparator(int order);

        /**
         * @brief  (see description)
         * @param path menupath* @param order menuorder* @param transName menu翻译名* @param action menu动作* @param eventType Event type
         */
        void addSubMenuItem(const QString& path, int order, const QString& transName,
                            QAction* action, EventType eventType);

        /**
         * @brief  (see description)
         * @param path menupath* @param order menuorder* @param transName menu翻译名* @param action menu动作* @param notifierType notificationtype*/
        void addSubMenuItem(const QString& path, int order, const QString& transName,
                            QAction* action, QHFNotifierType notifierType);

        /**
         * @brief  (see description)
         * @param path menupath* @param order menuorder* @param transName menu翻译名* @param action 自定义QWidgetAction* @param notifierType notificationtype*/
        void addSubMenuItem(const QString& path, int order, const QString& transName,
                            QWidgetAction* action, QHFNotifierType notifierType);

      private:
        /**
         * @brief Initializes (see description)
         * @details (see source)
         */
        virtual void initializeImpl() = 0;

        /**
         * @brief  (see description)
         * @param item menu项*/
        void insertItemsInOrder(const MenuItem& item);
    };

} // namespace QHF

#endif // HMENUBASE_H
