#include "KanbanDemoWorkspace.h"
#include "KanbanEditor.h"
#include "QHFEditorFactory.h"

#include <QDebug>

KanbanDemoWorkspace::KanbanDemoWorkspace(QWidget* parent, bool isInitialize)
    : QHF::QHFWorkspaceBase(parent, isInitialize)
{
}

KanbanDemoWorkspace::~KanbanDemoWorkspace() = default;

QString KanbanDemoWorkspace::workspaceName() const
{
    return QStringLiteral("Kanban Demo");
}

QHF::QHFWorkspaceType KanbanDemoWorkspace::workspaceType() const
{
    return QHF::QHFWorkspaceType::KANBAN_DEMO;
}

void KanbanDemoWorkspace::initializeImpl()
{
    setObjectName(QStringLiteral("KanbanDemoWorkspace"));
    if (!m_isInitialize) {
        return;
    }

    QHF::QHFEditorRegistry::instance().registerEditor(
        QHF::EditorType::KANBAN,
        [](QWidget* parent) -> QWidget* {
            auto* editor = new KanbanEditor(parent);
            editor->initialize();
            return editor;
        });

    auto* root      = new QHF::LayoutNode(QHF::EditorType::KANBAN);
    root->type      = QHF::LayoutNode::Type::LEAF;
    setRoot(root);

    qDebug() << "[UniDemo] KanbanDemoWorkspace initialized.";
}

void KanbanDemoWorkspace::handleNotifyImpl(const QHF::QHFNotifier& notifier)
{
    Q_UNUSED(notifier);
}
