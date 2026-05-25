#include "QHFAboutUsDialog.h"

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
#include <QTextEdit>
#include <QVBoxLayout>

#ifndef PRODUCT_VERSION
#define PRODUCT_VERSION "x.x.x"
#endif

const QString COPYRIGHT_YEAR = "2023";

namespace QHF {
    /// --- QHFAboutUsDialog ---
    QHFAboutUsDialog::QHFAboutUsDialog(QWidget* parent) : QDialog(parent) {
        // setstyle, 圆角border
        setObjectName("QHFAboutUsDialog");
        // setbackground为透明
        setAttribute(Qt::WA_StyledBackground, true);
        // InitializesTheme manager
        initTheme(QHFThemeManager::instance(), &QHFThemeManager::themeChanged);
        // Initializes
        initialize();
        // Applies theme
        applyTheme();
    }

    void QHFAboutUsDialog::onCloseButtonClicked() {
        close(); // 关闭对话框
    }

    void QHFAboutUsDialog::paintEvent(QPaintEvent* event) {
        QPainter     painter(this);
        QStyleOption opt;
        opt.init(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    }

    void QHFAboutUsDialog::applyTheme() {
        //
        const auto& theme = QHFThemeManager::instance().theme();
        //
        QString titleStyle =
            QString("QLabel#QHFAboutUsDialogMainTitleLabel {"
                    "  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "
                    "    stop:0 rgba(255, 0, 0, 255), stop:1 rgba(0, 0, 255, 255));"
                    "  color: %1;"
                    "}")
                .arg(theme.textColor.name());
        QString licenseTextEditStyle = QString("QTextEdit#QHFAboutUsDialogLibrariesTextEdit {"
                                               "  background-color: %1;"
                                               "  color: %2;"
                                               "}")
                                           .arg(theme.contentPanelColor.name())
                                           .arg(theme.textColor.name());

        QString buttonStyle = QString("QPushButton#QHFAboutUsDialogCloseButton {"
                                      "  background-color: %1;"
                                      "  color: %2;"
                                      "  padding: 5px;"
                                      "  border: none;"
                                      "  border-radius: %5px;"
                                      " }"
                                      "QPushButton#QHFAboutUsDialogCloseButton:hover {"
                                      "  background-color: %3;"
                                      " }"
                                      "QPushButton#QHFAboutUsDialogCloseButton:pressed {"
                                      "  background-color: %4;"
                                      " }")
                                  .arg(theme.btnBgColor.name())
                                  .arg(theme.btnFgColor.name())
                                  .arg(theme.btnBgColor.lighter(120).name())
                                  .arg(theme.btnBgColor.darker(120).name())
                                  .arg(theme.borderRadius);

        // setstyle
        setStyleSheet(titleStyle + licenseTextEditStyle + buttonStyle);
    }

    void QHFAboutUsDialog::initialize() {

        //
        constexpr QSize defaultSize(420, 320);
        // Set window title
        setWindowTitle(tr("About") + " " + tr("QHiveFrame"));

        QVBoxLayout* layout = new QVBoxLayout(this);
        // settitle
        auto* titleLabel = new QLabel(this);
        titleLabel->setObjectName("QHFAboutUsDialogMainTitleLabel");
        titleLabel->setText(tr("QHiveFrame"));
        // setfont为1.5倍
        QFont font = titleLabel->font();
        font.setBold(true);
        font.setPointSizeF(font.pointSizeF() * 1.75); // 1.5倍字体
        titleLabel->setFont(font);

        layout->addWidget(titleLabel, 2);

        // setDescribes 
        auto* descriptionLabel = new QLabel(this);
        descriptionLabel->setText(
            tr("QHiveFrame is a modular Qt/C++ GUI application framework inspired by Blender's editor system."));
        layout->addWidget(descriptionLabel);

        // Version Info
        QHBoxLayout* versionLayout = new QHBoxLayout();
        versionLayout->setSpacing(10);
        QLabel* versionLabel = new QLabel(this);
        versionLabel->setText(tr("Current Version:"));
        versionLayout->addWidget(versionLabel);
        versionLayout->setAlignment(versionLabel, Qt::AlignLeft);

        QLabel* versionValueLabel = new QLabel(this);
        versionValueLabel->setText(PRODUCT_VERSION);
        versionLayout->addWidget(versionValueLabel);
        versionLayout->setAlignment(versionValueLabel, Qt::AlignLeft);

        // 在标签after添加弹性empty间
        versionLayout->addStretch(); // 让空间填充剩余宽度, 以靠左对齐
        layout->addLayout(versionLayout);

        // Copyrightinfo
        QLabel* copyrightLabel = new QLabel(this);
        QString copyrightText  = QString("<font color='#000000'>%1 &#169; %2 %3 %4</font>")
                                    .arg(tr("Copyright"))
                                    .arg(COPYRIGHT_YEAR)
                                    .arg(tr("QHiveFrame Contributors"))
                                    .arg(tr("All rights reserved."));

        copyrightLabel->setText(copyrightText);
        layout->addWidget(copyrightLabel);

        // 软件授权情况 horizontalLayout 标签 + 授权state
        QHBoxLayout* licenseLayout = new QHBoxLayout();
        QLabel*      licenseLabel  = new QLabel(tr("License:"), this);
        licenseLayout->addWidget(licenseLabel);

        // 授权state标签
        QLabel* licenseStatusLabel = new QLabel(this);
        QString licenseStatusText  = tr("Open Source (MIT License)");

        licenseStatusLabel->setText(licenseStatusText);
        licenseLayout->addWidget(licenseStatusLabel);

        // empty白empty间
        licenseLayout->addStretch();
        layout->addLayout(licenseLayout);

        // 列出开源库
        QLabel* librariesLabel = new QLabel(tr("Third-Party Libraries:"), this);
        layout->addWidget(librariesLabel);

        // create只读 多行文本框
        auto* librariesTextEdit = new QTextEdit(this);
        librariesTextEdit->setObjectName("QHFAboutUsDialogLibrariesTextEdit");
        // set不选取
        librariesTextEdit->setCursorWidth(0);
        librariesTextEdit->setReadOnly(true); // 设置为只读模式

        // set三方库list
        QString librariesInfo = "- Qt - %1 5.15.x (LGPL License)\n"
                                "- Assimp - %1 5.3 (BSD License)\n"
                                "- CGAL - %1 5.6.1 (LGPL License)\n"
                                "- Boost - %1 1.84.0 (Boost Software 2.0)";

        QString translatedVersion = tr("Version");
        librariesInfo             = librariesInfo.arg(translatedVersion);
        librariesTextEdit->setPlainText(librariesInfo); // 设置文本内容
        layout->addWidget(librariesTextEdit, 7);        // Adds to Layout中

        // setclose按钮
        // sethorizontalLayout, About按钮在right侧
        auto* buttonLayout = new QHBoxLayout();
        buttonLayout->addStretch(); // 添加弹性空间, 使按钮靠右
        auto* closeButton = new QPushButton(tr("Close"), this);
        closeButton->setObjectName("QHFAboutUsDialogCloseButton");
        buttonLayout->addWidget(closeButton);
        layout->addLayout(buttonLayout);

        // 应用Layout
        setLayout(layout);

        // Set window size
        setMinimumSize(defaultSize); // 设置最小窗口大小
        resize(defaultSize);         // 设置默认窗口大小

        // connection按钮点击signal到Slot function
        connect(closeButton, &QPushButton::clicked, this, &QHFAboutUsDialog::onCloseButtonClicked);
    }

} // namespace QHF
