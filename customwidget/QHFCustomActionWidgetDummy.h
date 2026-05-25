#ifndef QHFCUSTOMACTIONWIDGETDUMMY_H
#define QHFCUSTOMACTIONWIDGETDUMMY_H

#include <QVBoxLayout>

#include "QHFCustomActionWidgetBase.h"

namespace QHF {
    class QHFCustomActionWidgetDummy : public QHFCustomActionWidgetBase {
        Q_OBJECT

      private:
        QVBoxLayout* m_layout;

      public:
        explicit QHFCustomActionWidgetDummy(ActionType type, QWidget* parent = nullptr);
        void                      updateContext() override;
        [[nodiscard]] QVariantMap getData() const override;
        [[nodiscard]] ActionType  getActionType() const override;

      private:
        /**
         * @brief Applies theme
         * @details Applies theme
         */
        void applyTheme() override;

        /**
         * @brief Initializes (see description)
         * @details (see source) Initializes
         */
        void initializeImpl() override;
    };
} // namespace QHF

#endif // HCUSTOMACTIONWIDGETDUMMY_H
