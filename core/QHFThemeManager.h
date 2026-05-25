#ifndef QHFTHMEMANAGER_H
#define QHFTHMEMANAGER_H

#include "QHFUIUtils.h"

#include <QColor>
#include <QFont>

namespace QHF {

    /**
     * @brief Theme type
     * @details (see source)
     */
    enum class ThemeType : std::uint8_t {
        LIGHT,
        DARK,
        OCEAN, // 浅蓝系
        SUNNY  // 浅黄系
    };

    /**
     * @brief Theme structure
     * @details (see source)
     */
    struct Theme {
        QColor backgroundColor;
        QColor textColor;
        QColor borderColor;
        QColor normalPanelColor; ///< 内容panelcolor, backgroundColor  外层, 用于显示内容 panel
        QColor innerPanelColor;///< 内层panelcolor, backgroundColor  内层, 普通editor background色
        QColor contentPanelColor;///< 内容panelcolor, innerPanelColor  内层
        QColor iconBtnBgColor;
        QColor iconBtnHoverBgColor;
        QColor iconBtnPressedBgColor;
        QColor btnBgColor;
        QColor btnFgColor;
        QString tabBarPadding;
        QString tabBarMarginRight;
        QString tabBarMarginBottom;
        QColor tabBarTabColor;
        QColor tabBarTabHoverColor;
        QColor tabBarTabPressedColor;
        QColor menuItemSelectedColor;
        QString cusMenuPadding;
        QString menuPadding;
        QString menuItemPadding;
        QColor comboBoxBgColor;
        QColor comboBoxHoverBgColor;
        QColor comboBoxSelecedColor;
        QString spinBoxMaskReleasedColor;
        QString spinBoxMaskPressedColor;
        QString spinBoxMaskFocusedColor;

        QFont font;
        int borderRadius;
        int padding;/**
         * @brief  (see description)
         */
        static Theme Light();

        /**
         * @brief  (see description)
         */
        static Theme Dark();

        /**
         * @brief  (see description)
         */
        static Theme Ocean();

        /**
         * @brief  (see description)
         */
        static Theme Sunny();
    };

    /**
     * @brief Theme manager
     * @details (see source)
     */
    class QHFThemeManager : public QObject {
        Q_OBJECT

      private:
        ThemeType m_type; ///< CurrentTheme type
        Theme m_theme;    ///< Current主题

      signals:
        /**
         * @brief Theme changed signal
         */
        void themeChanged();

      public:
        /**
template         * @brief Returns singleton instance
         * @return 单例object*/
        static QHFThemeManager& instance();

        /**
         * @brief  (see description)
         * @param type Theme type
         */
        void setThemeType(ThemeType type);

        /**
         * @brief Returns current Theme type
         * @return current Theme type*/
        [[nodiscard]] ThemeType themeType() const;

        /**
         * @brief Returns current  (see description)
         * @return current theme*/
        [[nodiscard]] const Theme& theme() const;

      private:
        /**
         * @brief  (see description)
         */
        QHFThemeManager();
        ~QHFThemeManager() override = default; // 默认Destructor
    };

} // namespace QHF

#endif // HTHMEMANAGER_H
