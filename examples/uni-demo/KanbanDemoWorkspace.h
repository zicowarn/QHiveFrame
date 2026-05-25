#ifndef TASKBOARDWORKSPACE_H
#define TASKBOARDWORKSPACE_H

#include "QHFWorkspaceBase.h"

class KanbanDemoWorkspace : public QHF::QHFWorkspaceBase {
    Q_OBJECT
public:
    explicit KanbanDemoWorkspace(QWidget* parent = nullptr, bool isInitialize = true);
    ~KanbanDemoWorkspace() override;

    [[nodiscard]] QString               workspaceName() const override;
    [[nodiscard]] QHF::QHFWorkspaceType workspaceType() const override;

private:
    void initializeImpl() override;
    void handleNotifyImpl(const QHF::QHFNotifier& notifier) override;
};

#endif // TASKBOARDWORKSPACE_H
