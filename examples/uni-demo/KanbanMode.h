#ifndef KANBANMODE_H
#define KANBANMODE_H

#include "QHFModeBase.h"
#include <QListWidget>
#include <QSplitter>

/**
 * @brief Kanban board mode — three-column (TODO/DOING/DONE) task board.
 */
class KanbanMode : public QHF::QHFModeBase {
    Q_OBJECT
public:
    explicit KanbanMode(QHF::IEditorContext* context, QWidget* parent = nullptr);
    ~KanbanMode() override = default;

    [[nodiscard]] QString modeName() const override;

private:
    void handleNotifyImpl(const QHF::QHFNotifier& notifier) override;
    void buildBoard();
    QWidget* createColumn(const QString& title, const QStringList& tasks);

    QSplitter* m_splitter = nullptr;
};

#endif // KANBANMODE_H
