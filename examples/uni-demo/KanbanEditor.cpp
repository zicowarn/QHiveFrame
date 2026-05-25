#include "KanbanEditor.h"
#include "KanbanMode.h"

#include <QDebug>

KanbanEditor::KanbanEditor(QWidget* parent)
    : QHF::QHFEditorBase(parent)
{
}

QString KanbanEditor::editorName() const
{
    return QStringLiteral("Kanban");
}

QHF::EditorType KanbanEditor::editorType() const
{
    return QHF::EditorType::KANBAN;
}

QHF::IModeContext* KanbanEditor::createMode(const QString& modeName)
{
    if (modeName == QStringLiteral("KanbanBoard")) {
        return new KanbanMode(this, this);
    }
    qWarning() << "[UniDemo] Unknown mode for KanbanEditor:" << modeName;
    return nullptr;
}

QString KanbanEditor::defaultModeName() const
{
    return QStringLiteral("KanbanBoard");
}

QList<QPair<QString, QString>> KanbanEditor::collectModeNames() const
{
    return {{tr("Kanban"), QStringLiteral("KanbanBoard")}};
}

void KanbanEditor::initializeImpl(const QList<QPair<QString, QString>>& modeNames)
{
    Q_UNUSED(modeNames);
    // Remove DUMMY/NONE from editor type combo (core placeholder editors)
    for (int i = m_editorCombo->count() - 1; i >= 0; --i) {
        const QString typeName = m_editorCombo->itemData(i).toString();
        if (typeName == QStringLiteral("DUMMY") || typeName == QStringLiteral("NONE")) {
            m_editorCombo->removeItem(i);
        }
    }
    setEditorTitle(tr("Kanban"));
    initializeModeCombo({{tr("Kanban"), QStringLiteral("KanbanBoard")}}, QStringLiteral("KanbanBoard"));
}

void KanbanEditor::handleNotifyImpl(const QHF::QHFNotifier& notifier)
{
    Q_UNUSED(notifier);
}
