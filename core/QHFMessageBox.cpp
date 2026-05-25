#include "QHFMessageBox.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStyle>
#include <QVBoxLayout>

namespace QHF {

    // staticinterfaceimplementation
    QHFMessageBox::QHFMsgBoxButton QHFMessageBox::information(QWidget* parent, const QString& title,
                                                        const QString& text, QHFMsgBoxButtons buttons,
                                                        QHFMsgBoxButton defaultButton) {
        QHFMessageBox msgBox(title, INFO, text, buttons, parent);
        if (defaultButton != NONE) {
            msgBox.setDefaultButton(defaultButton);
        }
        msgBox.exec();
        return msgBox.m_clickedButton;
    }

    QHFMessageBox::QHFMsgBoxButton QHFMessageBox::warning(QWidget* parent, const QString& title,
                                                    const QString& text, QHFMsgBoxButtons buttons,
                                                    QHFMsgBoxButton defaultButton) {
        QHFMessageBox msgBox(title, WARN, text, buttons, parent);
        if (defaultButton != NONE) {
            msgBox.setDefaultButton(defaultButton);
        }
        msgBox.exec();
        return msgBox.m_clickedButton;
    }

    QHFMessageBox::QHFMsgBoxButton QHFMessageBox::critical(QWidget* parent, const QString& title,
                                                     const QString& text, QHFMsgBoxButtons buttons,
                                                     QHFMsgBoxButton defaultButton) {
        QHFMessageBox msgBox(title, CRIT, text, buttons, parent);
        if (defaultButton != NONE) {
            msgBox.setDefaultButton(defaultButton);
        }
        msgBox.exec();
        return msgBox.m_clickedButton;
    }

    QHFMessageBox::QHFMsgBoxButton QHFMessageBox::question(QWidget* parent, const QString& title,
                                                     const QString& text, QHFMsgBoxButtons buttons,
                                                     QHFMsgBoxButton defaultButton) {
        QHFMessageBox msgBox(title, QUES, text, buttons, parent);
        if (defaultButton != NONE) {
            msgBox.setDefaultButton(defaultButton);
        }
        msgBox.exec();
        return msgBox.m_clickedButton;
    }

    /// --- QHFMessageBox implementation ---
    QHFMessageBox::QHFMessageBox(QString title, QHFMsgBoxIcon icon, QString text, QHFMsgBoxButtons buttons,
                             QWidget* parent)
        : QDialog(parent), m_msgIcon(icon), m_titleLabel(std::move(title)),
          m_textLabel(std::move(text)) {
        // Initializes按钮state
        m_clickedButton = NONE;
        m_isDragging = false;
        // Set object name
        setObjectName("QHFMessageBox");
        // setFlag,  Dialog window, 子window, 无边框, 无title
        setWindowFlags(Qt::Dialog | Qt::Window | Qt::FramelessWindowHint);
        // 添加按钮
        if (buttons != NONE) {
            addButton(buttons);
        }
        // Initializes
        initialize();
    }

    void QHFMessageBox::addButton(QHFMsgBoxButtons buttons) {
        if (buttons.testFlag(OK)) {
            addButton(buttonText(OK), OK);
        }
        if (buttons.testFlag(CANCEL)) {
            addButton(buttonText(CANCEL), CANCEL);
        }
        if (buttons.testFlag(YES)) {
            addButton(buttonText(YES), YES);
        }
        if (buttons.testFlag(NO)) {
            addButton(buttonText(NO), NO);
        }
        if (buttons.testFlag(RETRY)) {
            addButton(buttonText(RETRY), RETRY);
        }
        if (buttons.testFlag(IGNOREBTN)) {
            addButton(buttonText(IGNOREBTN), IGNOREBTN);
        }
    }

    void QHFMessageBox::addButton(const QString& text, QHFMsgBoxButton button) {
        auto* btn = new QPushButton(text, this);
        m_buttons[button] = btn;
        connect(btn, &QPushButton::clicked, this, &QHFMessageBox::onButtonClicked);
    }

