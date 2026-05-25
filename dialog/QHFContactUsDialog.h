#ifndef QHFCONTACTUSDIALOG_H
#define QHFCONTACTUSDIALOG_H

#include "QHFUIUtils.h"

#include <QDialog>

namespace QHF {

    /**
     * @brief Contact Us (see description)
     * @details (see source)
     */
    class QHFContactUsDialog : public QDialog, public QHFThemedCRTP<QHFContactUsDialog> {
        Q_OBJECT

      public:
        /**
         * @brief Constructor
         * @param parent Parent objectpointer
         */
        explicit QHFContactUsDialog(QWidget* parent = nullptr);
        ~QHFContactUsDialog() override = default; ///< Destructor

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

#endif // HCONTACTUSDIALOG_H
