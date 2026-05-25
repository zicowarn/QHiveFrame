#include "QHFMenuBase.h"

#include <QDebug>
#include <QMenu>

namespace QHF {

    QHFMenuBase::QHFMenuBase(QObject* parent) : QObject(parent) {}

    void QHFMenuBase::initialize() { initializeImpl(); }

    QList<MenuItem> QHFMenuBase::menuItems() const
    {
        return m_items;
    }

    void QHFMenuBase::addSubMenu(const QString& path, int order, const QString& transName) {
        // Create menu项
        MenuItem item{path, order, transName};
        // 断言type为 SUBMENU
        assert(item.type == MenuItemType::SUBMENU);
        // 按 order 插入
        insertItemsInOrder(item);
    }

    void QHFMenuBase::addSeparator(int order) {
        // Create menu项
        MenuItem item{order};
        // 断言type为 SEPARATOR
        assert(item.type == MenuItemType::SEPARATOR);
        // 按 order 插入
        insertItemsInOrder(item);
    }

    void QHFMenuBase::addSubMenuItem(const QString& path, int order, const QString& transName,
                                   QAction* action, EventType eventType) {
        // Create menu项info
        MenuItem item{path, order, transName, action, eventType};
        // 断言type为 ACTION
        assert(item.type == MenuItemType::ACTION);
        // 断言 action 不为empty
        assert(item.action != nullptr);
        // 断言Event type不为empty
        assert(item.eventType != EventType::NONE);
        // 按 order 插入到list
        insertItemsInOrder(item);
    }

    void QHFMenuBase::addSubMenuItem(const QString& path, int order, const QString& transName,
                                   QAction* action, QHFNotifierType notifierType) {
        // Create menu项info
        MenuItem item{path, order, transName, action, notifierType};
        // 断言type为 ACTION
        assert(item.type == MenuItemType::ACTION);
        // 断言 action 不为empty
        assert(item.action != nullptr);
        // 断言Event type不为empty
        assert(item.notifierType != QHFNotifierType::NONE);
        // 按 order 插入到list
        insertItemsInOrder(item);
    }

    void QHFMenuBase::addSubMenuItem(const QString& path, int order, const QString& transName,
                                   QWidgetAction* action, QHFNotifierType notifierType) {
        // Create menu项info
        MenuItem item{
            path,        order, transName, action, MenuItemType::CUSTOMACTION, EventType::NONE,
            notifierType};
        // 断言type为 CUSTOMACTION
        assert(item.type == MenuItemType::CUSTOMACTION);
        // 断言 action 不为empty
        assert(item.action != nullptr);
        // 断言Event type不为empty
        assert(item.notifierType != QHFNotifierType::NONE);
        // 按 order 插入到list
        insertItemsInOrder(item);
    }

    void QHFMenuBase::insertItemsInOrder(const MenuItem& item) {
        auto iter = m_items.begin();
        while (iter != m_items.end() && iter->order < item.order) {
            ++iter;
        }
        m_items.insert(iter, item);
    }

} // namespace QHF
