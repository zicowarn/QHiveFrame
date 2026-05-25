#ifndef PROJECTMANAGEWORKSPACE_H
#define PROJECTMANAGEWORKSPACE_H

#include "QHFWorkspaceBase.h"

class ProjectManageWorkspace : public QHF::QHFWorkspaceBase {
    Q_OBJECT
public:
    explicit ProjectManageWorkspace(QWidget* parent = nullptr, bool isInitialize = true);
    ~ProjectManageWorkspace() override;

    [[nodiscard]] QString               workspaceName() const override;
    [[nodiscard]] QHF::QHFWorkspaceType workspaceType() const override;

private:
    void initializeImpl() override;
    void handleNotifyImpl(const QHF::QHFNotifier& notifier) override;
};

#endif // PROJECTMANAGEWORKSPACE_H
