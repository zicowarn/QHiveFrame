#ifndef QHFCUSTOMACTIONWIDGETBASE_H
#define QHFCUSTOMACTIONWIDGETBASE_H

#include <QVariantMap>
#include <QWidget>
#include <type_traits>
#include <variant>

#include "QHFGuiEventBus.h"
#include "QHFGuiNotifierCenter.h"
#include "QHFUIUtils.h"

namespace QHF {

    enum class QHFCustomActionWidgetType : std::uint8_t {
        NONE,
        DUMMY
    };

    namespace QHFCustomActionWidgetTypeUtils {
        /**
         * @brief Converts to string
         * @param type 自定义动作widgettype* @return string*/
        inline QString toString(QHFCustomActionWidgetType type) {
            switch (type) {
            case QHFCustomActionWidgetType::DUMMY:
                return "DUMMY";
            default:
                return "NONE";
            }
        }

        /**
         * @brief Converts from string (see description)
         * @param str string* @return 自定义动作widgettype*/
        inline QHFCustomActionWidgetType fromString(const QString& str) {
            if (str == "DUMMY") {
                return QHFCustomActionWidgetType::DUMMY;
            }
            return QHFCustomActionWidgetType::NONE;
        }

    } // namespace QHFCustomActionWidgetTypeUtils

    using ActionType = std::variant<QHFNotifierType, EventType>;

    class QHFCustomActionWidgetBase : public QWidget, QHFThemedCRTP<QHFCustomActionWidgetBase> {
        Q_OBJECT

      protected:
        ActionType m_type;

      signals:
        void finished();

      public:
        /**
         * @brief Constructor
         * @param type 动作type* @param parent 父级pointer*/
        QHFCustomActionWidgetBase(ActionType type, QWidget* parent = nullptr);
        ~QHFCustomActionWidgetBase() override = default; ///< DefaultDestructor
/**
 * @brief Initializes
         * @details (see source)
         */
        void initialize();

        /**
         * @brief  (see description)
         * @details (see source)
         */
        virtual void updateContext() = 0;

        /**
         * @brief  (see description)
         * @details (see source)
         * @return datamap*/
        [[nodiscard]] virtual QVariantMap getData() const = 0;

        /**
         * @brief  (see description)
         * @details (see source)
         * @return 动作type*/
        [[nodiscard]] virtual ActionType getActionType() const = 0;

      protected:
        /**
         * @brief  (see description)
         * @param event 绘制eventpointer*/
        void paintEvent(QPaintEvent* event) override;

        /**
         * @brief  (see description)
         * @details (see source)
         */
        virtual void applyTheme() = 0;

      private:
        /**
         * @brief Initializes (see description)
         * @details (see source)
         */
        virtual void initializeImpl() = 0;
    };

    template <typename Func> void dispatchActionType(const ActionType& type, Func&& func) {
        std::visit(
            [&func](const auto& typ) {
                if constexpr (std::is_same_v<std::decay_t<decltype(typ)>, QHFNotifierType>) {
                    func(QHFNotifierCenter::instance(), typ);
                } else if constexpr (std::is_same_v<std::decay_t<decltype(typ)>, EventType>) {
                    func(QHFGuiEventBus::instance(), typ);
                }
            },
            type);
    }

} // namespace QHF

#endif // HCUSTOMACTIONWIDGETBASE_H
