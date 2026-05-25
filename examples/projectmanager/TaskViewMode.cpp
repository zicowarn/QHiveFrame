#include "TaskViewMode.h"
#include "QHFCollapsibleSection.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QGroupBox>

TaskViewMode::TaskViewMode(QHF::IEditorContext* context, QWidget* parent)
    : QHF::QHFModeBase(context, parent)
{
    buildUI();
}

QString TaskViewMode::modeName() const
{
    return QStringLiteral("TaskView");
}

void TaskViewMode::buildUI()
{
    auto* outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(8, 8, 8, 8);

    // Title
    auto* titleLabel = new QLabel(tr("<h2>核心框架重构</h2>"), this);
    titleLabel->setAlignment(Qt::AlignLeft);
    outerLayout->addWidget(titleLabel);

    // Meta info section — demonstrates QHFCollapsibleSection
    auto* metaSection = new QHF::QHFCollapsibleSection(tr("基本信息"), 0, this);
    auto* metaForm    = new QFormLayout();
    metaForm->addRow(tr("所属项目:"), new QLabel(tr("QHiveFrame 开发"), this));
    metaForm->addRow(tr("负责人:"),   new QLabel(tr("张三"), this));
    metaForm->addRow(tr("优先级:"),   new QLabel(tr("高"), this));
    metaForm->addRow(tr("状态:"),     new QLabel(tr("进行中"), this));
    metaForm->addRow(tr("截止日期:"), new QLabel(tr("2026-06-30"), this));
    metaForm->addRow(tr("进度:"),     new QLabel(tr("65%"), this));
    metaSection->setContentLayout(*metaForm);
    outerLayout->addWidget(metaSection);

    // Description section
    auto* descSection = new QHF::QHFCollapsibleSection(tr("任务描述"), 0, this);
    m_detailView = new QTextEdit(this);
    m_detailView->setReadOnly(true);
    m_detailView->setHtml(
        tr("<p>重构 QHiveFrame 核心模块，优化 Workspace/Editor/Mode 架构：</p>"
           "<ul>"
           "<li>提取公共接口到 IEditorContext / IModeContext</li>"
           "<li>实现 LayoutNode 增量更新机制</li>"
           "<li>添加多 Window 状态同步支持</li>"
           "<li>性能优化：减少不必要的 full rebuild</li>"
           "</ul>"
           "<p><b>预期收益：</b>降低内存占用 30%，提升切换响应速度</p>"));
    auto* descLayout = new QVBoxLayout();
    descLayout->addWidget(m_detailView);
    descSection->setContentLayout(*descLayout);
    outerLayout->addWidget(descSection);

    // Sub-tasks section
    auto* subSection = new QHF::QHFCollapsibleSection(tr("子任务 (3)"), 0, this);
    auto* subForm    = new QFormLayout();
    subForm->addRow(tr("✅ 接口定义"), new QLabel(tr("已完成"), this));
    subForm->addRow(tr("🔄 增量更新"), new QLabel(tr("进行中"), this));
    subForm->addRow(tr("⏳ 性能测试"), new QLabel(tr("待开始"), this));
    subSection->setContentLayout(*subForm);
    outerLayout->addWidget(subSection);

    outerLayout->addStretch();
}

void TaskViewMode::handleNotifyImpl(const QHF::QHFNotifier& notifier)
{
    Q_UNUSED(notifier);
}
