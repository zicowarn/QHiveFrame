#ifndef QHFUIIMODECONTEXT_H
#define QHFUIIMODECONTEXT_H

#include "QHFUIUtils.h"

namespace QHF {

    enum class QHFModeType : std::uint8_t {
        NONE,
        // 占位editorrelatedmode
        DUMMY_DEFAULT, ///< Default placeholder editor
        DUMMY_TABBED,  ///< Tabbable placeholder
        // specialeditorrelatedmode
        SPECIAL_DEFAULT, ///< Default special editor
        // ... 在此扩展更多mode type
    };

    namespace QHFModeTypeUtils {
        QHF_DECLARE_NAMESPACE_TR(QHF::QHFModeTypeUtils);

        /**
         * @brief Converts to string
         * @param modeType modetype* @return string*/
        inline QString toString(QHFModeType modeType) {
            switch (modeType) {
            case QHFModeType::DUMMY_DEFAULT:
                return "DUMMY::DEFAULT";
            case QHFModeType::DUMMY_TABBED:
                return "DUMMY::TABBED";
            case QHFModeType::SPECIAL_DEFAULT:
                return "SPECIAL::DEFAULT";
            // ... extend with more types here
            default:
                return "NONE";
            }
        }

        /**
         * @brief Converts from string (see description)
         * @param str string* @return modetype*/
        inline QHFModeType fromString(const QString& str) {
            if (str == "DUMMY::DEFAULT") {
                return QHFModeType::DUMMY_DEFAULT;
            }
            if (str == "DUMMY::TABBED") {
                return QHFModeType::DUMMY_TABBED;
            }
            if (str == "SPECIAL::DEFAULT") {
                return QHFModeType::SPECIAL_DEFAULT;
            }
            // ... extend with more types here
            return QHFModeType::NONE;
        }

        /**
         * @brief  (see description)
         * @param modeType modetype* @return modetypename 翻译*/
        inline QString transName(QHFModeType modeType) {
            switch (modeType) {
            case QHFModeType::DUMMY_DEFAULT:
                return tr("DUMMY::DEFAULT");
            case QHFModeType::DUMMY_TABBED:
                return tr("DUMMY::TABBED");
            case QHFModeType::SPECIAL_DEFAULT:
                return tr("SPECIAL::DEFAULT");
            // ... extend with more types here
            default:
                return tr("NONE");
            }
        }
    } // namespace QHFModeTypeUtils

    /**
     * @brief Mode (see description)
     * @details (see source)
     */
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
