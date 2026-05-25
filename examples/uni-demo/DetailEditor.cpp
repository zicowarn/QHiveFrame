#include "DetailEditor.h"
#include "DetailViewMode.h"
#include "DetailEditMode.h"

#include <QDebug>

DetailEditor::DetailEditor(QWidget* parent)
    : QHF::QHFEditorBase(parent)
{
}

QString DetailEditor::editorName() const
{
    return QStringLiteral("Detail");
}

QHF::EditorType DetailEditor::editorType() const
{
    return QHF::EditorType::DETAIL;
}

QHF::IModeContext* DetailEditor::createMode(const QString& modeName)
{
    if (modeName == QStringLiteral("TaskView")) {
        return new DetailViewMode(this, this);
    }
    if (modeName == QStringLiteral("TaskEdit")) {
        return new DetailEditMode(this, this);
    }
    qWarning() << "[UniDemo] Unknown mode for DetailEditor:" << modeName;
    return nullptr;
}

QString DetailEditor::defaultModeName() const
{
    return QStringLiteral("TaskView");
}

QList<QPair<QString, QString>> DetailEditor::collectModeNames() const
{
    return {
        {tr("View"), QStringLiteral("TaskView")},
        {tr("Edit"), QStringLiteral("TaskEdit")},
    };
}

void DetailEditor::initializeImpl(const QList<QPair<QString, QString>>& modeNames)
{
    Q_UNUSED(modeNames);
    // Remove DUMMY/NONE from editor type combo (core placeholder editors)
    for (int i = m_editorCombo->count() - 1; i >= 0; --i) {
        const QString typeName = m_editorCombo->itemData(i).toString();
        if (typeName == QStringLiteral("DUMMY") || typeName == QStringLiteral("NONE")) {
            m_editorCombo->removeItem(i);
        }
    }
    setEditorTitle(tr("Task"));
    initializeModeCombo(
        {{tr("View"), QStringLiteral("TaskView")},
         {tr("Edit"), QStringLiteral("TaskEdit")}},
        QStringLiteral("TaskView"));
}

void DetailEditor::handleNotifyImpl(const QHF::QHFNotifier& notifier)
{
    Q_UNUSED(notifier);
}
