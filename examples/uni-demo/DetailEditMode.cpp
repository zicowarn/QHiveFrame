#include "DetailEditMode.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>

DetailEditMode::DetailEditMode(QHF::IEditorContext* context, QWidget* parent)
    : QHF::QHFModeBase(context, parent)
{
    buildForm();
}

QString DetailEditMode::modeName() const
{
    return QStringLiteral("TaskEdit");
}

void DetailEditMode::buildForm()
{
    auto* outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(8, 8, 8, 8);

    auto* titleLabel = new QLabel(tr("<h3>Edit Task</h3>"), this);
    outerLayout->addWidget(titleLabel);

    auto* formLayout = new QFormLayout();
    formLayout->setSpacing(10);

    // Title
    m_titleEdit = new QLineEdit(this);
    m_titleEdit->setText(tr("Core Refactor"));
    m_titleEdit->setPlaceholderText(tr("Enter task title..."));
    formLayout->addRow(tr("Title:"), m_titleEdit);

    // Priority
    m_priorityCbo = new QComboBox(this);
    m_priorityCbo->addItems({tr("Low"), tr("Medium"), tr("High"), tr("Urgent")});
    m_priorityCbo->setCurrentIndex(2); // High
    formLayout->addRow(tr("Priority:"), m_priorityCbo);

    // Status
    m_statusCbo = new QComboBox(this);
    m_statusCbo->addItems({tr("Todo"), tr("In Progress"), tr("Done"), tr("Cancelled")});
    m_statusCbo->setCurrentIndex(1); // In Progress
    formLayout->addRow(tr("Status:"), m_statusCbo);

    // Progress (QHFCustomUnitSpinBox substitute — demonstrates custom widget pattern)
    m_progressSpin = new QSpinBox(this);
    m_progressSpin->setRange(0, 100);
    m_progressSpin->setValue(65);
    m_progressSpin->setSuffix(tr(" %"));
    m_progressSpin->setToolTip(tr("Task progress (0-100%)"));
    formLayout->addRow(tr("Progress:"), m_progressSpin);

    // Deadline
    m_deadlineEdit = new QDateEdit(this);
    m_deadlineEdit->setDate(QDate(2026, 6, 30));
    m_deadlineEdit->setCalendarPopup(true);
    m_deadlineEdit->setDisplayFormat("yyyy-MM-dd");
    formLayout->addRow(tr("Due Date:"), m_deadlineEdit);

    outerLayout->addLayout(formLayout);

    // Description
    outerLayout->addWidget(new QLabel(tr("Description:"), this));
    m_descEdit = new QTextEdit(this);
    m_descEdit->setPlaceholderText(tr("Enter task description..."));
    m_descEdit->setMinimumHeight(120);
    m_descEdit->setPlainText(
        tr("Refactor QHiveFrame core modules.\n"
           "\n"
           "- Extract common interfaces\n"
           "- Incremental updates\n"
           "- Multi-window state sync\n"
           "- Performance optimization"));
    outerLayout->addWidget(m_descEdit);

    // Save button
    auto* saveBtn = new QPushButton(tr("Save"), this);
    saveBtn->setMinimumHeight(36);
    QObject::connect(saveBtn, &QPushButton::clicked, this, [this]() {
        QMessageBox::information(this, tr("Saved"),
                                 tr("Task \"%1\" saved.\n\nPriority: %2\nStatus: %3\nProgress: %4%")
                                     .arg(m_titleEdit->text(),
                                          m_priorityCbo->currentText(),
                                          m_statusCbo->currentText(),
                                          QString::number(m_progressSpin->value())));
    });
    outerLayout->addWidget(saveBtn);

    outerLayout->addStretch();
}

void DetailEditMode::handleNotifyImpl(const QHF::QHFNotifier& notifier)
{
    Q_UNUSED(notifier);
}
