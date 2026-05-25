#include "TaskEditMode.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>

TaskEditMode::TaskEditMode(QHF::IEditorContext* context, QWidget* parent)
    : QHF::QHFModeBase(context, parent)
{
    buildForm();
}

QString TaskEditMode::modeName() const
{
    return QStringLiteral("TaskEdit");
}

void TaskEditMode::buildForm()
{
    auto* outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(8, 8, 8, 8);

    auto* titleLabel = new QLabel(tr("<h3>编辑任务</h3>"), this);
    outerLayout->addWidget(titleLabel);

    auto* formLayout = new QFormLayout();
    formLayout->setSpacing(10);

    // Title
    m_titleEdit = new QLineEdit(this);
    m_titleEdit->setText(tr("核心框架重构"));
    m_titleEdit->setPlaceholderText(tr("请输入任务标题"));
    formLayout->addRow(tr("任务标题:"), m_titleEdit);

    // Priority
    m_priorityCbo = new QComboBox(this);
    m_priorityCbo->addItems({tr("低"), tr("中"), tr("高"), tr("紧急")});
    m_priorityCbo->setCurrentIndex(2); // 高
    formLayout->addRow(tr("优先级:"), m_priorityCbo);

    // Status
    m_statusCbo = new QComboBox(this);
    m_statusCbo->addItems({tr("待开始"), tr("进行中"), tr("已完成"), tr("已取消")});
    m_statusCbo->setCurrentIndex(1); // 进行中
    formLayout->addRow(tr("状态:"), m_statusCbo);

    // Progress (QHFCustomUnitSpinBox substitute — demonstrates custom widget pattern)
    m_progressSpin = new QSpinBox(this);
    m_progressSpin->setRange(0, 100);
    m_progressSpin->setValue(65);
    m_progressSpin->setSuffix(tr(" %"));
    m_progressSpin->setToolTip(tr("任务完成进度 (0-100%)"));
    formLayout->addRow(tr("进度:"), m_progressSpin);

    // Deadline
    m_deadlineEdit = new QDateEdit(this);
    m_deadlineEdit->setDate(QDate(2026, 6, 30));
    m_deadlineEdit->setCalendarPopup(true);
    m_deadlineEdit->setDisplayFormat("yyyy-MM-dd");
    formLayout->addRow(tr("截止日期:"), m_deadlineEdit);

    outerLayout->addLayout(formLayout);

    // Description
    outerLayout->addWidget(new QLabel(tr("描述:"), this));
    m_descEdit = new QTextEdit(this);
    m_descEdit->setPlaceholderText(tr("请输入任务描述..."));
    m_descEdit->setMinimumHeight(120);
    m_descEdit->setPlainText(
        tr("重构 QHiveFrame 核心模块，优化 Workspace/Editor/Mode 架构。\n"
           "\n"
           "- 提取公共接口\n"
           "- 实现增量更新\n"
           "- 多窗口状态同步\n"
           "- 性能优化"));
    outerLayout->addWidget(m_descEdit);

    // Save button
    auto* saveBtn = new QPushButton(tr("💾 保存修改"), this);
    saveBtn->setMinimumHeight(36);
    QObject::connect(saveBtn, &QPushButton::clicked, this, [this]() {
        QMessageBox::information(this, tr("保存成功"),
                                 tr("任务 \"%1\" 已保存。\n\n优先级: %2\n状态: %3\n进度: %4%")
                                     .arg(m_titleEdit->text(),
                                          m_priorityCbo->currentText(),
                                          m_statusCbo->currentText(),
                                          QString::number(m_progressSpin->value())));
    });
    outerLayout->addWidget(saveBtn);

    outerLayout->addStretch();
}

void TaskEditMode::handleNotifyImpl(const QHF::QHFNotifier& notifier)
{
    Q_UNUSED(notifier);
}
