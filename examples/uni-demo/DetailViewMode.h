#ifndef TASKVIEWMODE_H
#define TASKVIEWMODE_H

#include "QHFModeBase.h"
#include <QTextEdit>

/**
 * @brief Task view mode — read-only task detail with collapsible sections.
 */
class DetailViewMode : public QHF::QHFModeBase {
    Q_OBJECT
public:
    explicit DetailViewMode(QHF::IEditorContext* context, QWidget* parent = nullptr);
    ~DetailViewMode() override = default;

    [[nodiscard]] QString modeName() const override;

private:
    void handleNotifyImpl(const QHF::QHFNotifier& notifier) override;
    void buildUI();

    QTextEdit* m_detailView = nullptr;
};

#endif // TASKVIEWMODE_H
