#include "QHFPreferenceDialog.h"

#include "QHFMessageBox.h"
#include "QHFPreferencePanelBase.h"
#include "QHFPreferencePanelFactory.h"
#include "QHFPreferencesManager.h"
#include "QHFThemeManager.h"

#include <QDebug>
#include <QMainWindow>
#include <QPainter>
#include <QProcess>
#include <QStyleOption>

namespace QHF {

    /// --- QHFPreferDialogMenu implementation ---
    QHFPreferDialogMenu::QHFPreferDialogMenu(QWidget* parent) : QToolButton(parent) {
        //
        constexpr QSize defaultIconSize(16, 16);
        // Set object name
        setObjectName("QHFPreferDialogMenu");
        // Set stylesheet
        initTheme(QHFThemeManager::instance(), &QHFThemeManager::themeChanged);
        // set自己 缩放策略
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        // set图标
        setIcon(QIcon(":/gui/icons/menuIcon.svg"));
        // set图标尺寸
        setIconSize(defaultIconSize);
        // 无箭header
        setArrowType(Qt::NoArrow);
        // setmenu
        m_menu = new QMenu(this);
        // set按钮menuname
        m_menu->setObjectName("QHFPreferDialogMenuMenu");
        // 移除直角边框和阴影
        m_menu->setWindowFlags(m_menu->windowFlags() | Qt::FramelessWindowHint |
                               Qt::NoDropShadowWindowHint);
        // allow透明background
        m_menu->setAttribute(Qt::WA_TranslucentBackground);
        // set提示文本
        setToolTip(tr("Save&load"));
        // set按钮
        setMenu(m_menu);
        // 接受theme
        applyTheme();
        // Initializes
        initialize();
    }

    void QHFPreferDialogMenu::applyTheme() {
        //
        const auto& theme = QHFThemeManager::instance().theme();
        QString menuStyle = QString("QMenu#QHFPreferDialogMenuMenu {"
                                    "    border: 1px solid %1;"
                                    "    border-radius: %1px;"
                                    "    padding: %3;"
                                    "}"
                                    "QMenu#QHFPreferDialogMenuMenu::item {"
                                    "    padding: %4;"
                                    "    border-radius: %1px;"
                                    "}"
                                    "QMenu#QHFPreferDialogMenuMenu::item:selected {"
                                    "    background-color: %2;"
                                    "}")
                                .arg(theme.borderRadius)
                                .arg(theme.menuItemSelectedColor.name())
                                .arg(theme.menuPadding)
                                .arg(theme.menuItemPadding);
        setStyleSheet(menuStyle);
    }

    void QHFPreferDialogMenu::initialize() {
        // 点击按钮时向top弹出menu
        m_menu->addAction(tr("Save user settings"), this,
                          [this]() { emit evtSaveUserSettingsClicked(); });
        m_menu->addAction(tr("Load default settings"), this,
                          [this]() { emit evtResetDefaultSettingsClicked(); });
        connect(this, &QToolButton::clicked, [=]() {
            QPoint pos = mapToGlobal(QPoint(0, -m_menu->sizeHint().height()));
            m_menu->exec(pos);
        });
    }

    /// --- QHFPreferenceDialog implementation ---
    QHFPreferenceDialog::QHFPreferenceDialog(QWidget* parent) : QDialog(parent) {
        // Initializes
        m_mainLayout = nullptr;
        m_tabbar = nullptr;
        m_stackedWidget = nullptr;
        m_panelMap.clear();
        // 强制checkParent object
        if (qobject_cast<QMainWindow*>(parent) == nullptr) {
            qWarning() << "QHFAppHeader parent must be QMainWindow or its subclass";
        }
        // Set object name
        setObjectName("QHFPreferenceDialog");
        // Set window title
        QString title = tr("My App") + " - " + tr("Preferences Settings");
        // Set window title
        setWindowTitle(title);
        // set为自毁对象
        setAttribute(Qt::WA_DeleteOnClose);
        // setFlag
        setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
        // Set stylesheet
        initTheme(QHFThemeManager::instance(), &QHFThemeManager::themeChanged);
        // set自己 缩放策略
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        // Initializes
        initialize();
    }

