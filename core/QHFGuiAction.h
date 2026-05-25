#ifndef QHFGUIACTION_H
#define QHFGUIACTION_H

#include <QAction>
#include <QDebug>
#include <QMenu>
#include <QPushButton>
#include <QToolBar>
#include <QWidgetAction>

#include "QHFCustomActionWidgetBase.h"
#include "QHFGuiEventBus.h"
#include "QHFGuiNotifierCenter.h"

namespace QHF {

    namespace { // 内部实现
        QWidget* findParentContainer(QWidget* obj) {
            while (obj != nullptr) {
                if (qobject_cast<QMenu*>(obj) != nullptr ||
                    qobject_cast<QToolBar*>(obj) != nullptr) {
                    return obj;
                }
                obj = obj->parentWidget();
            }
            return nullptr;
        }
    } // namespace

    template <EventType evt> class EventAction : public QAction {
      public:
        explicit EventAction(QObject* parent = nullptr) : QAction(parent) {
            connect(this, &QAction::triggered, this,
                    []() { QHFGuiEventBus::instance().emitEvent(evt); });
        }
    };

    /**
     * @brief Custom (see description)
     * @details (see source)
     * 时triggersignal, 并阻止Menuclose
     * @tparam evt Event type
     * @note subclass必须implementationcreateCustomWidget方法, 使用this作为widget 父
     */
    template <QHFNotifierType evt> class NotifierAction : public QAction {
      public:
        /**
         * @brief Constructor
         * @param parent Parent objectpointer, current环境无法get准确父时可传入nullptr
         */
        explicit NotifierAction(QObject* parent = nullptr) : QAction(parent) {
            connect(this, &QAction::triggered, this, []() {
                QHFNotifier note(evt);
                note.set("source", QString("MenuAction"));
                QHFNotifierCenter::instance().publish(note);
            });
        }
    };

    /**
     * @brief Custom (see description)
     * @details (see source)
     * 时triggersignal, 并阻止Menuclose
     * @tparam evt Event type
     * @note subclass必须implementationcreateCustomWidget方法, 使用this作为widget 父
     */
    template <EventType evt> class EventCustomAction : public QWidgetAction {
      private:
        QWidget* m_parent    = nullptr; ///< Parent object指针
        bool     m_autoClose = false;   ///< is 否自动关闭menu

      public:/**
         * @brief Constructor
         * @param parent Parent objectpointer, current环境无法get准确父时可传入nullptr
         */
        explicit EventCustomAction(QObject* parent = nullptr, bool autoClose = false)
            : QWidgetAction(parent), m_autoClose(autoClose) {
            connect(this, &QAction::triggered, this, [=]() {
                if (m_autoClose) {
                    if (auto* container =
                            findParentContainer(m_parent ? m_parent : parentWidget())) {
                        container->close();
                    }
                }
            });
        }

        /**
         * @brief  (see description)
         * @param parent Parent objectpointer, current环境无法get准确父时可传入nullptr
         * @return Returns widgetpointer
         */
        QWidget* createWidget(QWidget* parent) override {
            // update父, 此处父为调用者, QMenu或者QToolBar
            if (m_parent == nullptr) {
                m_parent = parent;
            }
            auto* widget = createCustomWidget(m_parent);
            connect(widget, &QHFCustomActionWidgetBase::finished, this, &QWidgetAction::trigger);
            return widget;
        }

      protected:
        /**
         * @brief  (see description)
         * @details (see source)Signal-slot
         * @return Returns 自定义widgetpointer
         * @note subclass必须implementation此方法, 使用this作为widget 父
         */
        virtual QHFCustomActionWidgetBase* createCustomWidget(QWidget* parent) const;
    };

    /**
     * @brief Custom (see description)
     * @details (see source)
     * 时triggersignal, 并阻止Menuclose
     */
    template <QHFNotifierType evt> class NotifierCustomAction : public QWidgetAction {
      private:
        QWidget* m_parent    = nullptr;
        bool     m_autoClose = false;

      public:
        /**
         * @brief Constructor
         * @param parent Parent objectpointer, current环境无法get准确父时可传入nullptr
         * @param autoClose is 否autoclosemenu*/
        explicit NotifierCustomAction(QObject* parent = nullptr, bool autoClose = false)
            : QWidgetAction(parent), m_autoClose(autoClose) {
            connect(this, &QAction::triggered, this, [=]() {
                if (m_autoClose) {
                    if (auto* container =
                            findParentContainer(m_parent ? m_parent : parentWidget())) {
                        container->close();
                    }
                }
            });
        }

        /**
         * @brief  (see description)
         * @param parent Parent objectpointer, current环境无法get准确父时可传入nullptr
         * @return Returns widgetpointer
         */
        QWidget* createWidget(QWidget* parent) override {
            // update父, 此处父为调用者, QMenu或者QToolBar
            if (m_parent == nullptr) {
                m_parent = parent;
            }
            auto* widget = createCustomWidget(m_parent);
            connect(widget, &QHFCustomActionWidgetBase::finished, this, &QWidgetAction::trigger);
            return widget;
        }

      protected:
        /**
         * @brief  (see description)
         * @details (see source)Signal-slot
         * @return Returns 自定义widgetpointer
         * @note subclass必须implementation此方法, 使用this作为widget 父
         */
        virtual QHFCustomActionWidgetBase* createCustomWidget(QWidget* parent) const;
    };
} // namespace QHF

#include "QHFGuiAction.tpp"

#endif // HGUIACTION_H
