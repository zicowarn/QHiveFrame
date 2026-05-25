#include "QHFModeBase.h"

#include "QHFEditorBase.h"
#include "QHFGuiNotifierCenter.h"
#include "QHFThemeManager.h"

#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>

namespace QHF {

    QHFModeBase::QHFModeBase(IEditorContext* context, QWidget* parent)
        : QWidget(parent), m_context(context) {
        m_parent = parent;
        // Set object name
        setObjectName("QHFModeBase");
        // setbackground为透明
        setAttribute(Qt::WA_StyledBackground, true);
        // Set stylesheet
        initTheme(QHFThemeManager::instance(), &QHFThemeManager::themeChanged);
        // set自己 缩放策略
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

    void QHFModeBase::initialize() {
        qDebug() << "Initializing mode:" << modeName();
        initializeImpl();
        applyTheme();
        // event处理
        connect(&QHFNotifierCenter::instance(), &QHFNotifierCenter::notify, this,
                &QHFModeBase::handleNotify);
    }

    void QHFModeBase::cleanup() {}

    void QHFModeBase::update() { emit contentChanged(); }

    QWidget* QHFModeBase::createHeaderContent() { return nullptr; }

    QWidget* QHFModeBase::createContent() { return static_cast<QWidget*>(this); };

    void QHFModeBase::connectSignals() {}

    void QHFModeBase::disconnectSignals() {}

    void QHFModeBase::paintEvent(QPaintEvent* event) {
        // 只调用base class  paintEvent, 让style表生效
        QStyleOption opt;
        opt.initFrom(this);
        QPainter painter(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
        QWidget::paintEvent(event);
    }

    void QHFModeBase::applyTheme() {
        // subclassstyle (currentstyle)
        QString currenStyleSheet = styleSheet();
        // Subclass may override this function以seteditorstyle表
        const auto& theme = QHFThemeManager::instance().theme();
        qDebug() << "Change style: " << objectName();
        QString editorStyle = QString(" #%1 {"
                                      "  background-color: %2;"
                                      "  color: %3;"
                                      "  border-bottom-left-radius: %4px;"
                                      "  border-bottom-right-radius: %4px;"
                                      "  border: none; }")
                                  .arg(objectName())
                                  .arg(theme.innerPanelColor.name())
                                  .arg(theme.textColor.name())
                                  .arg(theme.borderRadius);
        qDebug() << "Editor style: " << editorStyle;
        // 合并style表
        currenStyleSheet.append(editorStyle);
        // Set stylesheet
        setStyleSheet(currenStyleSheet);
    }

    void QHFModeBase::initializeImpl() {}

    void QHFModeBase::handleNotify(const QHFNotifier& notifier) {
        // Subclass may override this function以处理notification
        handleNotifyImpl(notifier);
    }

    void QHFModeBase::publishModeActive(const QString& reason, bool immediate) {
        QHFActivationEvent event(QHFActivationEvent::Type::MODE_ACTIVE, this, reason, immediate);
        event.context["modeName"] = modeName();
        QHFModeBase* mode = this; // 避免被转换为QWidget* 导致类型错误
        publishActivation(mode, event);
    }

} // namespace QHF
