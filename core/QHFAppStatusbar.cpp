#include "QHFAppStatusbar.h"

#include <QDebug>
#include <QIcon>
#include <QMainWindow>
#include <QMetaObject>
#include <QPair>

#include "QHFThemeManager.h"

namespace QHF {

    QHFAppStatusBar::QHFAppStatusBar(QWidget* parent) : QStatusBar(parent) {
        // 强制checkParent object
        if (qobject_cast<QMainWindow*>(parent) == nullptr) {
            qWarning() << "QHFAppStatusBar parent must be QMainWindow or its subclass";
        }
        // Initializes变量
        m_promptContainer = nullptr;
        m_promptLayout    = nullptr;
        m_statsLabel      = nullptr;
        m_versionLabel    = nullptr;
        // Set object name
        setObjectName("QHFAppStatusBar");
        // setbackground为透明
        // setAttribute(Qt::WA_StyledBackground, true);
        // Initializestheme
        initTheme(QHFThemeManager::instance(), &QHFThemeManager::themeChanged);
        // Initializes
        initialize();
    }

    void QHFAppStatusBar::initialize() {
        // initial state栏提示 (可变info)
        constexpr int promptMargin = 5;
        constexpr int promptSpace  = 10;
        m_promptContainer          = new QWidget(this);
        m_promptLayout             = new QHBoxLayout(m_promptContainer);
        m_promptLayout->setContentsMargins(0, 0, 0, promptMargin);
        m_promptLayout->setSpacing(promptSpace);
        addWidget(m_promptContainer, 1);
        // state标签
        m_statsLabel = new QLabel("", this);
        addPermanentWidget(m_statsLabel, 1);

        // 产品name
        QString appName = tr("My App");
        // Version Info
        QString version = "1.0.0";
#ifdef PRODUCT_VERSION
        version = PRODUCT_VERSION;
#endif
        m_versionLabel = new QLabel(QString("%1 v%2").arg(appName).arg(version), this);
        addPermanentWidget(m_versionLabel, 0);
        // 接受theme
        applyTheme();
        // signalconnection
        connect(&QHFNotifierCenter::instance(), &QHFNotifierCenter::notify, this,
                &QHFAppStatusBar::handleNotify);
    }

    void QHFAppStatusBar::applyTheme() {
        QString styleStr = "QStatusBar#QHFAppStatusBar { background-color: red; }";
        setStyleSheet(styleStr);
    }

    void QHFAppStatusBar::updatePrompts(const QList<QPair<QIcon, QString>>& prompts) {
        constexpr QSize promptSize(16, 16);
        while (QLayoutItem* item = m_promptLayout->takeAt(0)) {
            delete item->widget();
            delete item;
        }
        for (const auto& prompt : prompts) {
            auto* label = new QLabel(this);
            label->setPixmap(prompt.first.pixmap(promptSize));
            label->setText(prompt.second);
            m_promptLayout->addWidget(label);
            if (&prompt != &prompts.last()) {
                auto* separator = new QLabel("|", this);
                m_promptLayout->addWidget(separator);
            }
        }
        m_promptLayout->addStretch();
    }

    void QHFAppStatusBar::updateStats(const QString& stats) { m_statsLabel->setText(stats); }

    void QHFAppStatusBar::handleNotify(const QHFNotifier& n) {
        /*
        if (n.window != parent())
            return;
        if (n.type == NotifierType::TOOL_CHANGED) {
            QString toolName = n.data.toString();
            QList<QPair<QIcon, QString>> prompts;
            if (toolName == "Select") {
                prompts << qMakePair(QIcon(":/icons/lmb.png"), "Select")
                        << qMakePair(QIcon(":/icons/rmb.png"), "Menu");
            }
            updatePrompts(prompts);
        } else if (n.type == NotifierType::SCENE_UPDATED) {
            QVariantMap data = n.data.toMap();
            QString stats = QString("Verts: %1 | Faces: %2 | %3")
                                .arg(data["verts"].toInt())
                                .arg(data["faces"].toInt())
                                .arg(data["mode"].toString());
            updateStats(stats);
        } else if (n.type == NotifierType::HOVER_CHANGED) {
            QVariantMap data = n.data.toMap();
            updatePrompts({{data["icon"].value<QIcon>(), data["tooltip"].toString()}});
        }
            */
    }

} // namespace QHF
