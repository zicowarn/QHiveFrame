#include "QHFMenuFactory.h"

#include <QDebug>

namespace QHF {

    /// --- QHFMenuFactory implementation ---
    QHFMenuRegistry& QHFMenuRegistry::instance() {
        static QHFMenuRegistry instance;
        return instance;
    }

    void QHFMenuRegistry::registerMenuCore(const MenuName& name, int order, CreatorFunc creator) {
        int finalOrder =
            order >= 0 ? allocateOrder(name, order) : allocateOrder(name, DEFAULT_ORDER_BASE);
        for (const auto& menu : m_menus) {
            if (menu.order == finalOrder) {
                qWarning() << "Order conflict for" << name << ", adjusting from" << finalOrder;
                finalOrder = allocateOrder(name, finalOrder + ORDER_STEP);
            }
        }
        MenuInfo info{name, finalOrder};
        m_registeredOrders[name] = finalOrder; // 保存 order
        m_creators[name]         = std::move(creator);
        insertMenuInOrder(info);
    }

    IMenuProvider* QHFMenuRegistry::createMenu(const QString& name, QObject* parent) const {
        if (!m_creators.contains(name)) {
            return nullptr;
        }
        return m_creators[name](parent);
    }

    QList<MenuInfo> QHFMenuRegistry::menus() const {
        QList<MenuInfo> result;
        for (const auto& menu : m_menus) {
#ifndef _DEBUG
            if (menu.name == "Debug") {
                qDebug() << "Debug menu found";
                continue;
            }
#endif
            result.append(menu);
        }
        return result;
    }

    int QHFMenuRegistry::getOrder(const QString& menuName) const {
        return m_registeredOrders.value(menuName, DEFAULT_ORDER_BASE +
                                                      (m_registeredOrders.size() * ORDER_STEP));
    }

    int QHFMenuRegistry::getSubOrder(const QString& menuName, int index) const {
        if (index < 0) {
            qWarning() << "Invalid sub-order index for" << menuName << ":" << index;
            return getOrder(menuName); // Returns 基准 order
        }
        return getOrder(menuName) + (index * SUB_ORDER_STEP); // 例如, 1000 + 1*10 = 1010
    }

    int QHFMenuRegistry::allocateOrder(const QString& menuName, int suggestedOrder) {
        if (m_registeredOrders.contains(menuName)) {
            return m_registeredOrders[menuName];
        }
        int newOrder = suggestedOrder;
        while (m_registeredOrders.values().contains(newOrder)) {
            newOrder += ORDER_STEP; // 冲突时递增 100
        }
        return newOrder;
    }

    void QHFMenuRegistry::insertMenuInOrder(const MenuInfo& info) {
        auto iter = m_menus.begin();
        while (iter != m_menus.end() && iter->order < info.order) {
            ++iter;
        }
        m_menus.insert(iter, info);
    }

} // namespace QHF
