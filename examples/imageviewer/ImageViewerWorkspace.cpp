#include "ImageViewerWorkspace.h"
#include "ImageViewerEditor.h"
#include "QHFEditorFactory.h"

#include <QDebug>

ImageViewerWorkspace::ImageViewerWorkspace(QWidget* parent, bool isInitialize)
    : QHF::QHFWorkspaceBase(parent, isInitialize)
{
}

QString ImageViewerWorkspace::workspaceName() const
{
    return QStringLiteral("ImageViewer");
}

QHF::QHFWorkspaceType ImageViewerWorkspace::workspaceType() const
{
    return QHF::QHFWorkspaceType::DUMMY;
}

void ImageViewerWorkspace::initializeImpl()
{
    setObjectName(workspaceName());
    if (m_isInitialize) {
        // Register the ImageViewerEditor with the SPECIAL editor type
        // This must match what ImageViewerEditor::editorType() returns.
        // In a real project you would add a new enum value to EditorType instead.
        QHF::QHFEditorRegistry::instance().registerEditor(
            QHF::EditorType::SPECIAL,
            [](QWidget* parent) -> QWidget* {
                return new ImageViewerEditor(parent);
            });

        // Create a root layout node using the SPECIAL editor type
        auto* root        = new QHF::LayoutNode(QHF::EditorType::SPECIAL);
        root->type        = QHF::LayoutNode::Type::LEAF;
        setRoot(root);
    }
}

void ImageViewerWorkspace::handleNotifyImpl(const QHF::QHFNotifier& notifier)
{
    Q_UNUSED(notifier);
}
