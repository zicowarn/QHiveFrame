#ifndef QHFDUMMYEDITOR_H
#define QHFDUMMYEDITOR_H

#include "QHFEditorBase.h"
#include "QHFEditorFactory.h"
#include "QHFModeBase.h"
#include "QHFModeFactory.h"

#include <QLabel>
#include <QTabWidget>

namespace QHF {

    // Editor typestring
#define REGISTER_DUMMY_MODE(ModeClass, ModeType)                                                   \
    QHF_REGISTER_MODE(EditorTypeUtils::toString(EditorType::DUMMY), ModeClass, ModeType)

    class QHFDummyEditor : public QHFEditorBase {
        Q_OBJECT
      public:
        explicit QHFDummyEditor(QWidget* parent = nullptr);
        ~QHFDummyEditor() override = default; ///< Virtual destructor
        [[nodiscard]] QString editorName() const override;
        [[nodiscard]] EditorType editorType() const override;
        IModeContext*            createMode(const QString& modeName) override;

        [[nodiscard]] QString defaultModeName() const override;

      private:
        /**
         * @brief  (see description)
         * @return SpecialMode namelist, QPair<显示name, name>
         */
        [[nodiscard]] QList<QPair<QString, QString>> collectModeNames() const override;

        /**
         * @brief Initializes (see description)
         * @param modeNames modenamelist, QPair<显示name, mode name>
         */
        void initializeImpl(const QList<QPair<QString, QString>>& modeNames) override;

        /**
         * @brief  (see description)
         * @param notifier notification* @note Subclass may override this function以处理notificationevent
         */
        void handleNotifyImpl(const QHFNotifier& notifier) override;
    };

} // namespace QHF

#endif // HDUMMYEDITOR_H
