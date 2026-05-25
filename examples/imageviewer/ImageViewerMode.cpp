#include "ImageViewerMode.h"

#include <QVBoxLayout>

ImageViewerMode::ImageViewerMode(QHF::IEditorContext* context, QWidget* parent)
    : QHF::QHFModeBase(context, parent)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_imageLabel = new QLabel(this);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setText(tr("No image loaded.\nUse File \u2192 Open to load an image."));
    layout->addWidget(m_imageLabel);
}

QString ImageViewerMode::modeName() const
{
    return QStringLiteral("ImageViewer");
}

void ImageViewerMode::loadImage(const QString& filePath)
{
    QPixmap pix(filePath);
    if (pix.isNull()) {
        m_imageLabel->setText(tr("Failed to load:\n%1").arg(filePath));
        return;
    }
    m_imageLabel->setPixmap(pix.scaled(m_imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_imageLabel->setText({});
}

void ImageViewerMode::handleNotifyImpl(const QHF::QHFNotifier& notifier)
{
    Q_UNUSED(notifier);
}
