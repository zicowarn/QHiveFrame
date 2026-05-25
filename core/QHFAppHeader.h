#ifndef QHFAPPHEADER_H
#define QHFAPPHEADER_H

#include <QAction>
#include <QMenuBar>
#include <QObject>
#include <QTabBar>
#include <QVBoxLayout>
#include <QWidgetAction>

#include "QHFGuiNotifierCenter.h"
#include "QHFMenuFactory.h"
#include "QHFUIUtils.h"

namespace QHF {

    using MenuName    = StrongType<struct MenuNameTag, QString>;
    using DisplayName = StrongType<struct DisplayNameTag, QString>;
    using MenuPath    = StrongType<struct MenuPathTag, QString>;
    using TransName   = StrongType<struct TransKeyTag, QString>;

    /**
     * @brief Menu (see description)
     * @details (see source)QMainWindow
     */
    class QHFAppHeader : public QWidget, public QHFThemedCRTP<QHFAppHeader> {
        Q_OBJECT

      private:
        QVBoxLayout*          m_layout;          ///< Layout
        QMenuBar*             m_menuBar;         ///< menu栏
        QMap<QString, QMenu*> m_menusMap;        ///< menulist (Path -> Menu)
        QAction*              m_statusBarAction; ///< Status bar菜单项
        QTabBar*              m_tabBar;          ///< 选项卡栏

      public:/**
         * @brief  (see description)
         * @param parent Parent object
         */
        explicit QHFAppHeader(QWidget* parent = nullptr);
        ~QHFAppHeader() override = default; ///< Destructor

      protected:
        /**
         * @brief  (see description)
         * @param event 绘制eventpointer*/
        void paintEvent(QPaintEvent* event) override;

      private:
        /**
         * @brief Initializes (see description)
         */
        void initialize();

        /**
         * @brief InitializesLayout
         */
        void initLayout();

        /**
         * @brief Initializes (see description)
         * @details (see source)State manager
         */
        void initializeActionsState();

        /**
         * @brief Applies theme
         * @details (see source)
         */
        void applyTheme();

        /**
         * @brief  (see description)
         * @param notifier notification*/
        void handleNotify(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @param parent Parent object
         * @return Logo
         */
        static QWidget* createLogo(QWidget* parent); // 新增: 创建Logo

        /**
         * createmenu栏
         * @param parent Parent object
         */
        QMenuBar* createMenuBar(QWidget* parent);

        /**
         * @brief  (see description)
         * @param menuBar menu栏* @param path menupath* @param menuName menuname* @param displayName menu翻译键* @param items menu项list* @return 构建 menu*/
        QMenu* createMenu(QMenuBar* menuBar, const MenuPath& path, const MenuName& menuName,
                          const DisplayName&     displayName,
                          const QList<MenuItem>& items); // 新增: 构建单个菜单

        /**
         * @brief  (see description)
         * @param menuBar menu栏* @param path menupath* @param menuName menuname* @param displayName menu翻译键* @return menu*/
        QMenu* findOrCreateMenu(QMenuBar* menuBar, const MenuPath& path, const MenuName& menuName,
                                const DisplayName& displayName); // 新增: 查找或创建菜单

        /**
         * @brief  (see description)
         * @param parentMenu 父menu* @param path menupath* @param transName menu翻译键* @param order menuorder* @return 子menu*/
        QMenu* createSubMenu(QMenu* parentMenu, const MenuPath& path, const TransName& transName,
                             int order); // 新增: 添加子菜单

        /**
         * @brief  (see description)
         * @details (see source)State manager)
         * @param parent Parent object
         * @return option卡栏*/
        QTabBar* createTabBar(QWidget* parent); // 新增: 创建选项卡栏
    };

} // namespace QHF

#endif // HAPPHEADER_H
