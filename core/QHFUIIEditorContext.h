#ifndef QHFUIIEDITORCONTEXT_H
#define QHFUIIEDITORCONTEXT_H

#include "QHFGuiNotifierCenter.h"
#include "QHFTypeDefs.h"

#include <QHBoxLayout>
#include <QScrollArea>
#include <QVariant>

namespace QHF {

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
        [[nodiscard]] virtual QGridLayout* headerLayout() const
        {
            return nullptr;
        }

        /**
         * @brief  (see description)
         * @return Content Layout pointer, 若无则Returns  nullptr
         */
        [[nodiscard]] virtual QHBoxLayout* contentLayout() const
        {
            return nullptr;
        }

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
        [[nodiscard]] virtual QString editorTitle() const
        {
            return {};
        }

        /// --- filerelated ---
        /**
         * @brief  (see description)
         * @param path filepath*/
        virtual void setFilePath(const QString& path) {}

        /**
         * @brief  (see description)
         * @return filepath*/
        [[nodiscard]] virtual QString filePath() const
        {
            return {};
        }

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
