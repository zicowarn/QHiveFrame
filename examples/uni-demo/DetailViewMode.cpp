#include "DetailViewMode.h"
#include "QHFCollapsibleSection.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QGroupBox>

DetailViewMode::DetailViewMode(QHF::IEditorContext* context, QWidget* parent)
    : QHF::QHFModeBase(context, parent)
{
    buildUI();
}

QString DetailViewMode::modeName() const
{
    return QStringLiteral("TaskView");
}

void DetailViewMode::buildUI()
{
    auto* outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(8, 8, 8, 8);

    // Title
    auto* titleLabel = new QLabel(tr("<h2>Core Refactor</h2>"), this);
    titleLabel->setAlignment(Qt::AlignLeft);
    outerLayout->addWidget(titleLabel);

    // Meta info section — demonstrates QHFCollapsibleSection
    auto* metaSection = new QHF::QHFCollapsibleSection(tr("Basic Info"), 0, this);
    auto* metaForm    = new QFormLayout();
    metaForm->addRow(tr("Project:"), new QLabel(tr("QHiveFrame Dev"), this));
    metaForm->addRow(tr("Assignee:"),   new QLabel(tr("Alice"), this));
    metaForm->addRow(tr("Priority:"),   new QLabel(tr("High"), this));
    metaForm->addRow(tr("Status:"),     new QLabel(tr("In Progress"), this));
    metaForm->addRow(tr("Due Date:"), new QLabel(tr("2026-06-30"), this));
    metaForm->addRow(tr("Progress:"),     new QLabel(tr("65%"), this));
    metaSection->setContentLayout(*metaForm);
    outerLayout->addWidget(metaSection);

    // Description section
    auto* descSection = new QHF::QHFCollapsibleSection(tr("Description"), 0, this);
    m_detailView = new QTextEdit(this);
    m_detailView->setReadOnly(true);
    m_detailView->setHtml(
        tr("<p>Refactor core module, optimize Workspace/Editor/Mode architecture:</p>"
           "<ul>"
           "<li>Extract common interfaces to IEditorContext / IModeContext</li>"
           "<li>Incremental LayoutNode updates</li>"
           "<li>Multi-window state sync support</li>"
           "<li>Reduce unnecessary full rebuilds</li>"
           "</ul>"
           "<p><b>Expected:</b> 30% less memory, faster switching</p>"));
    auto* descLayout = new QVBoxLayout();
    descLayout->addWidget(m_detailView);
    descSection->setContentLayout(*descLayout);
    outerLayout->addWidget(descSection);

    // Sub-tasks section
    auto* subSection = new QHF::QHFCollapsibleSection(tr("Subtasks (3)"), 0, this);
    auto* subForm    = new QFormLayout();
    subForm->addRow(tr("Interface Definition"), new QLabel(tr("Done"), this));
    subForm->addRow(tr("Incremental Update"), new QLabel(tr("In Progress"), this));
    subForm->addRow(tr("Performance Test"), new QLabel(tr("Todo"), this));
    subSection->setContentLayout(*subForm);
    outerLayout->addWidget(subSection);

    outerLayout->addStretch();
}

void DetailViewMode::handleNotifyImpl(const QHF::QHFNotifier& notifier)
{
    Q_UNUSED(notifier);
}
