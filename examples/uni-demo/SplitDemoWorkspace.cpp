#include "SplitDemoWorkspace.h"
#include "TreeEditor.h"
#include "DetailEditor.h"
#include "QHFEditorFactory.h"

#include <QDebug>

SplitDemoWorkspace::SplitDemoWorkspace(QWidget* parent, bool isInitialize)
    : QHF::QHFWorkspaceBase(parent, isInitialize)
{
}

SplitDemoWorkspace::~SplitDemoWorkspace() = default;

QString SplitDemoWorkspace::workspaceName() const
{
    return QStringLiteral("Split Demo");
}

QHF::QHFWorkspaceType SplitDemoWorkspace::workspaceType() const
{
    return QHF::QHFWorkspaceType::SPLIT_DEMO;
}

void SplitDemoWorkspace::initializeImpl()
{
    setObjectName(QStringLiteral("SplitDemoWorkspace"));
    if (!m_isInitialize) {
        return;
    }

    // Register editors — must call initialize() on each (factory macros do this automatically)
    QHF::QHFEditorRegistry::instance().registerEditor(
        QHF::EditorType::TREE,
        [](QWidget* parent) -> QWidget* {
            auto* editor = new TreeEditor(parent);
            editor->initialize();
            return editor;
        });

    QHF::QHFEditorRegistry::instance().registerEditor(
        QHF::EditorType::DETAIL,
        [](QWidget* parent) -> QWidget* {
            auto* editor = new DetailEditor(parent);
            editor->initialize();
            return editor;
        });

    // Build a split layout: project tree (left) | task detail (right)
    auto* root            = new QHF::LayoutNode(QHF::EditorType::DETAIL);
    root->type            = QHF::LayoutNode::Type::SPLIT;
    root->orientation     = QHF::LayoutNode::Orientation::HORIZONTAL;
    root->firstChild      = new QHF::LayoutNode(QHF::EditorType::TREE);
    root->firstChild->type = QHF::LayoutNode::Type::LEAF;
    root->secondChild     = new QHF::LayoutNode(QHF::EditorType::DETAIL);
    root->secondChild->type = QHF::LayoutNode::Type::LEAF;
    root->splitRatio      = {0.3f, 0.7f};

    setRoot(root);
    qDebug() << "[UniDemo] SplitDemoWorkspace initialized with split layout.";
}

void SplitDemoWorkspace::handleNotifyImpl(const QHF::QHFNotifier& notifier)
{
    Q_UNUSED(notifier);
}
