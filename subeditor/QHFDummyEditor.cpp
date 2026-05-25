#include "QHFDummyEditor.h"

namespace QHF {

    /// --- QHFDummyEditor 具体implementation ---
    QHF_REGISTER_EDITOR_DEFAULT(QHFDummyEditor, EditorType::DUMMY);
    QHFDummyEditor::QHFDummyEditor(QWidget* parent) : QHFEditorBase(parent) {}

    QString QHFDummyEditor::editorName() const {
        return EditorTypeUtils::toString(EditorType::DUMMY);
    }
    EditorType    QHFDummyEditor::editorType() const
    {
        return EditorType::DUMMY;
    }
    IModeContext* QHFDummyEditor::createMode(const QString& modeName) {
        // check if modeName is valid
        auto type = QHFModeTypeUtils::fromString(modeName);
        if (type == QHFModeType::NONE) {
            return nullptr;
        }
        return QHFEditorBase::createMode(modeName);
    }

    QString QHFDummyEditor::defaultModeName() const {
        return QHFModeTypeUtils::toString(QHFModeType::DUMMY_DEFAULT);
    }

    QList<QPair<QString, QString>> QHFDummyEditor::collectModeNames() const {
        QList<QPair<QString, QString>> modes;
        QString                        editorName = EditorTypeUtils::toString(EditorType::DUMMY);
        QStringList                    modeNames  = QHFModeRegistry::instance().modeNames(editorName);
        if (modeNames.isEmpty()) {
            qWarning() << "No modes registered for" << editorName;
        } else {
            qDebug() << "Modes for" << editorName << ":" << modeNames;
        }
        for (const QString& modeName : modeNames) {
            auto modeType  = QHFModeTypeUtils::fromString(modeName);
            auto transName = QHFModeTypeUtils::transName(modeType);
            modes.append(qMakePair(transName, modeName)); // first: displayName, second: modeName
        }
        return modes;
    }

    void QHFDummyEditor::initializeImpl(const QList<QPair<QString, QString>>& modeNames) {
        initializeModeCombo(modeNames, defaultModeName()); //
    }

    void QHFDummyEditor::handleNotifyImpl(const QHFNotifier& notifier) {}

} // namespace QHF
