
#include "QHFThemeManager.h"

namespace QHF {

    constexpr int DEFAULT_FONT_SIZE = 10;
    constexpr int DEFAULT_BORDER_RADIUS = 5;
    constexpr int DEFAULT_PADDING = 6;

    /// --- 预定义themeimplementation ---
    Theme Theme::Light() {
        Theme thm;
        thm.backgroundColor = QColor("#FAFAFA");       // 背景 #FAFAFA 比 #FFFFFF 柔和
        thm.textColor = QColor("#000000");             // 黑色文本
        thm.borderColor = QColor("#DDDDDD");           // 淡灰色边框
        thm.normalPanelColor = QColor("#F0F0F0");      // 面板灰, 区分层级
        thm.innerPanelColor = QColor("#F5F5F5");       // 更亮一点 内层面板
        thm.contentPanelColor = QColor("#FFFFFF");     // 内容面板全白
        thm.iconBtnBgColor = QColor("#E0E0E0");        // Icon按钮默认浅灰
        thm.iconBtnHoverBgColor = QColor("#D5D5D5");   // Hover 时更深灰
        thm.iconBtnPressedBgColor = QColor("#3F7BFF"); // 强调色统一蓝
        thm.btnBgColor = QColor("#E0E0E0");            // 普通按钮背景
        thm.btnFgColor = QColor("#000000");            // 按钮文字黑色
        thm.tabBarPadding = "6px 10px";
        thm.tabBarMarginRight = "8px";
        thm.tabBarMarginBottom = "8px";
        thm.tabBarTabColor = QColor("#E0E0E0");        // 默认 Tab 灰
        thm.tabBarTabHoverColor = QColor("#D0D0D0");   // Hover 浅灰
        thm.tabBarTabPressedColor = QColor("#3F7BFF"); // 统一强调色
        thm.menuItemSelectedColor = QColor("#E8E8E8"); // 菜单选中灰
        thm.cusMenuPadding = "4px";
        thm.menuPadding = "4px";
        thm.menuItemPadding = "6px 10px";
        thm.comboBoxBgColor = QColor("#FFFFFF");      // ComboBox 背景白
        thm.comboBoxHoverBgColor = QColor("#F0F0F0"); // Hover 灰
        thm.comboBoxSelecedColor = QColor("#3F7BFF"); // 强调蓝
        thm.spinBoxMaskReleasedColor = "#E0E0E0";     // 默认灰
        thm.spinBoxMaskPressedColor = "#CCCCCC";      // 按下深灰
        thm.spinBoxMaskFocusedColor = "#666666";      // 聚焦深一点
        thm.font = QFont("Arial", DEFAULT_FONT_SIZE);
        thm.borderRadius = DEFAULT_BORDER_RADIUS;
        thm.padding = DEFAULT_PADDING;
        return thm;
    }

    Theme Theme::Dark() {
        Theme thm;
        thm.backgroundColor = QColor("#121212"); // #121212 取值 #121212
        thm.textColor = QColor("#FFFFFF");       // 文本取值 #FFFFFF
        thm.borderColor = QColor("#333333");
        thm.normalPanelColor = QColor("#3D3D3D"); // #3D3D3D 取值 #3D3D3D
        thm.innerPanelColor = QColor("#303030");
        thm.contentPanelColor = QColor("#3D3D3D");
        thm.iconBtnBgColor = QColor("#545454");        // 比# 545454 稍微暗一点 取值 #545454
        thm.iconBtnHoverBgColor = QColor("#555555");   // 比# 545454 稍微亮一点 取值 #555555
        thm.iconBtnPressedBgColor = QColor("#5555FF"); // 亮蓝色取值 #5555FF
        thm.btnBgColor = QColor("#545454");            // 取值 #545454
        thm.btnFgColor = QColor("#FFFFFF");
        thm.tabBarPadding = "6px 10px";
        thm.tabBarMarginRight = "8px";
        thm.tabBarMarginBottom = "8px";
        thm.tabBarTabColor = QColor("#545454");
        thm.tabBarTabHoverColor = QColor("#656565");
        thm.tabBarTabPressedColor = QColor("#4772B3");
        thm.menuItemSelectedColor = QColor("#3F3F3F"); // 取值 #3F3F3F
        thm.cusMenuPadding = "4px";
        thm.menuPadding = "4px";
        thm.menuItemPadding = "6px 10px";
        thm.comboBoxBgColor = QColor("#1A1A1A");      // 取值 #1A1A1A
        thm.comboBoxHoverBgColor = QColor("#3F3F3F"); // 取值 #3F3F3F
        thm.comboBoxSelecedColor = QColor("#4772B3"); // 取值 #4772B3
        thm.spinBoxMaskReleasedColor = "#545454";     // 取值 #545454
        thm.spinBoxMaskPressedColor = "#222222";      // 取值 #222222
        thm.spinBoxMaskFocusedColor = "#797979";      // 取值 #797979
        thm.font = QFont("Arial", DEFAULT_FONT_SIZE);
        thm.borderRadius = DEFAULT_BORDER_RADIUS;
        thm.padding = DEFAULT_PADDING;
        return thm;
    }

