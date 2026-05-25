#include "QHFPreferencePanelBase.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>

namespace QHF {

    /// --- QHFPreferencePanelBase implementation ---
    QList<SettingMeta> QHFPreferencePanelBase::registeredSettings() { return {}; }

    QHFPreferencePanelBase::QHFPreferencePanelBase(QWidget* parent) : QWidget(parent) {
        // Initializes变量
        m_layout = nullptr;
        // Set object name
        setObjectName("QHFPreferencePanelBase");
        // setstyle, 圆角border
        setAttribute(Qt::WA_StyledBackground, true);
        // InitializesTheme manager
        initTheme(QHFThemeManager::instance(), &QHFThemeManager::themeChanged);
    }

    void QHFPreferencePanelBase::initialize() {
        //
        constexpr int defaultMargin = 10;
        constexpr int defaultSpacing = 5;
        // InitializesLayout
        m_layout = new QVBoxLayout(this);
        m_layout->setContentsMargins(defaultMargin, defaultMargin, defaultMargin, defaultMargin);
        m_layout->setSpacing(defaultSpacing);
        setLayout(m_layout);
        // subclassimplementationInitializes
        initializeImpl();
        // 接受theme
        applyTheme();
    }

    void QHFPreferencePanelBase::paintEvent(QPaintEvent* event) {
        // 只调用base class  paintEvent, 让style表生效
        QStyleOption opt;
        opt.initFrom(this);
        QPainter painter(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
        QWidget::paintEvent(event);
    }

    void QHFPreferencePanelBase::applyTheme() {
        // subclassstyle (currentstyle)
        QString currenStyleSheet = styleSheet();
        // base classstyle
        const auto& theme = QHFThemeManager::instance().theme();
        QString panelStyle = QString("QWidget#QHFPreferencePanelBase {"
                                     "    background-color: %1;"
                                     "    border-radius: %2;"
                                     "}")
                                 .arg(theme.normalPanelColor.name())
                                 .arg(theme.borderRadius);
        // 合并style
        currenStyleSheet.append(panelStyle);
        // setstyle
        setStyleSheet(currenStyleSheet);
    }

} // namespace QHF
