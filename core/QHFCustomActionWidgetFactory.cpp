#include "QHFCustomActionWidgetFactory.h"

namespace QHF {
    /// --- QHFCustomActionWidgetFactory implementation ---
    QHFCustomActionWidgetRegistry& QHFCustomActionWidgetRegistry::instance() {
        static QHFCustomActionWidgetRegistry inst;
        return inst;
    }

    void QHFCustomActionWidgetRegistry::registerCustomActionWidget(QHFCustomActionWidgetType type,
                                                                 CreatorFunc             creator) {
        QString typeName     = QHFCustomActionWidgetTypeUtils::toString(type);
        m_creators[typeName] = std::move(creator);
    }

    QHFCustomActionWidgetBase* QHFCustomActionWidgetRegistry::create(QHFCustomActionWidgetType type,
                                                                 ActionType              actionType,
                                                                 QWidget* parent) const {
        QString typeName = QHFCustomActionWidgetTypeUtils::toString(type);
        auto    iter     = m_creators.find(typeName);
        return (iter != m_creators.end()) ? iter.value()(actionType, parent) : nullptr;
    }

    QStringList QHFCustomActionWidgetRegistry::customActionWidgetNames() const {
        QStringList names; // m_creators.keys() QMap<QString, CreatorFunc>
        for (auto iter = m_creators.constBegin(); iter != m_creators.constEnd(); ++iter) {
            names.append(iter.key());
        }
        return names;
    }

} // namespace QHF
