#include "TreeMode.h"
#include <QVBoxLayout>
#include <QHeaderView>

TreeMode::TreeMode(QHF::IEditorContext* context, QWidget* parent)
    : QHF::QHFModeBase(context, parent)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_tree = new QTreeWidget(this);
    m_tree->setHeaderLabels({tr("Name"), tr("Type"), tr("Status")});
    m_tree->header()->setStretchLastSection(false);
    m_tree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_tree->setAlternatingRowColors(true);
    m_tree->setAnimated(true);

    setupTree();
    populateMockData();

    layout->addWidget(m_tree);
}

QString TreeMode::modeName() const
{
    return QStringLiteral("ProjectTree");
}

void TreeMode::setupTree()
{
    m_tree->setColumnCount(3);
    m_tree->setHeaderLabels({tr("Name"), tr("Type"), tr("Status")});
    m_tree->setRootIsDecorated(true);
}

void TreeMode::populateMockData()
{
    // Project 1
    auto* proj1 = new QTreeWidgetItem(m_tree);
    proj1->setText(0, tr("QHiveFrame Dev"));
    proj1->setText(1, tr("Project"));
    proj1->setText(2, tr("In Progress"));
    proj1->setIcon(0, qApp->style()->standardIcon(QStyle::SP_DirIcon));

    auto* task11 = new QTreeWidgetItem(proj1);
    task11->setText(0, tr("Core Refactor"));
    task11->setText(1, tr("Task"));
    task11->setText(2, tr("In Progress"));

    auto* task12 = new QTreeWidgetItem(proj1);
    task12->setText(0, tr("Docs Update"));
    task12->setText(1, tr("Task"));
    task12->setText(2, tr("Todo"));

    auto* task13 = new QTreeWidgetItem(proj1);
    task13->setText(0, tr("Unit Tests"));
    task13->setText(1, tr("Task"));
    task13->setText(2, tr("Done"));

    // Project 2
    auto* proj2 = new QTreeWidgetItem(m_tree);
    proj2->setText(0, tr("Client Demo"));
    proj2->setText(1, tr("Project"));
    proj2->setText(2, tr("Planning"));
    proj2->setIcon(0, qApp->style()->standardIcon(QStyle::SP_DirIcon));

    auto* task21 = new QTreeWidgetItem(proj2);
    task21->setText(0, tr("Prototype"));
    task21->setText(1, tr("Task"));
    task21->setText(2, tr("Todo"));

    auto* task22 = new QTreeWidgetItem(proj2);
    task22->setText(0, tr("Frontend Dev"));
    task22->setText(1, tr("Task"));
    task22->setText(2, tr("Todo"));

    m_tree->expandAll();
}

void TreeMode::handleNotifyImpl(const QHF::QHFNotifier& notifier)
{
    Q_UNUSED(notifier);
}
