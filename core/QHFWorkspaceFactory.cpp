#include "QHFWorkspaceFactory.h"

#include <QDebug>

namespace QHF {
    QHFWorkspaceRegistry& QHFWorkspaceRegistry::instance() {
        static QHFWorkspaceRegistry instance;
        return instance;
    }

    void QHFWorkspaceRegistry::registerWorkspaceCore(const QHFWorkspaceType& type, int order,
                                                   CreatorFunc creator) {
        QString name = QHFWorkspaceTypeUtils::toString(type);
        int finalOrder =
            order >= 0 ? allocateOrder(name, order) : allocateOrder(name, DEFAULT_ORDER_BASE);
        for (const auto& workspace : m_workspaces) {
            if (workspace.order == finalOrder) {
                qWarning() << "Order conflict for" << name << ", adjusting from" << finalOrder;
                finalOrder = allocateOrder(name, finalOrder + ORDER_STEP);
            }
        }

        WorkspaceInfo info{name, type, finalOrder};
        m_registeredOrders[name] = finalOrder; // 保存 order
        m_creators[name] = std::move(creator);
        insertWorkspaceInOrder(info); // 插入工作区信息
    }

    QHFWorkspaceBase* QHFWorkspaceRegistry::createWorkspace(const QHFWorkspaceType& type, QWidget* parent,
                                                        bool isInitialize) const {
        QString name = QHFWorkspaceTypeUtils::toString(type);
        if (!m_creators.contains(name)) {
            return nullptr;
        }
        qDebug() << "Creating workspace" << name;
        return m_creators[name](parent, isInitialize);
    }

    QList<WorkspaceInfo> QHFWorkspaceRegistry::workspaces() const
    {
        return m_workspaces;
    }

    int QHFWorkspaceRegistry::getOrder(const QString& workspaceName) const {
        return m_registeredOrders.value(
            workspaceName, DEFAULT_ORDER_BASE + (m_registeredOrders.size() * ORDER_STEP));
    }

    int QHFWorkspaceRegistry::allocateOrder(const QString& workspaceName, int suggestedOrder) {
        if (m_registeredOrders.contains(workspaceName)) {
            return m_registeredOrders[workspaceName];
        }
        int newOrder = suggestedOrder;
        while (m_registeredOrders.values().contains(newOrder)) {
            newOrder += ORDER_STEP; // 冲突时递增 100
        }
        return newOrder;
    }

    void QHFWorkspaceRegistry::insertWorkspaceInOrder(const WorkspaceInfo& info) {
        auto iter = m_workspaces.begin();
        while (iter != m_workspaces.end() && iter->order < info.order) {
            ++iter;
        }
        m_workspaces.insert(iter, info);
    }

} // namespace QHF
