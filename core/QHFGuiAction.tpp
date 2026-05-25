#ifndef HGUIACTION_TPP
#define HGUIACTION_TPP

#include "QHFCustomActionWidgetBase.h"
#include "QHFCustomActionWidgetFactory.h"

namespace QHF {

    template <EventType evt>
    QHFCustomActionWidgetBase* EventCustomAction<evt>::createCustomWidget(QWidget* parent) const {
        /*
        if constexpr (evt == EventType::OPEN_RECENTLY) {
            return new RecentFilesWidget(parent);
        } else if constexpr (evt == EventType::REDO_HISOTRY || evt == EventType::REPEAT_HISOTRY) {
            return new HistoryWidget(parent);
        } else {
            return new PushButtonWidget(parent);
        }
        */
        return QHFCustomActionWidgetRegistry::instance().create(QHFCustomActionWidgetType::DUMMY, QHFNotifierType::NONE, parent);
    }

    template <QHFNotifierType evt>
    QHFCustomActionWidgetBase* NotifierCustomAction<evt>::createCustomWidget(QWidget* parent) const {
        /*
        if constexpr (evt == QHFNotifierType::OPEN_RECENTLY) {
            return new RecentFilesWidget(parent);
        } else if constexpr (evt == QHFNotifierType::REDO_HISOTRY ||
                             evt == QHFNotifierType::REPEAT_HISOTRY) {
            return new HistoryWidget(parent);
        } else {
            return new PushButtonWidget(evt, parent);
        }
        */
        return QHFCustomActionWidgetRegistry::instance().create(QHFCustomActionWidgetType::DUMMY, QHFNotifierType::NONE, parent);
    }


} // namespace QHF

#endif // HGUIACTION_TPP
