#ifndef QHFUIIMODECONTEXT_H
#define QHFUIIMODECONTEXT_H

#include "QHFTypeDefs.h"

namespace QHF {

    class IModeContext {

      public:
        /**
         * @brief Destructor
         */
        virtual ~IModeContext() = default;

        /**
         * @brief Initializes (see description)
         * @details (see source)
         */
        virtual void initialize() = 0;

        /**
         * @brief Cleans up mode
         * @details (see source)
         */
        virtual void cleanup() = 0;

        /**
         * @brief Updates content
         * @details (see source))
         */
        virtual void update() = 0;

        /**
         * @brief Returns mode name
         * @return modename*/
        [[nodiscard]] virtual QString modeName() const = 0;

        /**
         * @brief  (see description)
         * @return Header contentwidget
         * @note component必须无Parent object, 父由 QHFEditorBase 管理
         */
        virtual QWidget* createHeaderContent() = 0;

        /**
         * @brief  (see description)
         * @return Mode contentwidget
         * @note recommend在subclass initializeImpl()中完成Layout和widget, 
         * 若需要dynamiccreatecontentwidget可在subclass重载函数中create.noteReturns type
         */
        virtual QWidget* createContent() = 0;

        /**
         * @brief  (see description)
         * @details (see source))
         */
        virtual void connectSignals() = 0;

        /**
         * @brief  (see description)
         * @details (see source)
         */
        virtual void disconnectSignals() = 0;

        /// --- 处理notification ----
        /**
         * @brief  (see description)
         * @param notifier notification*/
        virtual void handleNotify(const QHFNotifier& notifier) = 0;

        /**
         * @brief  (see description)
         * @param notifier notification*/
        virtual void handleNotifyImpl(const QHFNotifier& notifier) = 0;

        /// --- Publishes notification---
        /**
         * @brief Publishes notification
         * @param reason notification原因*/
        virtual void publishModeActive(const QString& reason, bool immediate = false) = 0;
    };

} // namespace QHF

#endif // HUIIMODECONTEXT_H
