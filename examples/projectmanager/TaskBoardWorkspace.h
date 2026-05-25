#ifndef TASKBOARDWORKSPACE_H
#define TASKBOARDWORKSPACE_H

#include "QHFWorkspaceBase.h"

class TaskBoardWorkspace : public QHF::QHFWorkspaceBase {
    Q_OBJECT
public:
    explicit TaskBoardWorkspace(QWidget* parent = nullptr, bool isInitialize = true);
    ~TaskBoardWorkspace() override;

    [[nodiscard]] QString               workspaceName() const override;
    [[nodiscard]] QHF::QHFWorkspaceType workspaceType() const override;

private:
    void initializeImpl() override;
    void handleNotifyImpl(const QHF::QHFNotifier& notifier) override;
};

#endif // TASKBOARDWORKSPACE_H
