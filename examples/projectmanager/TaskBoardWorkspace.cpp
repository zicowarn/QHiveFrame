#include "TaskBoardWorkspace.h"
#include "KanbanEditor.h"
#include "QHFEditorFactory.h"

#include <QDebug>

TaskBoardWorkspace::TaskBoardWorkspace(QWidget* parent, bool isInitialize)
    : QHF::QHFWorkspaceBase(parent, isInitialize)
{
}

TaskBoardWorkspace::~TaskBoardWorkspace() = default;

QString TaskBoardWorkspace::workspaceName() const
{
    return QStringLiteral("任务看板");
}

QHF::QHFWorkspaceType TaskBoardWorkspace::workspaceType() const
{
    return QHF::QHFWorkspaceType::PM_TASKBOARD;
}

void TaskBoardWorkspace::initializeImpl()
{
    setObjectName(QStringLiteral("PM_TaskBoardWorkspace"));
    if (!m_isInitialize) {
        return;
    }

    QHF::QHFEditorRegistry::instance().registerEditor(
        QHF::EditorType::PM_KANBAN,
        [](QWidget* parent) -> QWidget* {
            auto* editor = new KanbanEditor(parent);
            editor->initialize();
            return editor;
        });

    auto* root      = new QHF::LayoutNode(QHF::EditorType::PM_KANBAN);
    root->type      = QHF::LayoutNode::Type::LEAF;
    setRoot(root);

    qDebug() << "[PM] TaskBoardWorkspace initialized.";
}

void TaskBoardWorkspace::handleNotifyImpl(const QHF::QHFNotifier& notifier)
{
    Q_UNUSED(notifier);
}
