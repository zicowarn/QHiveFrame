#include "ProjectTreeMode.h"
#include <QVBoxLayout>
#include <QHeaderView>

ProjectTreeMode::ProjectTreeMode(QHF::IEditorContext* context, QWidget* parent)
    : QHF::QHFModeBase(context, parent)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_tree = new QTreeWidget(this);
    m_tree->setHeaderLabels({tr("名称"), tr("类型"), tr("状态")});
    m_tree->header()->setStretchLastSection(false);
    m_tree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_tree->setAlternatingRowColors(true);
    m_tree->setAnimated(true);

    setupTree();
    populateMockData();

    layout->addWidget(m_tree);
}

QString ProjectTreeMode::modeName() const
{
    return QStringLiteral("ProjectTree");
}

void ProjectTreeMode::setupTree()
{
    m_tree->setColumnCount(3);
    m_tree->setHeaderLabels({tr("名称"), tr("类型"), tr("状态")});
    m_tree->setRootIsDecorated(true);
}

void ProjectTreeMode::populateMockData()
{
    // Project 1
    auto* proj1 = new QTreeWidgetItem(m_tree);
    proj1->setText(0, tr("QHiveFrame 开发"));
    proj1->setText(1, tr("项目"));
    proj1->setText(2, tr("进行中"));
    proj1->setIcon(0, qApp->style()->standardIcon(QStyle::SP_DirIcon));

    auto* task11 = new QTreeWidgetItem(proj1);
    task11->setText(0, tr("核心框架重构"));
    task11->setText(1, tr("任务"));
    task11->setText(2, tr("进行中"));

    auto* task12 = new QTreeWidgetItem(proj1);
    task12->setText(0, tr("文档更新"));
    task12->setText(1, tr("任务"));
    task12->setText(2, tr("待开始"));

    auto* task13 = new QTreeWidgetItem(proj1);
    task13->setText(0, tr("单元测试"));
    task13->setText(1, tr("任务"));
    task13->setText(2, tr("已完成"));

    // Project 2
    auto* proj2 = new QTreeWidgetItem(m_tree);
    proj2->setText(0, tr("客户Demo演示"));
    proj2->setText(1, tr("项目"));
    proj2->setText(2, tr("计划中"));
    proj2->setIcon(0, qApp->style()->standardIcon(QStyle::SP_DirIcon));

    auto* task21 = new QTreeWidgetItem(proj2);
    task21->setText(0, tr("原型设计"));
    task21->setText(1, tr("任务"));
    task21->setText(2, tr("待开始"));

    auto* task22 = new QTreeWidgetItem(proj2);
    task22->setText(0, tr("前端开发"));
    task22->setText(1, tr("任务"));
    task22->setText(2, tr("待开始"));

    m_tree->expandAll();
}

void ProjectTreeMode::handleNotifyImpl(const QHF::QHFNotifier& notifier)
{
    Q_UNUSED(notifier);
}
