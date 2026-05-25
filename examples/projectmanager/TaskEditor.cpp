#include "TaskEditor.h"
#include "TaskViewMode.h"
#include "TaskEditMode.h"

#include <QDebug>

TaskEditor::TaskEditor(QWidget* parent)
    : QHF::QHFEditorBase(parent)
{
}

QString TaskEditor::editorName() const
{
    return QStringLiteral("任务详情");
}

QHF::EditorType TaskEditor::editorType() const
{
    return QHF::EditorType::PM_TASK_DETAIL;
}

QHF::IModeContext* TaskEditor::createMode(const QString& modeName)
{
    if (modeName == QStringLiteral("TaskView")) {
        return new TaskViewMode(this, this);
    }
    if (modeName == QStringLiteral("TaskEdit")) {
        return new TaskEditMode(this, this);
    }
    qWarning() << "[PM] Unknown mode for TaskEditor:" << modeName;
    return nullptr;
}

QString TaskEditor::defaultModeName() const
{
    return QStringLiteral("TaskView");
}

QList<QPair<QString, QString>> TaskEditor::collectModeNames() const
{
    return {
        {tr("查看"), QStringLiteral("TaskView")},
        {tr("编辑"), QStringLiteral("TaskEdit")},
    };
}

void TaskEditor::initializeImpl(const QList<QPair<QString, QString>>& modeNames)
{
    Q_UNUSED(modeNames);
    // Remove DUMMY/NONE from editor type combo (core placeholder editors)
    for (int i = m_editorCombo->count() - 1; i >= 0; --i) {
        const QString typeName = m_editorCombo->itemData(i).toString();
        if (typeName == QStringLiteral("DUMMY") || typeName == QStringLiteral("NONE")) {
            m_editorCombo->removeItem(i);
        }
    }
    setEditorTitle(tr("任务"));
    initializeModeCombo(
        {{tr("查看"), QStringLiteral("TaskView")},
         {tr("编辑"), QStringLiteral("TaskEdit")}},
        QStringLiteral("TaskView"));
}

void TaskEditor::handleNotifyImpl(const QHF::QHFNotifier& notifier)
{
    Q_UNUSED(notifier);
}
