#ifndef IMAGEVIEWERMODE_H
#define IMAGEVIEWERMODE_H

#include "QHFModeBase.h"

#include <QLabel>
#include <QPixmap>

/**
 * @brief Image viewer mode — displays a loaded image at fit-to-window scale.
 */
class ImageViewerMode : public QHF::QHFModeBase {
    Q_OBJECT
  public:
    explicit ImageViewerMode(QHF::IEditorContext* context, QWidget* parent = nullptr);
    ~ImageViewerMode() override = default;

    [[nodiscard]] QString modeName() const override;

    /** Load and display an image. */
    void loadImage(const QString& filePath);

  private:
    void handleNotifyImpl(const QHF::QHFNotifier& notifier) override;

    QLabel* m_imageLabel = nullptr;
};

#endif // IMAGEVIEWERMODE_H