    Theme Theme::Ocean() {
        Theme thm;
        thm.backgroundColor = QColor("#F5F9FF");
        thm.textColor = QColor("#2A4365");
        thm.borderColor = QColor("#90CDF4");
        thm.normalPanelColor = QColor("#E5F2FF");
        thm.innerPanelColor = QColor("#303030");
        thm.contentPanelColor = QColor("#3D3D3D");
        thm.iconBtnBgColor = QColor("#545454");        // 比# 545454 稍微暗一点 取值 #545454
        thm.iconBtnHoverBgColor = QColor("#555555");   // 比# 545454 稍微亮一点 取值 #555555
        thm.iconBtnPressedBgColor = QColor("#5555FF"); // 亮蓝色取值 #5555FF
        thm.btnBgColor = QColor("#545454");
        thm.btnFgColor = QColor("#555555");
        thm.tabBarPadding = "6px 10px";
        thm.tabBarMarginRight = "8px";
        thm.tabBarMarginBottom = "8px";
        thm.tabBarTabColor = QColor("#545454");
        thm.tabBarTabHoverColor = QColor("#656565");
        thm.tabBarTabPressedColor = QColor("#4772B3");
        thm.menuItemSelectedColor = QColor("#3F3F3F"); // 取值 #3F3F3F
        thm.cusMenuPadding = "4px";
        thm.menuPadding = "4px";
        thm.menuItemPadding = "6px 10px";
        thm.comboBoxBgColor = QColor("#1A1A1A");      // 取值 #1A1A1A
        thm.comboBoxHoverBgColor = QColor("#3F3F3F"); // 取值 #3F3F3F
        thm.comboBoxSelecedColor = QColor("#4772B3"); // 取值 #4772B3
        thm.spinBoxMaskReleasedColor = "#FFFFFF";
        thm.spinBoxMaskPressedColor = "#FFFFFF";
        thm.spinBoxMaskFocusedColor = "#FFFFFF";
        thm.font = QFont("Arial", DEFAULT_FONT_SIZE);
        thm.borderRadius = DEFAULT_BORDER_RADIUS;
        thm.padding = DEFAULT_PADDING;
        return thm;
    }

    Theme Theme::Sunny() {
        Theme thm;
        thm.backgroundColor = QColor("#FFF9E6");
        thm.textColor = QColor("#5F4B32");
        thm.borderColor = QColor("#F6AD55");
        thm.normalPanelColor = QColor("#FFEFD5");
        thm.contentPanelColor = QColor("#3D3D3D");
        thm.innerPanelColor = QColor("#303030");
        thm.iconBtnBgColor = QColor("#545454");        // 比# 545454 稍微暗一点 取值 #545454
        thm.iconBtnHoverBgColor = QColor("#555555");   // 比# 545454 稍微亮一点 取值 #555555
        thm.iconBtnPressedBgColor = QColor("#5555FF"); // 亮蓝色取值 #5555FF
        thm.btnBgColor = QColor("#545454");
        thm.btnFgColor = QColor("#555555");
        thm.tabBarPadding = "6px 10px";
        thm.tabBarMarginRight = "8px";
        thm.tabBarMarginBottom = "8px";
        thm.tabBarTabColor = QColor("#545454");
        thm.tabBarTabHoverColor = QColor("#656565");
        thm.tabBarTabPressedColor = QColor("#4772B3");
        thm.cusMenuPadding = "4px";
        thm.menuPadding = "4px";
        thm.menuItemPadding = "6px 10px";
        thm.comboBoxBgColor = QColor("#1A1A1A");      // 取值 #1A1A1A
        thm.comboBoxHoverBgColor = QColor("#3F3F3F"); // 取值 #3F3F3F
        thm.comboBoxSelecedColor = QColor("#4772B3"); // 取值 #4772B3
        thm.spinBoxMaskReleasedColor = "#FFFFFF";
        thm.spinBoxMaskPressedColor = "#FFFFFF";
        thm.spinBoxMaskFocusedColor = "#FFFFFF";
        thm.font = QFont("Noto Sans", DEFAULT_FONT_SIZE);
        thm.borderRadius = DEFAULT_BORDER_RADIUS;
        thm.padding = DEFAULT_PADDING;
        return thm;
    }

    /// --- theme管理implementation ---
    QHFThemeManager& QHFThemeManager::instance() {
        // 局部Static variable, 线程安全, autodestroy
        static QHFThemeManager instance;
        return instance;
    }

    QHFThemeManager::QHFThemeManager() {
        m_type = ThemeType::LIGHT;
        m_theme = Theme::Light();
    }

    void QHFThemeManager::setThemeType(ThemeType type) {
        if (m_type != type) {
            m_type = type;
            // updatecurrenttheme
            switch (m_type) {
            case ThemeType::LIGHT:
                m_theme = Theme::Light();
                break;
            case ThemeType::DARK:
                m_theme = Theme::Dark();
                break;
            case ThemeType::OCEAN:
                m_theme = Theme::Ocean();
                break;
            case ThemeType::SUNNY:
                m_theme = Theme::Sunny();
                break;
            default:
                break;
            }
            emit themeChanged();
        }
    }

    ThemeType QHFThemeManager::themeType() const
    {
        return m_type;
    }

    const Theme& QHFThemeManager::theme() const
    {
        return m_theme;
    }
} // namespace QHF