    void QHFMessageBox::setDefaultButton(QHFMsgBoxButton button) {
        auto iter = m_buttons.find(button);
        if (iter != m_buttons.end()) {
            iter.value()->setDefault(true);
            iter.value()->setFocus();
        }
    }

    QHFMessageBox::QHFMsgBoxButton QHFMessageBox::result() const { return m_clickedButton; }

    void QHFMessageBox::setButtonText(QHFMessageBox::QHFMsgBoxButton button, const QString& text) {
        auto iter = m_buttons.find(button);
        if (iter != m_buttons.end()) {
            iter.value()->setText(text);
        }
    }

    void QHFMessageBox::showEvent(QShowEvent* event) {
        // Center on screen
        if (parentWidget() != nullptr) {
            QSize parentSize = parentWidget()->size();
            QPoint parentPos = parentWidget()->mapToGlobal(QPoint(0, 0));
            move(parentPos.x() + ((parentSize.width() - width()) / 2),
                 parentPos.y() + ((parentSize.height() - height()) / 2));
        }
        // 调用base class方法
        QDialog::showEvent(event);
    }

    void QHFMessageBox::mousePressEvent(QMouseEvent* event) {
        if (event->button() == Qt::LeftButton) {
            for (const QString& name : {"QHFMessageBoxTextLabel", "QHFMessageBoxTitleLabel"}) {
                QWidget* widget = findChild<QLabel*>(name);
                if (widget != nullptr &&
                    widget->rect().contains(widget->mapFromGlobal(event->globalPos()))) {
                    m_isDragging = true;
                    m_dragStartPosition = event->globalPos();
                    event->accept();
                }
            }
        }
        QDialog::mousePressEvent(event);
    }

    void QHFMessageBox::mouseMoveEvent(QMouseEvent* event) {
        if (m_isDragging && (event->buttons() & Qt::LeftButton) != 0) {
            QPoint delta = event->globalPos() - m_dragStartPosition;
            move(pos() + delta);
            m_dragStartPosition = event->globalPos();
            event->accept();
        }
        QDialog::mouseMoveEvent(event);
    }

    void QHFMessageBox::mouseReleaseEvent(QMouseEvent* event) {
        if (event->button() == Qt::LeftButton) {
            m_isDragging = false;
            event->accept();
        }
        QDialog::mouseReleaseEvent(event);
    }

    void QHFMessageBox::initialize() {
        constexpr QSize defaultSize(400, 120);
        constexpr int iconStretch = 1;
        constexpr int textStretch = 9;
        constexpr int mainMargin = 10;
        constexpr int mainSpacing = 10;
        constexpr int contentMargin = 10;
        constexpr int contentSpacing = 10;
        constexpr int textMargin = 0;
        constexpr int textSpacing = 10;
        constexpr int buttonLeftSpacing = 20;
        constexpr int buttonMinWidth = 80;
        constexpr int buttonMargin = 0;
        constexpr int buttonSpacing = 10;
        // 主Layout
        auto* mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(mainMargin, mainMargin, mainMargin, mainMargin);
        mainLayout->setSpacing(mainSpacing);

        auto* contentLayout = new QHBoxLayout();
        contentLayout->setContentsMargins(contentMargin, contentMargin, contentMargin,
                                          contentMargin);
        contentLayout->setSpacing(contentSpacing);

        // 加入图标
        auto* iconLabel = createIcon();
        if (iconLabel != nullptr) {
            contentLayout->addWidget(iconLabel, iconStretch);
        }

        // 加入empty格
        contentLayout->addSpacing(contentSpacing);
        //
        auto* textLayout = new QVBoxLayout();
        textLayout->setContentsMargins(textMargin, textMargin, textMargin, textMargin);
        textLayout->setSpacing(textSpacing);
        // 加入title
        auto* titleLabel = new QLabel(m_titleLabel, this);
        titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        titleLabel->setObjectName("QHFMessageBoxTitleLabel");
        textLayout->addWidget(titleLabel);
        // 加入文本
        auto* textLabel = new QLabel(m_textLabel, this);
        textLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        textLabel->setObjectName("QHFMessageBoxTextLabel");
        textLabel->setWordWrap(true);
        textLayout->addWidget(textLabel);
        // 加入Layout
        contentLayout->addLayout(textLayout, textStretch);

        // 加入主Layout
        mainLayout->addLayout(contentLayout);

        // 加入按钮Layout
        auto* buttonLayout = new QHBoxLayout();
        buttonLayout->setContentsMargins(buttonMargin, buttonMargin, buttonMargin, buttonMargin);
        buttonLayout->setSpacing(buttonSpacing);
        // 按钮left侧empty格
        buttonLayout->addStretch(1);
        // 按钮按order添加
        for (auto iter = m_buttons.constBegin(); iter != m_buttons.constEnd(); ++iter) {
            auto* btn = iter.value();
            // btn->setMinimumWidth(buttonMinWidth);
            btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            buttonLayout->addWidget(btn, 1);
        }
        // 按钮Layout加入主Layout
        mainLayout->addLayout(buttonLayout);

        // 加入empty格
        mainLayout->addStretch();

        // Set layout
        setLayout(mainLayout);
        // Applies theme
        applyTheme();
        //
        setMinimumSize(defaultSize);
        //
        resize(defaultSize);
    }

