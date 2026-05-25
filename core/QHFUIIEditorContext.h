#ifndef QHFUIIEDITORCONTEXT_H
#define QHFUIIEDITORCONTEXT_H

#include "QHFGuiNotifierCenter.h"
#include "QHFUIUtils.h"

#include <QHBoxLayout>
#include <QScrollArea>
#include <QVariant>

namespace QHF {

    /**
     * @brief Editor (see description)
     * @details (see source)
     * @note 请在此枚举中添加更多type
     */
    enum class EditorType : std::uint8_t {
        NONE,    ///< Empty type
        DUMMY,   ///< Placeholder type
        SPECIAL, ///< Special type (development example)
        // ... extend with more editor types here
    };

    /**
     * @brief  (see description)
     * @details (see source)
     */
    namespace EditorTypeUtils {
        QHF_DECLARE_NAMESPACE_TR(QHF::EditorTypeUtils);

        /**
         * @brief Converts to string
         * @param mode editortype枚举value* @return string*/
        inline QString toString(EditorType mode) {
            switch (mode) {
            case EditorType::DUMMY:
                return "DUMMY";
            case EditorType::SPECIAL:
                return "SPECIAL";
            // ... extend with more types here
            default:
                return "NONE";
            }
        }

        inline QString toTransName(EditorType mode) {
            switch (mode) {
            case EditorType::DUMMY:
                return tr("DUMMY");
            case EditorType::SPECIAL:
                return tr("SPECIAL");
            // ... extend with more types here
            default:
                return tr("NONE");
            }
        }

        /**
         * @brief Converts from string (see description)
         * @param str string* @return editortype枚举value*/
        inline EditorType fromString(const QString& str) {
            if (str == "DUMMY") {
                return EditorType::DUMMY;
            }
            if (str == "SPECIAL") {
                return EditorType::SPECIAL;
            }
            // ... extend with more types here
            return EditorType::NONE;
        }
    } // namespace EditorTypeUtils

    /**
     * @brief Editor (see description)
     * @details (see source)
     */
    class IEditorContext {
      public:
        /**
         * @brief Destructor
         */
        virtual ~IEditorContext() = default;

        /**
         * @brief Initializes
         */
        virtual void initialize() = 0;

        /// --- Layoutrelated ---
        /**
         * @brief  (see description)
         */
        virtual void requestUpdate() = 0;

        /**
         * @brief  (see description)
         * @return Header Layout pointer, 若无则Returns  nullptr
         */
        [[nodiscard]] virtual QGridLayout* headerLayout() const { return nullptr; }

        /**
         * @brief  (see description)
         * @return Content Layout pointer, 若无则Returns  nullptr
         */
        [[nodiscard]] virtual QHBoxLayout* contentLayout() const { return nullptr; }

        /// --- datarelated ---
        /**
         * @brief Returns current  (see description)
         * @return current modenamestring*/
        [[nodiscard]] virtual QString currentModeName() const = 0;

        /**
         * @brief  (see description)
         * @param modeName modename* @param state statedata*/
        virtual void setModeState(const QString& modeName, const QVariant& state) = 0;

        /**
         * @brief  (see description)
         * @param modeName modename* @return statedata*/
        [[nodiscard]] virtual QVariant getModeState(const QString& modeName) const = 0;

        /**
         * @brief Returns editor name
         * @return Editor name string
         */
        [[nodiscard]] virtual QString editorName() const = 0;

        /**
         * @brief Returns editor type
         * @return editortype*/
        [[nodiscard]] virtual EditorType editorType() const = 0;

        /**
         * @brief  (see description)
         * @param modeName modename*/
        virtual void setEditorPreSelectMode(const QString& modeName) = 0;

        /// --- title管理 ---
        /**
         * @brief  (see description)
         * @param title title*/
        virtual void setEditorTitle(const QString& title) {}

        /**
         * @brief  (see description)
         * @return editortitle*/
        [[nodiscard]] virtual QString editorTitle() const { return {}; }

        /// --- filerelated ---
        /**
         * @brief  (see description)
         * @param path filepath*/
        virtual void setFilePath(const QString& path) {}

        /**
         * @brief  (see description)
         * @return filepath*/
        [[nodiscard]] virtual QString filePath() const { return {}; }

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
        virtual void publishEditorActive(const QString& reason, bool immediate = false) = 0;
    };
} // namespace QHF

#endif // HUIIEDITORCONTEXT_H
