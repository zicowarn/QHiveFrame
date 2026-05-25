#ifndef PROJECTTREEMODE_H
#define PROJECTTREEMODE_H

#include "QHFModeBase.h"
#include <QTreeWidget>

/**
 * @brief Project tree view mode — displays project/task hierarchy using QTreeWidget.
 */
class TreeMode : public QHF::QHFModeBase {
    Q_OBJECT
public:
    explicit TreeMode(QHF::IEditorContext* context, QWidget* parent = nullptr);
    ~TreeMode() override = default;

    [[nodiscard]] QString modeName() const override;

private:
    void handleNotifyImpl(const QHF::QHFNotifier& notifier) override;
    void setupTree();
    void populateMockData();

    QTreeWidget* m_tree = nullptr;
};

#endif // PROJECTTREEMODE_H
