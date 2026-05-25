#include "ProjectListMode.h"
#include <QVBoxLayout>
#include <QHeaderView>

ProjectListMode::ProjectListMode(QHF::IEditorContext* context, QWidget* parent)
    : QHF::QHFModeBase(context, parent)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_table = new QTableWidget(this);
    m_table->setColumnCount(4);
    m_table->setHorizontalHeaderLabels({tr("项目名称"), tr("任务数"), tr("进度"), tr("状态")});
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_table->setAlternatingRowColors(true);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);

    layout->addWidget(m_table);

    populateMockData();
}

QString ProjectListMode::modeName() const
{
    return QStringLiteral("ProjectList");
}

void ProjectListMode::populateMockData()
{
    struct ProjectRow {
        QString name;
        int     taskCount;
        int     progress;
        QString status;
    };

    const QList<ProjectRow> data = {
        {tr("QHiveFrame 开发"),     8, 65, tr("进行中")},
        {tr("客户Demo演示"),         4, 10, tr("计划中")},
        {tr("性能优化专项"),         6, 90, tr("收尾中")},
        {tr("企业内部工具"),         12, 40, tr("进行中")},
        {tr("开源社区运营"),         3, 100, tr("已完成")},
    };

    m_table->setRowCount(data.size());
    for (int i = 0; i < data.size(); ++i) {
        m_table->setItem(i, 0, new QTableWidgetItem(data[i].name));
        m_table->setItem(i, 1, new QTableWidgetItem(QString::number(data[i].taskCount)));
        m_table->setItem(i, 2, new QTableWidgetItem(QStringLiteral("%1%").arg(data[i].progress)));
        m_table->setItem(i, 3, new QTableWidgetItem(data[i].status));
    }
}

void ProjectListMode::handleNotifyImpl(const QHF::QHFNotifier& notifier)
{
    Q_UNUSED(notifier);
}
