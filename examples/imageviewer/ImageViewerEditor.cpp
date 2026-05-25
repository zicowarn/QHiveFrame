#include "ImageViewerEditor.h"
#include "ImageViewerMode.h"

#include <QDebug>

ImageViewerEditor::ImageViewerEditor(QWidget* parent)
    : QHF::QHFEditorBase(parent)
{
}

QString ImageViewerEditor::editorName() const
{
    return QStringLiteral("Image Viewer");
}

QHF::EditorType ImageViewerEditor::editorType() const
{
    return QHF::EditorType::SPECIAL;
}

QHF::IModeContext* ImageViewerEditor::createMode(const QString& modeName)
{
    if (modeName == QStringLiteral("ImageViewer")) {
        return new ImageViewerMode(this, this);
    }
    qWarning() << "Unknown mode:" << modeName;
    return nullptr;
}

QString ImageViewerEditor::defaultModeName() const
{
    return QStringLiteral("ImageViewer");
}

QList<QPair<QString, QString>> ImageViewerEditor::collectModeNames() const
{
    return {{tr("Viewer"), QStringLiteral("ImageViewer")}};
}

void ImageViewerEditor::initializeImpl(const QList<QPair<QString, QString>>& modeNames)
{
    Q_UNUSED(modeNames);
    setupLayout();
    initializeModeCombo({{tr("Viewer"), QStringLiteral("ImageViewer")}},
                        QStringLiteral("ImageViewer"));
}

void ImageViewerEditor::handleNotifyImpl(const QHF::QHFNotifier& notifier)
{
    Q_UNUSED(notifier);
}
