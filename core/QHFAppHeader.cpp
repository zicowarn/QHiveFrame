#include "QHFAppHeader.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QPainter>
#include <QStyleOption>
#include <QVBoxLayout>

#include "QHFMenuFactory.h"
#include "QHFStateManager.h"
#include "QHFThemeManager.h"
#include "QHFWorkspaceFactory.h"

namespace QHF {

    /// --- QHFAppHeader implementation ---
    QHFAppHeader::QHFAppHeader(QWidget* parent) : QWidget(parent) {
        // 强制checkParent object
        if (qobject_cast<QMainWindow*>(parent) == nullptr) {
            qWarning() << "QHFAppHeader parent must be QMainWindow or its subclass";
        }
        // InitializesMember variable
        m_layout  = nullptr;
        m_menuBar = nullptr;
        m_menusMap.clear();
        m_statusBarAction = nullptr;
        m_tabBar          = nullptr;
        // Set object name
        setObjectName("QHFAppHeader");
        // setbackground为透明
        // setAttribute(Qt::WA_StyledBackground, true);
        // Initializestheme
        initTheme(QHFThemeManager::instance(), &QHFThemeManager::themeChanged);
        // set自己 缩放策略
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        // Initializes
        initialize();
    }

    void QHFAppHeader::paintEvent(QPaintEvent* event) {
        // 只调用base class  paintEvent, 让style表生效
        QStyleOption opt;
        opt.initFrom(this);
        QPainter painter(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
        QWidget::paintEvent(event);
    }

    void QHFAppHeader::initialize() {
        // InitializesLayout
        initLayout();
        // Initializesstatus barmenu项
        initializeActionsState();
        // Applies theme
        applyTheme();
    }

    void QHFAppHeader::initLayout() {
        //
        constexpr int vLayoutMargin    = 0;
        constexpr int vLayoutSpacing   = 0;
        constexpr int hLayoutMargin    = 10;
        constexpr int hLayoutSpacing   = 10;
        constexpr int menuLeftSpacing  = 5;
        constexpr int menuRightSpacing = 35;
        constexpr int menuBarMargin    = 5;
        constexpr int tabBarMargin     = 0;
        // verticalLayout
        m_layout = new QVBoxLayout();
        m_layout->setContentsMargins(vLayoutMargin, vLayoutMargin, vLayoutMargin, vLayoutMargin);
        m_layout->setSpacing(vLayoutSpacing);
        // 中间horizontalLayout
        auto* layout = new QHBoxLayout();
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(hLayoutSpacing);
        // left边empty隙
        layout->addSpacing(menuLeftSpacing);
        // App Logo
        layout->addWidget(createLogo(this));
        // menu栏
        auto* menuLayout = new QVBoxLayout();
        menuLayout->setContentsMargins(menuBarMargin, menuBarMargin, menuBarMargin, menuBarMargin);
        m_menuBar = createMenuBar(this);
        menuLayout->addWidget(m_menuBar);
        layout->addLayout(menuLayout);
        // empty隙
        layout->addSpacing(menuRightSpacing);
        // Tabbar
        auto* tabLayout = new QVBoxLayout();
        tabLayout->setContentsMargins(tabBarMargin, tabBarMargin, tabBarMargin, tabBarMargin);
        tabLayout->addStretch(1);
        m_tabBar = createTabBar(this);
        tabLayout->addWidget(m_tabBar);
        layout->addLayout(tabLayout);
        // empty隙
        layout->addStretch(1);
        // Layout
        m_layout->addLayout(layout);
        // Set layout
        setLayout(m_layout);

        // 监听signal
        connect(m_tabBar, &QTabBar::currentChanged, this, [=](int index) {
            qDebug() << "Tabbar current changed: " << index;
            // Update state
            auto* parenWin = qobject_cast<QMainWindow*>(this->parent());
            QHFStateManager::instance().setMainWinWorkspaceIndex(parenWin, index);
            // 发送event
            QHFNotifier note(QHFNotifierType::SET_WORKSPACE_INDXE);
            note.set("index", index);
            QHFNotifierCenter::instance().publish(note);
        });

        // 监听event
        connect(&QHFNotifierCenter::instance(), &QHFNotifierCenter::notify, this,
                &QHFAppHeader::handleNotify);
    }

    void QHFAppHeader::initializeActionsState() {}

    void QHFAppHeader::applyTheme() {
        // Returns current theme
        const auto& theme = QHFThemeManager::instance().theme();
        // QHFAppHeader style
        QString comMenuStyle = QString("QMenuBar::item:selected {"
                                       " background-color: %1;"
                                       " border-radius: %2px;"
                                       "}"
                                       "QMenu#HMenuBarHMenu {"
                                       " border-radius: %2px;"
                                       " padding: %3;"
                                       "}"
                                       "QMenu#HMenuBarHMenu::item {"
                                       " border-radius: %2px;"
                                       " padding: %4;"
                                       "}"

                                       "QMenu#HMenuBarHMenu::item:selected {"
                                       " background-color: %1;"
                                       "}")
                                   .arg(theme.menuItemSelectedColor.name())
                                   .arg(theme.borderRadius)
                                   .arg(theme.menuPadding)
                                   .arg(theme.menuItemPadding);

        QString subMenuStyle = QString("QMenu#HMenuBarHSubMenu {"
                                       " border-radius: %2px;"
                                       " padding: %3;"
                                       "}"
                                       "QMenu#HMenuBarHSubMenu::item {"
                                       " border-radius: %2px;"
                                       " padding: %4;"
                                       "}"

                                       "QMenu#HMenuBarHSubMenu::item:selected {"
                                       " background-color: %1;"
                                       "}")
                                   .arg(theme.menuItemSelectedColor.name())
                                   .arg(theme.borderRadius)
                                   .arg(theme.menuPadding)
                                   .arg(theme.menuItemPadding);

        // QHFTabbar Style
        QString tabBarStyle = QString("QTabBar#QHFTabBar::tab {"
                                      "  background-color: %1;"
                                      "  padding: %4;"
                                      "  margin-right: %5;"
                                      "  border-radius: %3px;"
                                      "}"
                                      "QTabBar#QHFTabBar::tab:hover {"
                                      " background-color: %2;"
                                      "}"
                                      "QTabBar#QHFTabBar::tab:selected {"
                                      " background-color: %2;"
                                      "}")
                                  .arg(theme.backgroundColor.name())
                                  .arg(theme.tabBarTabHoverColor.name())
                                  .arg(theme.borderRadius)
                                  .arg(theme.tabBarPadding)
                                  .arg(theme.tabBarMarginRight);
        // referencestyle
        setStyleSheet(comMenuStyle + subMenuStyle + tabBarStyle);
    }

    void QHFAppHeader::handleNotify(const QHFNotifier& notifier) {
        // 切换workspace
        auto* parenWin = qobject_cast<QMainWindow*>(this->parent());
        auto* activWin = qApp->activeWindow();
        // checkcurrentwindow是否activate
        if (parenWin != activWin) {
            return;
        }
        // Handle notification
        int currentIndex = m_tabBar->currentIndex();
        int nextIndex    = 0;
        int prevIndex    = 0;
        switch (notifier.type) {
        case QHFNotifierType::NEXT_WORKSPACE:
            nextIndex = currentIndex + 1;
            if (nextIndex >= m_tabBar->count()) {
                nextIndex = 0;
            }
            m_tabBar->setCurrentIndex(nextIndex);
            // Update state
            QHFStateManager::instance().setMainWinWorkspaceIndex(parenWin, nextIndex);
            break;
        case QHFNotifierType::PREV_WORKSPACE:
            prevIndex = currentIndex - 1;
            if (prevIndex < 0) {
                prevIndex = m_tabBar->count() - 1;
            }
            m_tabBar->setCurrentIndex(prevIndex);
            // Update state
            QHFStateManager::instance().setMainWinWorkspaceIndex(parenWin, prevIndex);
            break;
        default:
            break;
        }
    }

    QWidget* QHFAppHeader::createLogo(QWidget* parent) {
        // 使用icon
        auto* logo = new QLabel(parent);
        logo->setPixmap(QPixmap(":/images/logo.png"));
        logo->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        return logo;
    }

    QMenuBar* QHFAppHeader::createMenuBar(QWidget* parent) {
        //
        auto* menuBar = new QMenuBar(parent);
        //
        menuBar->setObjectName("HMenuBar");
        // iterateregister 顶级menu
        for (const auto& menuInfo : QHFMenuRegistry::instance().menus()) {
            // Create child类
            auto* menu = QHFMenuRegistry::instance().createMenu(menuInfo.name, this);
            // Get child项
            QList<MenuItem> items = menu->menuItems();
            // check子项
            if (items.empty()) {
                continue;
            }
            // get显示name
            QString displayName = menu->transName();
            // Create menu
            createMenu(menuBar, MenuPath{menuInfo.name}, MenuName{menuInfo.name},
                       DisplayName{displayName}, items);
        }
        return menuBar;
    }

    QMenu* QHFAppHeader::createMenu(QMenuBar* menuBar, const MenuPath& path, const MenuName& menuName,
                                  const DisplayName& displayName, const QList<MenuItem>& items) {
        // create或find顶级menu
        QMenu* menu = findOrCreateMenu(menuBar, path, menuName, displayName);
        // 移除直角边框和阴影
        menu->setWindowFlags(menu->windowFlags() | Qt::FramelessWindowHint |
                             Qt::NoDropShadowWindowHint);
        // allow透明background
        menu->setAttribute(Qt::WA_TranslucentBackground);
        // iteratemenu项
        for (const auto& item : items) {
            // 处理分隔符
            QStringList pathParts = item.path.split("/");
            //
            QMenu* currentMenu = menu;
            // 处理子menupath
            for (int i = 0; i < pathParts.size() - 1; ++i) {
                currentMenu = createSubMenu(currentMenu, MenuPath{pathParts[i]},
                                            TransName{pathParts[i]}, item.order);
            }
            // 添加子menu
            if (item.type == MenuItemType::SUBMENU) {
                qDebug() << "CreateMenu: Add sub menu" << item.path << item.order;
                // Create childmenu
                createSubMenu(currentMenu, MenuPath{pathParts.last()}, TransName{item.transName},
                              item.order);
            } // 添加动作
            if (item.type == MenuItemType::ACTION) {
                qDebug() << "CreateMenu: Add action" << item.path << item.order;
                if (item.notifierType == QHFNotifierType::TOGGLE_STATUS_BAR) {
                    m_statusBarAction = item.action;
                    // Get state态
                    auto* parenWin = qobject_cast<QMainWindow*>(this->parent());
                    bool  isShow   = QHFStateManager::instance().getStatusBarVisible(parenWin);
                    // setstate
                    m_statusBarAction->blockSignals(true);
                    m_statusBarAction->setChecked(isShow);
                    m_statusBarAction->blockSignals(false);
                }
                item.action->setText(item.transName);
                currentMenu->addAction(item.action);
            }
            if (item.type == MenuItemType::CUSTOMACTION) {
                qDebug() << "CreateMenu: Add custom action" << item.path << item.order;
                QMenu* subMenu = createSubMenu(currentMenu, MenuPath{pathParts.last()},
                                               TransName{item.transName}, item.order);
                subMenu->addAction(item.action);
            }
            if (item.type == MenuItemType::SEPARATOR) {
                qDebug() << "CreateMenu: Add separator" << item.path << item.order;
                currentMenu->addSeparator();
            }
        }
        return menu;
    }

    QMenu* QHFAppHeader::findOrCreateMenu(QMenuBar* menuBar, const MenuPath& path,
                                        const MenuName& menuName, const DisplayName& displayName) {
        // find现有menu 从 m_menusMap cache
        if (m_menusMap.contains(path)) {
            return m_menusMap[path];
        }
        // create新menu
        auto* menu = menuBar->addMenu(displayName);
        // Set object name
        menu->setObjectName("HMenuBarHMenu");
        // cache
        m_menusMap[path] = menu;
        // Returns 
        return menu;
    }

    QMenu* QHFAppHeader::createSubMenu(QMenu* parentMenu, const MenuPath& path,
                                     const TransName& transName, int order) {
        // find现有子menu
        if (m_menusMap.contains(path)) {
            return m_menusMap[path];
        }
        // create新子menu
        auto* subMenu = new QMenu(transName, parentMenu);
        // Set object name
        subMenu->setObjectName("HMenuBarHSubMenu");
        // 加入主menu
        parentMenu->addMenu(subMenu);
        // cache
        m_menusMap[path] = subMenu;
        // Returns 
        return subMenu;
    }

    QTabBar* QHFAppHeader::createTabBar(QWidget* parent) {
        // createoption卡栏
        auto* tabBar = new QTabBar(parent);
        // Set object name
        tabBar->setObjectName("QHFTabBar");
        // 可选: 使用紧凑style
        tabBar->setDocumentMode(true); // 更紧凑 标签样式
        // set drawbase 属性为 false, 隐藏底部横线
        tabBar->setDrawBase(false);
        // 从WorkspaceRegistrygetworkspacelist
        auto workspaces = QHFWorkspaceRegistry::instance().workspaces();
        // iterateworkspacelist
        for (const auto& workspaceInfo : workspaces) {
            // createworkspace标签
            auto workspaceType = workspaceInfo.type;
            // 添加标签
            tabBar->addTab(QHFWorkspaceTypeUtils::toTransName(workspaceType));
        }
        // 从state中心Returns current workspaceindex
        auto* parenWin     = qobject_cast<QMainWindow*>(this->parent());
        int   currentIndex = QHFStateManager::instance().getMainWinWorkspaceIndex(parenWin);
        tabBar->setCurrentIndex(currentIndex);
        // Returns 
        return tabBar;
    }

} // namespace QHF
