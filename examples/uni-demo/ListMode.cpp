#include "ListMode.h"
#include <QVBoxLayout>
#include <QHeaderView>

ListMode::ListMode(QHF::IEditorContext* context, QWidget* parent)
    : QHF::QHFModeBase(context, parent)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_table = new QTableWidget(this);
    m_table->setColumnCount(4);
    m_table->setHorizontalHeaderLabels({tr("Project Name"), tr("Tasks"), tr("Progress"), tr("Status")});
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_table->setAlternatingRowColors(true);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);

    layout->addWidget(m_table);

    populateMockData();
}

QString ListMode::modeName() const
{
    return QStringLiteral("ProjectList");
}

void ListMode::populateMockData()
{
    struct ProjectRow {
        QString name;
        int     taskCount;
        int     progress;
        QString status;
    };

    const QList<ProjectRow> data = {
        {tr("QHiveFrame Dev"),     8, 65, tr("In Progress")},
        {tr("Client Demo"),         4, 10, tr("Planning")},
        {tr("Performance Opt"),         6, 90, tr("Wrapping Up")},
        {tr("Internal Tools"),         12, 40, tr("In Progress")},
        {tr("OSS Community"),         3, 100, tr("Done")},
    };

    m_table->setRowCount(data.size());
    for (int i = 0; i < data.size(); ++i) {
        m_table->setItem(i, 0, new QTableWidgetItem(data[i].name));
        m_table->setItem(i, 1, new QTableWidgetItem(QString::number(data[i].taskCount)));
        m_table->setItem(i, 2, new QTableWidgetItem(QStringLiteral("%1%").arg(data[i].progress)));
        m_table->setItem(i, 3, new QTableWidgetItem(data[i].status));
    }
}

void ListMode::handleNotifyImpl(const QHF::QHFNotifier& notifier)
{
    Q_UNUSED(notifier);
}
