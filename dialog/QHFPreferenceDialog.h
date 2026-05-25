#ifndef QHFPREFERENCEDIALOG_H
#define QHFPREFERENCEDIALOG_H

#include <QDialog>
#include <QList>
#include <QMenu>
#include <QStackedWidget>
#include <QTabBar>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>

#include "QHFUIIPreferencePanel.h"
#include "QHFUIUtils.h"

namespace QHF {

    /**
     * @brief Preferences (see description)
     * @details (see source)
     */
    class QHFPreferDialogMenu : public QToolButton, public QHFThemedCRTP<QHFPreferDialogMenu> {
        Q_OBJECT

      private:
        QMenu* m_menu; ///< menupointer

      signals:
        void evtSaveUserSettingsClicked();
        void evtResetDefaultSettingsClicked();

      public:/**
         * @brief Constructor
         * @param parent Parent object
         */
        QHFPreferDialogMenu(QWidget* parent = nullptr);
        ~QHFPreferDialogMenu() override = default; ///< DefaultDestructor

      private:
        /**
         * @brief Interface (see description)
         */
        void applyTheme();

        /**
         * @brief Initializes (see description)
         */
        void initialize();
    };

    /**
     * @brief Preferences (see description)
     * @details (see source)
     */
    class QHFPreferenceDialog : public QDialog, public QHFThemedCRTP<QHFPreferenceDialog> {
        Q_OBJECT

      private:
        QList<Setting>          m_settings;         // 保存 配置
        QList<Setting>          m_originalSettings; // Initializes配置
        QVBoxLayout*            m_mainLayout;
        QTabBar*                m_tabbar;
        QStackedWidget*         m_stackedWidget;
        QMap<QString, QWidget*> m_panelMap;

      public:
        /**
         * @brief Constructor
         * @param parent Parent object
         */
        explicit QHFPreferenceDialog(QWidget* parent = nullptr);
        ~QHFPreferenceDialog() override = default;

      protected:
        /**
         * @brief  (see description)
         * @param event 绘制eventpointer*/
        void paintEvent(QPaintEvent* event) override;

        /**
         * @brief Slot function,  (see description)
         * @details (see source)
         */
        void onCancel();

        /**
         * @brief Slot function,  (see description)
         * @details (see source)
         */
        void onSaveUserSettings();

        /**
         * @brief Slot function,  (see description)
         * @details (see source)
         */
        void onResetDefaultSettings();

      private:
        /**
         * @brief Applies theme
         * @details (see source)
         */
        void applyTheme();

        /**
         * @brief Initializes (see description)
         */
        void initialize();

        /**
         * @brief  (see description)
         * @return panelinfolist*/
        void collectPanelNames();
    };

} // namespace QHF

#endif // HPREFERENCEDIALOG_H
