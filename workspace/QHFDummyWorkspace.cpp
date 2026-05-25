#include "QHFDummyWorkspace.h"

#include "QHFWorkspaceFactory.h"

#include <QDebug>

namespace QHF {

    /// --- QHFDummyWorkspace implementation ---
    // QHF_REGISTER_WORKSPACE(QHFWorkspaceType::DUMMY, QHFDummyWorkspace, QHFDummyWorkspace::SUGGESTED_ORDER)
    QHFDummyWorkspace::QHFDummyWorkspace(QWidget* parent, bool isInitialize)
        : QHFWorkspaceBase(parent, isInitialize) {}

    QString QHFDummyWorkspace::workspaceName() const {
        return QHFWorkspaceTypeUtils::toString(QHFWorkspaceType::DUMMY);
    }

    QHFWorkspaceType QHFDummyWorkspace::workspaceType() const { return QHFWorkspaceType::DUMMY; }

    void QHFDummyWorkspace::initializeImpl() {
        // Set object name
        setObjectName(workspaceName());
        // InitializesLayout
        if (m_isInitialize) {
            // createroot node (SPLIT, horizontalsplit)
            auto* root        = new LayoutNode(EditorType::NONE);
            root->type        = LayoutNode::Type::SPLIT;
            root->orientation = LayoutNode::Orientation::HORIZONTAL;

            /// NOLINTNEXTLINE(readability-magic-numbers)
            root->splitRatio = {0.3f, 0.7f}; // 设置分割比例: 左30%, 右70%

            // createleftchild node (LEAF)
            root->firstChild       = new LayoutNode(EditorType::DUMMY); // 使用DUMMY编辑器
            root->firstChild->type = LayoutNode::Type::LEAF;

            // createrightchild node (LEAF)
            root->secondChild       = new LayoutNode(EditorType::SPECIAL); // 使用TEST编辑器
            root->secondChild->type = LayoutNode::Type::LEAF;
            // setroot node, triggerrebuild
            setRoot(root);
        }
    }

    void QHFDummyWorkspace::handleNotifyImpl(const QHFNotifier& notifier) {}

} // namespace QHF
