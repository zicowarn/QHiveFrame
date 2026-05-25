/**
 * // Usage:
 *
Qt 信号与槽: 
用于本地 UI 交互和紧密耦合 组件通信。

QHFGuiEventBus:
用于全局 UI 交互和松耦合 组件通信。

// 监听 Notifier
connect(&QHF::QHFNotifierCenter::instance(), &QHF::QHFNotifierCenter::notify,
        this, [](const QHF::QHFNotifier& n) {
    if (n.type == "SomethingHappened") {
        QString detail = n.get<QString>("desc");
        qDebug() << "Notifier: Something happened:" << detail;
    }
});

// 发送 Notifier
QHF::QHFNotifier note("SomethingHappened");
note.set("desc", "User clicked button");
QHF::QHFNotifierCenter::instance().publish(note);

事件需携带多种数据和上下文	使用 QHFNotifierCenter::publish()
希望类型安全, 减少拼写错误	使用枚举+转换函数封装事件名称
事件数量庞大且需动态扩展	使用字符串事件名
*/
#ifndef QHFGUINOTIFIERCENTER_H
#define QHFGUINOTIFIERCENTER_H

#include "QHFGuiNotifier.h"

#include <QDebug>
#include <QHash>
#include <QObject>
#include <QTimer>

namespace QHF {

    /**
     * @brief  (see description)
     */
    class QHFNotifierCenter : public QObject {
        Q_OBJECT

      public:
        /**
         * @brief Singleton pattern, Returns notification center instance
         * @return notification中心实例(reference)
         */
        static QHFNotifierCenter& instance();

        /**
         * @brief Publishes notification ( (see description)
         * @param notifier notificationobject* @param delayMs 延迟发送时间(毫秒)
         */
        void publish(const QHFNotifier& notifier, int delayMs = 0);

      signals:
        /**
         * @brief Notification (see description)
         * @param notifier notificationobject*/
        void notify(const QHFNotifier& notifier);

      private:
        QHFNotifierCenter() = default;
    };

    /**
     * @brief  (see description)
     * @details (see source).
     * @tparam T 发送者type (mode、editor或workspace)
     * @param sender 发送者实例* @param event activateevent*/
    template <typename T> void publishActivation(T* sender, const QHFActivationEvent& event) {
        static QHash<T*, QPair<QTimer*, QHFActivationEvent>> timers;
        if (!timers.contains(sender)) {
            auto* timer = new QTimer(sender);
            timers[sender] = {timer, QHFActivationEvent(event.type, nullptr, "")};
            QObject::connect(timer, &QTimer::timeout, sender, [sender]() {
                auto& pair = timers[sender];
                if (pair.second.source) {
                    QHFNotifier noti(QHFNotifierType::REPORT_ACTIVATION);
                    noti.set("event", QVariant::fromValue(pair.second));
                    QHFNotifierCenter::instance().publish(noti);
                    QString eventStr = QHFActivationTypeUtils::toString(pair.second.type);
                    qDebug() << "Published activation event:" << eventStr << "from"
                             << pair.second.source << "reason:" << pair.second.reason;
                }
                pair.second = QHFActivationEvent(); // 重置
            });
        }

        if (event.immediate) {
            // 立即发送
            QHFNotifier noti(QHFNotifierType::REPORT_ACTIVATION);
            noti.set("event", QVariant::fromValue(event));
            QHFNotifierCenter::instance().publish(noti);
            timers[sender].first->stop(); // 取消待处理 延迟通知
            timers[sender].second = QHFActivationEvent(event.type, nullptr, "");
            QString eventStr = QHFActivationTypeUtils::toString(event.type);
            qDebug() << "Immediate activation event:" << eventStr << "from" << event.source;
        } else {
            // 延迟发送, updatecache
            timers[sender].second = event;
            constexpr int mDelayMs = 100; // 延迟发送时间 (毫秒)
            constexpr int eDelayMs = 200; // 事件超时时间 (毫秒)
            constexpr int wDelayMs = 400; // 工作区超时时间 (毫秒)
            int delayMs = 0;
            switch (event.type) {
            case QHFActivationEvent::Type::MODE_ACTIVE:
                delayMs = mDelayMs;
                break;
            case QHFActivationEvent::Type::EDITOR_ACTIVE:
                delayMs = eDelayMs;
                break;
            case QHFActivationEvent::Type::WORKSPACE_ACTIVE:
                delayMs = wDelayMs;
                break;
            default:
                break;
            }
            timers[sender].first->start(delayMs);
        }
    }

    /**
     * @brief  (see description)
     * @details (see source).
     * @tparam T 发送者type (mode、editor或workspace)
     * @param sender 发送者实例* @param notifier eventobject* @param delayMs 延迟发送时间(毫秒)
     */
    template <typename T>
    void publishEvent(T* sender, const QHFNotifier& notifier, int delayMs = 150) {
        static QHash<QPair<T*, QHFNotifierType>, QPair<QTimer*, QHFNotifier>> timers;
        QPair<T*, QHFNotifierType> key{sender, notifier.type};
        if (!timers.contains(key)) {
            auto* timer = new QTimer(sender);
            timers[key] = {timer, QHFNotifier()};
            QObject::connect(timer, &QTimer::timeout, sender, [key, sender]() {
                auto& pair = timers[key];
                if (pair.second.type != QHFNotifierType::NONE) {
                    QHFNotifierCenter::instance().publish(pair.second);
                    qDebug() << "Published event:" << QHFNotifierNameUtils::toString(pair.second.type)
                             << "from" << sender;
                }
                pair.second = QHFNotifier();
            });
        }

        if (delayMs == 0) {
            QHFNotifierCenter::instance().publish(notifier);
            timers[key].first->stop();
            timers[key].second = QHFNotifier();
            qDebug() << "Immediate event:" << QHFNotifierNameUtils::toString(notifier.type) << "from"
                     << sender;
        } else {
            timers[key].second = notifier;
            int finalDelayMs = delayMs;
            if (notifier.type == QHFNotifierType::APP_WINDOW_MOVED) {
                finalDelayMs = 100; // 移动事件更短延迟
            }
            timers[key].first->start(finalDelayMs);
        }
    }

} // namespace QHF

#endif // HGUINOTIFIERCENTER_H
