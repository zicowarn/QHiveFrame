#include "QHFCustomActionWidgetBase.h"

#include <QDebug>
#include <QPainter>
#include <QPushButton>
#include <QStyleOption>

#include "QHFThemeManager.h"

namespace QHF {
    QHFCustomActionWidgetBase::QHFCustomActionWidgetBase(ActionType type, QWidget* parent)
        : QWidget(parent), m_type(type) {
        // Set object name
        setObjectName("QHFCustomActionWidgetBase");
        // set尺寸策略
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        // 移除边框和阴影
        setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
        // allow透明background
        setAttribute(Qt::WA_TranslucentBackground);
        // InitializesTheme manager
        initTheme(QHFThemeManager::instance(), &QHFThemeManager::themeChanged);
        // 启动鼠标跟踪
        setMouseTracking(true);
    }

    void QHFCustomActionWidgetBase::initialize() {
        // subclass完成Initializesmodebottom拉框
        initializeImpl();
        // Applies theme
        applyTheme();
    }

    void QHFCustomActionWidgetBase::paintEvent(QPaintEvent* event) {
        // 只调用base class  paintEvent, 让style表生效
        QStyleOption opt;
        opt.initFrom(this);
        QPainter painter(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
        QWidget::paintEvent(event);
    }
} // namespace QHF
