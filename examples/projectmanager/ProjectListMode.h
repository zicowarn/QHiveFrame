#ifndef PROJECTLISTMODE_H
#define PROJECTLISTMODE_H

#include "QHFModeBase.h"
#include <QTableWidget>

/**
 * @brief Project list view mode — flat table view of projects with status/count columns.
 */
class ProjectListMode : public QHF::QHFModeBase {
    Q_OBJECT
public:
    explicit ProjectListMode(QHF::IEditorContext* context, QWidget* parent = nullptr);
    ~ProjectListMode() override = default;

    [[nodiscard]] QString modeName() const override;

private:
    void handleNotifyImpl(const QHF::QHFNotifier& notifier) override;
    void populateMockData();

    QTableWidget* m_table = nullptr;
};

#endif // PROJECTLISTMODE_H
