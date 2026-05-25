#include "ProjectManageWorkspace.h"
#include "ProjectTreeEditor.h"
#include "TaskEditor.h"
#include "QHFEditorFactory.h"

#include <QDebug>

ProjectManageWorkspace::ProjectManageWorkspace(QWidget* parent, bool isInitialize)
    : QHF::QHFWorkspaceBase(parent, isInitialize)
{
}

ProjectManageWorkspace::~ProjectManageWorkspace() = default;

QString ProjectManageWorkspace::workspaceName() const
{
    return QStringLiteral("项目管理");
}

QHF::QHFWorkspaceType ProjectManageWorkspace::workspaceType() const
{
    return QHF::QHFWorkspaceType::PM_PROJECT;
}

void ProjectManageWorkspace::initializeImpl()
{
    setObjectName(QStringLiteral("PM_ProjectWorkspace"));
    if (!m_isInitialize) {
        return;
    }

    // Register editors — must call initialize() on each (factory macros do this automatically)
    QHF::QHFEditorRegistry::instance().registerEditor(
        QHF::EditorType::PM_PROJECT_TREE,
        [](QWidget* parent) -> QWidget* {
            auto* editor = new ProjectTreeEditor(parent);
            editor->initialize();
            return editor;
        });

    QHF::QHFEditorRegistry::instance().registerEditor(
        QHF::EditorType::PM_TASK_DETAIL,
        [](QWidget* parent) -> QWidget* {
            auto* editor = new TaskEditor(parent);
            editor->initialize();
            return editor;
        });

    // Build a split layout: project tree (left) | task detail (right)
    auto* root            = new QHF::LayoutNode(QHF::EditorType::PM_TASK_DETAIL);
    root->type            = QHF::LayoutNode::Type::SPLIT;
    root->orientation     = QHF::LayoutNode::Orientation::HORIZONTAL;
    root->firstChild      = new QHF::LayoutNode(QHF::EditorType::PM_PROJECT_TREE);
    root->firstChild->type = QHF::LayoutNode::Type::LEAF;
    root->secondChild     = new QHF::LayoutNode(QHF::EditorType::PM_TASK_DETAIL);
    root->secondChild->type = QHF::LayoutNode::Type::LEAF;
    root->splitRatio      = {0.3f, 0.7f};

    setRoot(root);
    qDebug() << "[PM] ProjectManageWorkspace initialized with split layout.";
}

void ProjectManageWorkspace::handleNotifyImpl(const QHF::QHFNotifier& notifier)
{
    Q_UNUSED(notifier);
}
