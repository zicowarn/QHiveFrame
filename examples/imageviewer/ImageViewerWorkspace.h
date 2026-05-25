#ifndef IMAGEVIEWERWORKSPACE_H
#define IMAGEVIEWERWORKSPACE_H

#include "QHFWorkspaceBase.h"

class ImageViewerWorkspace : public QHF::QHFWorkspaceBase {
    Q_OBJECT
  public:
    explicit ImageViewerWorkspace(QWidget* parent = nullptr, bool isInitialize = true);
    ~ImageViewerWorkspace() override = default;

    [[nodiscard]] QString                workspaceName() const override;
    [[nodiscard]] QHF::QHFWorkspaceType  workspaceType() const override;

  private:
    void initializeImpl() override;
    void handleNotifyImpl(const QHF::QHFNotifier& notifier) override;
};

#endif // IMAGEVIEWERWORKSPACE_H
