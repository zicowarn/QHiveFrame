#include "QHFGuiEventBus.h"

#include <QDebug>
#include <QEvent>

#include "QHFThemeManager.h"

namespace QHF {

    /// -------- QHFGuiEventBus implementation --------
    QHFGuiEventBus& QHFGuiEventBus::instance() {
        static QHFGuiEventBus inst;
        return inst;
    }

    // NOLINTBEGIN
    void QHFGuiEventBus::connectEvent(const EventType& eventType, QObject* context,
                                    const EventCallback& callback) {
        if (context == nullptr || !callback) {
            return;
        }

        auto eventName = EventNameUtils::toString(eventType);
        if (eventName.isEmpty()) {
            return;
        }

        // 显式create ListenerEntry 对象
        // 1. create一个具名  QPointer 对象
        // 这个 QPointer (qp_context)  lifecycle将至少持续到current作用域结束, 
        // ensure在 ListenerEntry 对象被复制到 m_listeners 之before, 它所reference  QPointer 是valid 。
        QPointer<QObject> qp_context(context);

        // 2. 使用这个具名  QPointer create ListenerEntry 对象
        // ListenerEntry   context 成员会从 qp_context 拷贝构造。
        ListenerEntry entry(qp_context, callback);

        // 3. 将 ListenerEntry 对象Adds to list中
        // QList::append() 或 QList::push_back() 会复制 'entry' 对象。
        // 'entry' 内部  QPointer 成员也会被正确复制, 新版Qt使用emplace_back()代替push_back()
        m_listeners[eventName].append(entry); // append() 是 QList 常用 方法

        // cleanup死掉 对象
        connect(context, &QObject::destroyed, this, [this, eventName]() {
            auto& list = m_listeners[eventName];
            // 从after向beforeiterate, 安全移除已destroy  QObject corresponding  ListenerEntry
            for (int index = list.size() - 1; index >= 0; --index) {
                if (list[index].context.isNull()) { // QPointer 自动置空
                    list.removeAt(index);
                }
            }
            if (list.isEmpty()) {
                m_listeners.remove(eventName);
            }
        });
    }
    // NOLINTEND

    void QHFGuiEventBus::emitEvent(const EventType& eventType) {
        auto eventName = EventNameUtils::toString(eventType);
        if (eventName.isEmpty()) {
            return;
        }

        auto iter = m_listeners.find(eventName);
        if (iter == m_listeners.end()) {
            return;
        }

        auto& listeners = iter.value();
        for (int index = listeners.size() - 1; index >= 0; --index) {
            if (listeners[index].context.isNull()) {
                listeners.removeAt(index);
                continue;
            }
            if (auto& callback = listeners[index].callback) {
                callback();
            }
        }
    }

} // namespace QHF
