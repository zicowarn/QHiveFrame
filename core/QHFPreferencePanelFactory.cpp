#include "QHFPreferencePanelFactory.h"

#include <QDebug>

namespace QHF {

    /// --- QHFPreferencePanelFactory implementation ---
    QHFPreferencePanelFactory& QHFPreferencePanelFactory::instance() {
        static QHFPreferencePanelFactory instance;
        return instance;
    }

    void QHFPreferencePanelFactory::registerPanelCore(const PanelName& name, int order,
                                                    PanelCreatorFunc creator,
                                                    SettingsFunc     settingsFunc) {
        int finalOrder =
            order >= 0 ? allocateOrder(name, order) : allocateOrder(name, DEFAULT_ORDER_BASE);
        for (const auto& panel : m_panels) {
            if (panel.order == finalOrder) {
                qWarning() << "Order conflict for" << name << ", adjusting from" << finalOrder;
                finalOrder = allocateOrder(name, finalOrder + ORDER_STEP);
            }
        }
        PreferencePanelInfo info{name, finalOrder};
        m_registeredOrders[name] = finalOrder; // 保存 order
        m_creators[name]         = std::move(creator);
        m_settingsFuncs[name]    = std::move(settingsFunc);
        insertPanelInOrder(info);
    }

    QWidget* QHFPreferencePanelFactory::createPanel(const QString& name,
                                                  QWidget*       parent) const { // Create panel
        if (!m_creators.contains(name)) {
            return nullptr;
        }
        return m_creators[name](parent);
    }

    QList<PreferencePanelInfo> QHFPreferencePanelFactory::panels() const {
        QList<PanelInfo> result;
        for (const auto& panel : m_panels) {
            result.append(panel);
        }
        return result;
    }

    int QHFPreferencePanelFactory::getOrder(const QString& panelName) const {
        return m_registeredOrders.value(panelName, DEFAULT_ORDER_BASE +
                                                       (m_registeredOrders.size() * ORDER_STEP));
    }

    int QHFPreferencePanelFactory::allocateOrder(const QString& panelName, int suggestedOrder) {
        if (m_registeredOrders.contains(panelName)) {
            return m_registeredOrders[panelName];
        }
        int newOrder = suggestedOrder;
        while (m_registeredOrders.values().contains(newOrder)) {
            newOrder += ORDER_STEP; // 冲突时递增 100
        }
        return newOrder;
    }

    QList<SettingMeta> QHFPreferencePanelFactory::getRegisteredSettings() const {
        QList<SettingMeta> settings;
        for (const auto& info : m_panels) {
            if (m_settingsFuncs.contains(info.name)) {
                settings.append(m_settingsFuncs[info.name]());
            }
        }
        return settings;
    }

    void QHFPreferencePanelFactory::insertPanelInOrder(const PreferencePanelInfo& info) {
        auto iter = m_panels.begin();
        while (iter != m_panels.end() && iter->order < info.order) {
            ++iter;
        }
        m_panels.insert(iter, info);
    }

} // namespace QHF
