#ifndef QHFUIUTILS_H
#define QHFUIUTILS_H

#include <QApplication>
#include <QWidget>

/**
 * @brief  (see description)
 * @details (see source)
 */
namespace QHF {

#define QHF_DECLARE_NAMESPACE_TR(context)                                                              \
    inline QString tr(const char* sourceText, const char* disambiguation = nullptr, int n = -1) {  \
        return QCoreApplication::translate(#context, sourceText, disambiguation, n);               \
    }

    /**
     * @brief The template class StrongType.
     * @details This class is used to define strong types for task name, task description, task
     * start time, etc.
     * @tparam Tag The tag type.
     */
    template <typename Tag, typename ValueType> struct StrongType {
        // Tag type
        ValueType value;

        // Default constructor for empty value
        StrongType() = default;

        // Allow explicit construction (recommended to avoid implicit conversion)
        explicit StrongType(ValueType val) : value(std::move(val)) {}

        // Provides conversion to ValueType (optional)
        operator ValueType() const { return value; }
    };

    /**
     * @brief Notification (see description)
     * 可Returns currentactivewindow 特定typepointer
     */
    template <typename T> T* activeWindowAs() { return qobject_cast<T*>(qApp->activeWindow()); }
    // 示例: MainWindow* mw = activeWindowAs<MainWindow>();

    /**
     * @brief Checks if  (see description)
     */
    template <typename T> class HasApplyThemeSFINAE {
      private:
        template <typename U>
        static auto test(int) -> decltype(std::declval<U>().applyTheme(), std::true_type());

        template <typename> static std::false_type test(...);

      public:
        static constexpr bool value = decltype(test<T>(0))::value;
    };

    /**
     * @brief Mode (see description)
     * @details (see source)Applies theme
     */
    template <typename Derived, typename SignalType = void()> class QHFThemedCRTP {
      private:
        // 将defaultConstructorset为私有
        QHFThemedCRTP() = default;

        // 将derived class声明为友元
        friend Derived;

      protected:
        /**
         * @brief Applies theme
         * @details (see source)
         */
        template <typename Manager> void initTheme(Manager& manager, SignalType Manager::*signal) {
            if constexpr (HasApplyThemeSFINAE<Derived>::value) {
                QObject::connect(&manager, signal, static_cast<Derived*>(this),
                                 &Derived::applyTheme);
            }
        }
    };

} // namespace QHF

#endif // HUIUTILS_H
