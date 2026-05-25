/**
 * // Usage:
 *
Qt 信号与槽: 
用于本地 UI 交互和紧密耦合 组件通信。

QHFGuiEventBus:
用于全局 UI 交互和松耦合 组件通信。


// 监听
QHF::QHFGuiEventBus::instance().connectEvent("ToggleFullscreen", this, [=]() {
    qDebug() << "Fullscreen toggled!";
});

// trigger
QHF::QHFGuiEventBus::instance().emitEvent("ToggleFullscreen");

事件只有“发生了”无需传递数据	使用 QHFGuiEventBus::emitEvent()
希望类型安全, 减少拼写错误	使用枚举+转换函数封装事件名称
事件数量庞大且需动态扩展	使用字符串事件名
*/
#ifndef QHFGUIEVENTBUS_H
#define QHFGUIEVENTBUS_H

#include <QList>
#include <QMap>
#include <QObject>
#include <QPointer>

#include "QHFGuiEvent.h"

namespace QHF {

    /**
     * @brief Event bus class, Manages  (see description)
     */
    class QHFGuiEventBus : public QObject {
        Q_OBJECT

      public:
        using EventCallback = std::function<void()>; ///< Event callback类型

      private:
        /**
         * @brief Event listener entry
         */
        struct ListenerEntry {
            QPointer<QObject> context;
            EventCallback     callback;

            ListenerEntry(const QPointer<QObject>& ctx, EventCallback cbf)
                : context(ctx), callback(std::move(cbf)) {}

            ListenerEntry(const ListenerEntry& other)            = default;
            ListenerEntry& operator=(const ListenerEntry& other) = default;
        };

        QMap<QString, QList<ListenerEntry>> m_listeners; ///< event监听器list

      public:/**
         * @brief Singleton pattern, Returns event bus instance
         * @return Event bus实例 (reference)
         */
        static QHFGuiEventBus& instance();

        /**
         * @brief Registers event listener
         * @param eventType Event type
         * @param context 监听者topbottom文(auto跟踪对象destroy)
         * @param callback Event callback
         */
        void connectEvent(const EventType& eventType, QObject* context,
                          const EventCallback& callback);

        /**
         * @brief Emits event ( (see description)
         * @param eventType Event type
         */
        void emitEvent(const EventType& eventType);

      Q_SIGNALS:
        void requestSetEditorActive(QWidget* editor); ///< Signal: 请求Sets current 活动编辑器
        void
        requestSplitActiveHorizontal(QWidget* senderEditor); ///< Signal: 请求水平分割当前活动编辑器
        void
        requestSplitActiveVertical(QWidget* senderEditor); ///< Signal: 请求垂直分割当前活动编辑器
        void requestRemoveActiveEditor(QWidget* senderEditor); ///< Signal: 关闭 (移除)当前活动编辑器
        void
        requestSwitchActiveEditorType(QWidget* senderEditor,
                                      const QString& newType); ///< Signal: 切换当前活动编辑器类型

      private:
        /**
         * @brief Constructor (External creation disabled)
         * @note Singleton pattern, External creation disabled
         */
        QHFGuiEventBus() = default;
    };

} // namespace QHF

#endif // HGUIEVENTBUS_H
