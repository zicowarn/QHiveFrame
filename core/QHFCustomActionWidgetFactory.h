#ifndef QHFCUSTOMACTIONWIDGETFACTORY_H
#define QHFCUSTOMACTIONWIDGETFACTORY_H

#include <QMap>
#include <QWidget>
#include <functional>
#include <memory>
#include <type_traits>

#include "QHFCustomActionWidgetBase.h"

namespace QHF {

    namespace { // non-exported namespace
        /**
         * @brief  (see description)
         * @tparam TCustomActionWidget 自定义动作widgettype
         * @details (see source)CoreCustomActionWidgetInitializer
         */
        template <typename TCustomActionWidget> struct CoreCustomActionWidgetInitializer {
            void operator()(TCustomActionWidget* widget) const { widget->initialize(); }
        };

        template <typename TCustomActionWidget> class CustomActionWidgetFactory {
            static_assert(std::is_base_of_v<QHFCustomActionWidgetBase, TCustomActionWidget>,
                          "TCustomActionWidget must derive from QHFCustomActionWidgetBase");

          public:
            static TCustomActionWidget* Create(ActionType type, QWidget* parent) {
                auto widget = new TCustomActionWidget(type, parent);
                CoreCustomActionWidgetInitializer<TCustomActionWidget>{}(
                    static_cast<TCustomActionWidget*>(widget)); // 默认调用initialize
                return widget;
            }
        };
    } // namespace

    /**
     * @brief Custom (see description)
     * @details (see source)
     */
    class QHFCustomActionWidgetRegistry {
      public:
        using CreatorFunc = std::function<QHFCustomActionWidgetBase*(ActionType, QWidget*)>;

      private:
        QMap<QString, CreatorFunc> m_creators;

      public:
        /**
         * @brief  (see description)
         * @return 自定义动作widgetRegister 器实例*/
        static QHFCustomActionWidgetRegistry& instance();

        /**
         * @brief Registers custom action widget (see description)
         * @param type 自定义动作widgettype* @param creator 自定义动作widgetCreate function*/
        void registerCustomActionWidget(QHFCustomActionWidgetType type, CreatorFunc creator);

        /**
         * @brief  (see description)
         * @param type 自定义动作widgettype* @param parent Parent object
         * @return new 自定义动作widget*/
        QHFCustomActionWidgetBase* create(QHFCustomActionWidgetType type, ActionType actionType,
                                        QWidget* parent) const;

        [[nodiscard]] QStringList customActionWidgetNames() const;
    };

#define QHF_REGISTER_CUSTOM_WIDGET(QHFCustomActionWidgetType, CustomActionWidgetClass)            \
    static bool _registered_##CustomActionWidgetClass = []() {                                     \
        QHFCustomActionWidgetRegistry::instance().registerCustomActionWidget(                        \
            QHFCustomActionWidgetType,                                                               \
            [](ActionType actionType, QWidget* parent) -> QHFCustomActionWidgetBase* {               \
                return CustomActionWidgetFactory<CustomActionWidgetClass>::Create(actionType,      \
                                                                                  parent);         \
            });                                                                                    \
        return true;                                                                               \
    }();

} // namespace QHF

#endif // HCUSTOMACTIONWIDGETFACTORY_H
