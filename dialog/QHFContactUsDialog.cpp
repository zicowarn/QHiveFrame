#include "QHFContactUsDialog.h"

#include "QHFThemeManager.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QSize>
#include <QStyle>
#include <QStyleOption>
#include <QVBoxLayout>

namespace QHF {
    /// --- QHFContactUsDialog ---
    QHFContactUsDialog::QHFContactUsDialog(QWidget* parent) : QDialog(parent) {
        // setstyle, 圆角border
        setObjectName("QHFContactUsDialog");
        // setbackground为透明
        setAttribute(Qt::WA_StyledBackground, true);
        // InitializesTheme manager
        initTheme(QHFThemeManager::instance(), &QHFThemeManager::themeChanged);
        // Initializes
        initialize();
        // Applies theme
        applyTheme();
    }

    void QHFContactUsDialog::onCloseButtonClicked() {
        close(); // 关闭对话框
    }

    void QHFContactUsDialog::paintEvent(QPaintEvent* event) {
        QPainter painter(this);
        QStyleOption opt;
        opt.init(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    }

    void QHFContactUsDialog::applyTheme() {
        //
        const auto& theme = QHFThemeManager::instance().theme();
        // settitlestyle
        QString buttonStyle = QString("QPushButton#QHFContactUsDialogCloseButton {"
                                      "  background-color: %1;"
                                      "  color: %2;"
                                      "  padding: 5px;"
                                      "  border: none;"
                                      "  border-radius: %5px;"
                                      " }"
                                      "QPushButton#QHFContactUsDialogCloseButton:hover {"
                                      "  background-color: %3;"
                                      " }"
                                      "QPushButton#QHFContactUsDialogCloseButton:pressed {"
                                      "  background-color: %4;"
                                      " }")
                                  .arg(theme.btnBgColor.name())
                                  .arg(theme.btnFgColor.name())
                                  .arg(theme.btnBgColor.lighter(120).name())
                                  .arg(theme.btnBgColor.darker(120).name())
                                  .arg(theme.borderRadius);
        // setstyle
        setStyleSheet(buttonStyle);
    }

    void QHFContactUsDialog::initialize() {
        //
        constexpr QSize defaultSize(420, 320);
        //
        auto* helpLabel = new QLabel(this);

        // HTML content, contain工作event、联系电话、官方网站、支持邮箱
        QString helpText =
            "<html>"
            "<head>"
            "<style>"
            "body {"
            "    font-family: Arial, sans-serif;"
            "    text-align: center;"
            "    padding: 10px;"
            "}"
            "h2 {"
            "    color:rgb(245, 245, 245);"
            "}"
            "p {"
            "    margin: 10px 0;"
            "}"
            "a {"
            "    color:rgb(206, 210, 216);"
            "    text-decoration: none;"
            "}"
            "a:hover {"
            "    text-decoration: underline;"
            "}"
            "</style>"
            "</head>"
            "<body>"
            "<h2>获取帮助</h2>"
            "<p>工作时间: 9:00 - 18:00</p>"
            "<p>联系电话: <b>+86 123 4567 890</b></p>"
            "<p>官方网站: <a href='https://www.example.com'>www.example.com</a></p>"
            "<p>支持邮箱: <a href='mailto:support@example.com'>support@example.com</a></p>"
            "</body>"
            "</html>";

        // 将HTML文本set到QLabel
        helpLabel->setText(helpText);

        // auto调整大小
        helpLabel->setWordWrap(true);

        // Set layout
        QVBoxLayout* layout = new QVBoxLayout;
        layout->addWidget(helpLabel);

        // setclose按钮
        // sethorizontalLayout, About按钮在right侧
        auto* buttonLayout = new QHBoxLayout();
        buttonLayout->addStretch(); // 添加弹性空间, 使按钮靠右
        auto* closeButton = new QPushButton(tr("Close"), this);
        closeButton->setObjectName("QHFContactUsDialogCloseButton");
        buttonLayout->addWidget(closeButton);
        layout->addLayout(buttonLayout);

        // 应用Layout
        setLayout(layout);

        // set尺寸
        setMinimumSize(defaultSize);
        resize(defaultSize);

        // connection按钮点击signal到Slot function
        connect(closeButton, &QPushButton::clicked, this, &QHFContactUsDialog::onCloseButtonClicked);
    }

} // namespace QHF
