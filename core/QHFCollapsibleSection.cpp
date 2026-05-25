#include "QHFCollapsibleSection.h"

#include "QHFThemeManager.h"

#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QStyleOption>

namespace QHF {
    /**
     * QHFCollapsibleSection implementation代码
     */
    QHFCollapsibleSection::QHFCollapsibleSection(QString title, int animationDuration, QWidget* parent)
        : QWidget(parent), m_animationDuration(animationDuration), m_title(std::move(title)) {
        // Initializes
        m_mainLayout = nullptr;
        m_toggleButton = nullptr;
        m_headerLine = nullptr;
        m_toggleAnimation = nullptr;
        m_contentArea = nullptr;
        m_collapsedHeight = 0;
        m_isExpanded = false;

        // setstyle, 圆角border
        setObjectName("QHFCollapsibleSection");
        // setbackground为透明
        setAttribute(Qt::WA_StyledBackground, true);
        // InitializesTheme manager
        initTheme(QHFThemeManager::instance(), &QHFThemeManager::themeChanged);
        // Initializes
        initialize();
        // 接受theme
        applyTheme();
    }

    void QHFCollapsibleSection::setContentLayout(QLayout& contentLayout) {
        delete m_contentArea->layout();
        m_contentArea->setLayout(&contentLayout);
        m_collapsedHeight = sizeHint().height() - m_contentArea->maximumHeight();
        updateHeights();
    }

    void QHFCollapsibleSection::setTitle(const QString& title) { m_toggleButton->setText(title); }

    void QHFCollapsibleSection::updateHeights() {
        constexpr int headerHeight = 24;
        int contentHeight = m_contentArea->layout()->sizeHint().height() + headerHeight;

        for (int i = 0; i < m_toggleAnimation->animationCount() - 1; ++i) {
            auto* SectionAnimation =
                static_cast<QPropertyAnimation*>(m_toggleAnimation->animationAt(i));
            SectionAnimation->setDuration(m_animationDuration);
            SectionAnimation->setStartValue(m_collapsedHeight);
            SectionAnimation->setEndValue(m_collapsedHeight + contentHeight);
        }

        auto* contentAnimation = static_cast<QPropertyAnimation*>(
            m_toggleAnimation->animationAt(m_toggleAnimation->animationCount() - 1));
        contentAnimation->setDuration(m_animationDuration);
        contentAnimation->setStartValue(0);
        contentAnimation->setEndValue(contentHeight);

        m_toggleAnimation->setDirection(m_isExpanded ? QAbstractAnimation::Forward
                                                     : QAbstractAnimation::Backward);
        m_toggleAnimation->start();
    }

    void QHFCollapsibleSection::toggle(bool collapsed) {
        m_toggleButton->setArrowType(collapsed ? Qt::ArrowType::DownArrow
                                               : Qt::ArrowType::RightArrow);
        m_toggleAnimation->setDirection(collapsed ? QAbstractAnimation::Forward
                                                  : QAbstractAnimation::Backward);
        m_toggleAnimation->start();
        this->m_isExpanded = collapsed;
    }

    void QHFCollapsibleSection::paintEvent(QPaintEvent* event) {
        // 只调用base class  paintEvent, 让style表生效
        QStyleOption opt;
        opt.initFrom(this);
        QPainter painter(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
        QWidget::paintEvent(event);
    }

    void QHFCollapsibleSection::applyTheme() {
        // Theme
        const auto& theme = QHFThemeManager::instance().theme();

        // toggle button
        QString toggleBtnStyle = QString(R"(
            QToolButton#QHFCollapsibleSectionToggle {
                border: none;
                font-size: 12px;
        })");

        QString contentAreaStyle =
            QString("QScrollArea#QHFCollapsibleSectionContent { background-color: %1; border: none; "
                    "border-radius: %2px; }")
                .arg(theme.normalPanelColor.name())
                .arg(theme.borderRadius);

        setStyleSheet(toggleBtnStyle + contentAreaStyle);
    }

    void QHFCollapsibleSection::initialize() {
        // 切换按钮
        m_toggleButton = new QToolButton(this);
        m_toggleButton->setObjectName("QHFCollapsibleSectionToggle");
        m_headerLine = new QFrame(this);
        m_toggleAnimation = new QParallelAnimationGroup(this);
        m_contentArea = new QScrollArea(this);
        m_mainLayout = new QGridLayout(this);

        // 切换按钮与title
        m_toggleButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        m_toggleButton->setArrowType(Qt::ArrowType::RightArrow);
        m_toggleButton->setText(m_title);
        m_toggleButton->setCheckable(true);
        m_toggleButton->setChecked(false);

        // titleafter面split线
        m_headerLine->setFrameShape(QFrame::HLine);
        m_headerLine->setFrameShadow(QFrame::Sunken);
        m_headerLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

        // contentarea
        m_contentArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        // setbackgroundcolor
        m_contentArea->setObjectName("QHFCollapsibleSectionContent");

        // start out collapsed
        m_contentArea->setMaximumHeight(0);
        m_contentArea->setMinimumHeight(0);

        // let the entire widget grow and shrink with its content
        m_toggleAnimation->addAnimation(new QPropertyAnimation(this, "maximumHeight"));
        m_toggleAnimation->addAnimation(new QPropertyAnimation(this, "minimumHeight"));
        m_toggleAnimation->addAnimation(new QPropertyAnimation(m_contentArea, "maximumHeight"));

        m_mainLayout->setVerticalSpacing(0);
        m_mainLayout->setContentsMargins(0, 0, 0, 0);

        int row = 0;
        /// NOLINTNEXTLINE(readability-magic-numbers)
        m_mainLayout->addWidget(m_toggleButton, row, 0, 1, 1, Qt::AlignLeft);
        /// NOLINTNEXTLINE(readability-magic-numbers)
        m_mainLayout->addWidget(m_headerLine, row++, 2, 1, 1);
        /// NOLINTNEXTLINE(readability-magic-numbers)
        m_mainLayout->addWidget(m_contentArea, row, 0, 1, 3);
        // Set layout
        setLayout(m_mainLayout);

        // signalconnection
        connect(m_toggleButton, &QToolButton::toggled, this, &QHFCollapsibleSection::toggle);
    }

} // namespace QHF
