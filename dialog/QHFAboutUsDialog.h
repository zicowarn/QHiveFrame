#ifndef QHFABOUTUSDIALOG_H
#define QHFABOUTUSDIALOG_H

#include "QHFUIUtils.h"

#include <QDialog>

namespace QHF {

    /**
     * @brief About (see description)
     * @details (see source)
     */
    class QHFAboutUsDialog : public QDialog, public QHFThemedCRTP<QHFAboutUsDialog> {
        Q_OBJECT

      public:
        /**
         * @brief Constructor
         * @param parent Parent objectpointer
         */
        explicit QHFAboutUsDialog(QWidget* parent = nullptr);
        ~QHFAboutUsDialog() override = default; ///< Destructor

      protected:
        /**
         * @brief  (see description)
         */
        void onCloseButtonClicked();

      private:
        /**
         * @brief  (see description)
         * @param event 绘制eventpointer*/
        void paintEvent(QPaintEvent* event) override;

        /**
         * @brief  (see description)
         * @details (see source)
         */
        void applyTheme();

        /**
         * @brief Initializes (see description)
         */
        void initialize();
    };
} // namespace QHF

#endif // HABOUTUSDIALOG_H
