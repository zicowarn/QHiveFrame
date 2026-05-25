#ifndef QHFAPPSTATUSBAR_H
#define QHFAPPSTATUSBAR_H

#include <QHBoxLayout>
#include <QLabel>
#include <QStatusBar>

#include "QHFGuiNotifierCenter.h"
#include "QHFUIUtils.h"

namespace QHF {
    /**
     * @brief State (see description)
     * @details (see source)
     * @note status bar Layout为: 提示info (可变)、stateinfo (固定)、Version Info (固定)
     */
    class QHFAppStatusBar : public QStatusBar, public QHFThemedCRTP<QHFAppStatusBar> {
        Q_OBJECT

      private:
        QWidget*     m_promptContainer; ///< 提示Container
        QHBoxLayout* m_promptLayout;    ///< 提示Layout
        QLabel*      m_statsLabel;      ///< state标签
        QLabel*      m_versionLabel;    ///< 版本标签

      public:/**
         * @brief Constructor
         * @param parent Parent object
         */
        explicit QHFAppStatusBar(QWidget* parent = nullptr);
        ~QHFAppStatusBar() override = default; ///< Destructor
/**
 * @brief (see description)
         * @param prompt 提示info*/
        void updatePrompts(const QList<QPair<QIcon, QString>>& prompts);

        /**
         * @brief  (see description)
         * @param stats stateinfo*/
        void updateStats(const QString& stats);

      private:
        /**
         * @brief Initializes (see description)
         */
        void initialize();

        /**
         * @brief Applies theme
         * @details (see source)
         */
        void applyTheme();

        /**
         * @brief  (see description)
         */
        void handleNotify(const QHFNotifier& n);
    };

} // namespace QHF

#endif // HAPPSTATUSBAR_H