    void QHFMessageBox::applyTheme() {

        // 应用 Blender 风格style表
        QString mainStyle = QString(" QDialog#QHFMessageBox { background-color: #2B2B2B; color: "
                                    "#D4D4D4; border: 1px solid #555555 } ");

        QString titleStyle = QString(" QLabel#QHFMessageBoxTitleLabel { font-size: 14pt; "
                                     "font-weight: bold; color: #F5F5F5; } ");

        QString textStyle = QString(" QLabel#QHFMessageBoxTextLabel { font-size: 12pt; } ");

        QString buttonStyle =
            QString(" QPushButton { background-color: #3C3C3C; border: 1px solid #555555; "
                    "border-radius: 4px; padding: 5px 10px; color: #D4D4D4; } "
                    " QPushButton:hover { background-color: #4A4A4A; } "
                    " QPushButton:pressed { background-color: #555555; } ");

        setStyleSheet(mainStyle + titleStyle + textStyle + buttonStyle);
    }

    QWidget* QHFMessageBox::createIcon() const {
        constexpr QSize iconSize(32, 32);
        QIcon qicon;
        switch (m_msgIcon) {
        case INFO:
            qicon = style()->standardIcon(QStyle::SP_MessageBoxInformation);
            break;
        case WARN:
            qicon = style()->standardIcon(QStyle::SP_MessageBoxWarning);
            break;
        case CRIT:
            qicon = style()->standardIcon(QStyle::SP_MessageBoxCritical);
            break;
        case QUES:
            qicon = style()->standardIcon(QStyle::SP_MessageBoxQuestion);
            break;
        default:
            return nullptr;
        }
        auto* label = new QLabel();
        label->setPixmap(qicon.pixmap(iconSize));
        return label;
    }

    QString QHFMessageBox::buttonText(QHFMsgBoxButton button) {
        switch (button) {
        case OK:
            return tr("OK");
        case CANCEL:
            return tr("CANCEL");
        case YES:
            return tr("YES");
        case NO:
            return tr("NO");
        case RETRY:
            return tr("RETRY");
        case IGNOREBTN:
            return tr("IGNOREBTN");
        default:
            return {};
        }
    }

    void QHFMessageBox::onButtonClicked() {
        auto* btn = qobject_cast<QPushButton*>(sender());
        for (auto iter = m_buttons.constBegin(); iter != m_buttons.constEnd(); ++iter) {
            if (iter.value() == btn) {
                m_clickedButton = iter.key();
                accept();
                return;
            }
        }
    }

} // namespace QHF
