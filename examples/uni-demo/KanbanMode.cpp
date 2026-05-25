#include "KanbanMode.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>

KanbanMode::KanbanMode(QHF::IEditorContext* context, QWidget* parent)
    : QHF::QHFModeBase(context, parent)
{
    buildBoard();
}

QString KanbanMode::modeName() const
{
    return QStringLiteral("KanbanBoard");
}

void KanbanMode::buildBoard()
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);

    m_splitter = new QSplitter(Qt::Horizontal, this);
    m_splitter->setHandleWidth(3);

    m_splitter->addWidget(createColumn(tr("Todo"),
        {tr("Prototype"), tr("Frontend Dev"), tr("Docs Update")}));

    m_splitter->addWidget(createColumn(tr("In Progress"),
        {tr("Core Refactor"), tr("Performance"), tr("Unit Tests")}));

    m_splitter->addWidget(createColumn(tr("Done"),
        {tr("Requirements"), tr("Tech Stack"), tr("Setup")}));

    m_splitter->setSizes({1, 1, 1});
    layout->addWidget(m_splitter);
}

QWidget* KanbanMode::createColumn(const QString& title, const QStringList& tasks)
{
    auto* column = new QFrame(this);
    column->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    auto* colLayout = new QVBoxLayout(column);
    colLayout->setContentsMargins(6, 6, 6, 6);

    // Header
    auto* header = new QLabel(title, column);
    header->setStyleSheet(QStringLiteral(
        "QLabel { font-size: 14px; font-weight: bold; padding: 6px; "
        "background: #e8e8e8; border-radius: 4px; }"));
    colLayout->addWidget(header);

    // Task cards
    for (const auto& task : tasks) {
        auto* card = new QFrame(column);
        card->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
        card->setStyleSheet(QStringLiteral(
            "QFrame { background: white; border: 1px solid #d0d0d0; "
            "border-radius: 4px; padding: 0px; margin: 2px 0px; }"));
        auto* cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(8, 6, 8, 6);

        auto* taskLabel = new QLabel(task, card);
        taskLabel->setWordWrap(true);
        taskLabel->setStyleSheet(QStringLiteral("border: none;"));
        cardLayout->addWidget(taskLabel);

        // Show a tag depending on the column
        auto* tag = new QLabel(card);
        if (title.contains(tr("Todo"))) {
            tag->setText(tr("Planning"));
            tag->setStyleSheet(QStringLiteral("color: #888; border: none; font-size: 11px;"));
        } else if (title.contains(tr("In Progress"))) {
            tag->setText(tr("Active"));
            tag->setStyleSheet(QStringLiteral("color: #e67e22; border: none; font-size: 11px;"));
        } else {
            tag->setText(tr("Done"));
            tag->setStyleSheet(QStringLiteral("color: #27ae60; border: none; font-size: 11px;"));
        }
        cardLayout->addWidget(tag);

        colLayout->addWidget(card);
    }

    colLayout->addStretch();
    return column;
}

void KanbanMode::handleNotifyImpl(const QHF::QHFNotifier& notifier)
{
    Q_UNUSED(notifier);
}