    void QHFPreferenceDialog::paintEvent(QPaintEvent* event) {
        // 只调用base class  paintEvent, 让style表生效
        QStyleOption opt;
        opt.initFrom(this);
        QPainter painter(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
        QWidget::paintEvent(event);
    }

    void QHFPreferenceDialog::onCancel() {
        // close对话框并恢复初始set
        close();
    }

    /// NOLINTNEXTLINE(readability-function-cognitive-complexity)
    void QHFPreferenceDialog::onSaveUserSettings() {
        QList<Setting> newSettings = m_settings;
        QList<QString> restartRequiredKeys;

        // 收集panel修改
        for (const auto& panel : m_panelMap.values()) {
            if (auto* prefPanel = dynamic_cast<IPreferencePanel*>(panel)) {
                QList<Setting> panelSettings = prefPanel->saveSettings();
                for (const auto& setting : panelSettings) {
                    for (auto& existing : newSettings) {
                        if (existing.key == setting.key) {
                            existing.value = setting.value;
                            break;
                        }
                    }
                }
            }
        }

        // check需要重启 set
        for (const auto& setting : newSettings) {
            auto origIt =
                std::find_if(m_originalSettings.constBegin(), m_originalSettings.constEnd(),
                             [&setting](const Setting& sett) { return sett.key == setting.key; });
            if (origIt != m_originalSettings.constEnd() && origIt->value != setting.value &&
                setting.requiresRestart) {
                restartRequiredKeys.append(setting.key);
            }
        }

        // saveset
        QHFPreferencesManager::instance().saveSettings(newSettings);
        m_settings = newSettings;
        m_originalSettings = newSettings;

        // 弹出重启提示
        if (!restartRequiredKeys.isEmpty()) {
            QString message = tr("The following settings require a restart to take effect:\n") +
                              restartRequiredKeys.join("\n");
            QHFMessageBox msgBox(tr("Restart Required"), QHFMessageBox::QHFMessageBox::INFO, message,
                               QHFMessageBox::YES | QHFMessageBox::NO, this);
            constexpr QSize defaultSize(420, 150);
            msgBox.setMinimumSize(defaultSize);
            msgBox.setMaximumSize(defaultSize);
            msgBox.setButtonText(QHFMessageBox::YES, tr("Restart Now"));
            msgBox.setButtonText(QHFMessageBox::NO, tr("Later"));
            msgBox.exec();
            if (msgBox.result() == QHFMessageBox::YES) {
                QProcess process;
                process.setProgram(QApplication::applicationFilePath());
                if (process.startDetached()) {
                    QApplication::quit();
                } else {
                    qDebug() << "Error:" << process.errorString();
                }
            }
        }
    }

    void QHFPreferenceDialog::onResetDefaultSettings() {
        m_settings = QHFPreferencesManager::instance().loadDefaultSettings();
        for (const auto& panel : m_panelMap.values()) {
            if (auto* prefPanel = dynamic_cast<IPreferencePanel*>(panel)) {
                QList<Setting> panelSettings;
                for (const auto& setting : m_settings) {
                    if (setting.key.startsWith(prefPanel->panelName().toLower() + ".")) {
                        panelSettings.append(setting);
                    }
                }
                prefPanel->loadSettings(panelSettings);
            }
        }
    }

    void QHFPreferenceDialog::applyTheme() {
        // Applies themestyle
        const auto& theme = QHFThemeManager::instance().theme();
        QString dialogStyle = QString("#QHFPreferenceDialog {"
                                      "    background-color: %1;"
                                      "}")
                                  .arg(theme.innerPanelColor.name());

        QString tabbarStyle = QString("QTabBar#HPreferenceTabBar::tab {"
                                      "    background-color: %1;"
                                      "    border-radius: %4px;"
                                      "    padding: %5;"
                                      "}"
                                      "QTabBar#HPreferenceTabBar::tab:selected {"
                                      "    background-color: %2;"
                                      "}"
                                      "QTabBar#HPreferenceTabBar::tab:hover {"
                                      "    background-color: %3;"
                                      "}")
                                  .arg(theme.tabBarTabColor.name())
                                  .arg(theme.tabBarTabPressedColor.name())
                                  .arg(theme.tabBarTabHoverColor.name())
                                  .arg(theme.borderRadius)
                                  .arg(theme.tabBarPadding);

        setStyleSheet(dialogStyle + tabbarStyle);
    }

    void QHFPreferenceDialog::initialize() {
        //
        const QPair<int, int> layoutStretch(2, 8);
        constexpr QSize defaultSize(600, 400);
        constexpr int defaultMargin = 10;
        constexpr int defaultSpacing = 5;
        constexpr int defaultContentMargin = 0;
        constexpr int defaultContentSpacing = 5;
        constexpr int defaultTabMargin = 5;
        // 收集panelinfo
        collectPanelNames();
        // InitializesLayout
        m_mainLayout = new QVBoxLayout(this);
        m_mainLayout->setContentsMargins(defaultMargin, defaultMargin, defaultMargin,
                                         defaultMargin);
        m_mainLayout->setSpacing(defaultSpacing);
        // horizontalLayout
        auto* contentLayout = new QHBoxLayout();
        contentLayout->setContentsMargins(defaultContentMargin, defaultContentMargin,
                                          defaultContentMargin, defaultContentMargin);
        contentLayout->setSpacing(defaultContentSpacing);
        // option卡栏
        auto* tabbarLayout = new QVBoxLayout();
        tabbarLayout->setContentsMargins(defaultTabMargin, defaultTabMargin, defaultTabMargin,
                                         defaultTabMargin);
        tabbarLayout->setSpacing(0);
        m_tabbar = new QTabBar(this);
        m_tabbar->setObjectName("HPreferenceTabBar");
        m_tabbar->setElideMode(Qt::ElideRight);
        m_tabbar->setTabsClosable(false);
        m_tabbar->setMovable(false);
        m_tabbar->setUsesScrollButtons(true);
        m_tabbar->setDocumentMode(true);
        m_tabbar->setShape(QTabBar::RoundedNorth);
        m_tabbar->setFocusPolicy(Qt::NoFocus);
        for (const auto& panel : m_panelMap.values()) {
            auto* panelBase = qobject_cast<QHFPreferencePanelBase*>(panel);
            const QString& displayName = panelBase->transName();
            m_tabbar->addTab(displayName);
        }
        m_tabbar->setCurrentIndex(0);
        tabbarLayout->addWidget(m_tabbar);
        tabbarLayout->addStretch(1);
        // loadmenu
        auto* menuLayout = new QHBoxLayout();
        menuLayout->setContentsMargins(0, 0, 0, 0);
        menuLayout->setSpacing(0);
        auto* muButton = new QHFPreferDialogMenu(this);
        menuLayout->addWidget(muButton);
        menuLayout->addStretch(1);
        tabbarLayout->addLayout(menuLayout);
        // contentLayout
        contentLayout->addLayout(tabbarLayout, layoutStretch.first);
        // option卡content
        m_stackedWidget = new QStackedWidget(this);
        //
        for (const auto& panel : m_panelMap.values()) {
            m_stackedWidget->addWidget(panel);
        }
        m_stackedWidget->setCurrentIndex(0);
        contentLayout->addWidget(m_stackedWidget, layoutStretch.second);
        m_mainLayout->addLayout(contentLayout);

        // loadset (只loadcurrentpanel set)
        const auto& preManager = QHFPreferencesManager::instance();
        m_originalSettings = preManager.loadSettings();
        m_settings = m_originalSettings;
        if (auto* panel = qobject_cast<QHFPreferencePanelBase*>(m_stackedWidget->currentWidget())) {
            QList<Setting> panelSettings;
            for (const auto& setting : m_settings) {
                if (setting.key.startsWith(panel->panelName().toLower() + ".")) {
                    panelSettings.append(setting);
                }
            }
            panel->loadSettings(panelSettings);
        }

        // signalconnection
        // 延迟loadotherpanel
        connect(m_tabbar, &QTabBar::currentChanged, this, [this](int index) {
            if (auto* panel = dynamic_cast<QHFPreferencePanelBase*>(m_stackedWidget->widget(index))) {
                QList<Setting> panelSettings;
                for (const auto& setting : m_settings) {
                    if (setting.key.startsWith(panel->panelName().toLower() + ".")) {
                        panelSettings.append(setting);
                    }
                }
                panel->loadSettings(panelSettings);
            }
        });

        // connectionsignal
        connect(muButton, &QHFPreferDialogMenu::evtSaveUserSettingsClicked, this,
                &QHFPreferenceDialog::onSaveUserSettings);
        connect(muButton, &QHFPreferDialogMenu::evtResetDefaultSettingsClicked, this,
                &QHFPreferenceDialog::onResetDefaultSettings);

        // Applies theme
        applyTheme();
        // Set window size
        setMinimumSize(defaultSize);
        resize(defaultSize);
    }

    void QHFPreferenceDialog::collectPanelNames() {
        m_panelMap.clear();
        // Iterate allregister panel
        const auto& factory = QHFPreferencePanelFactory::instance();
        for (const auto& info : factory.panels()) {
            m_panelMap[info.name] = factory.createPanel(info.name, this);
        }
    }
} // namespace QHF
