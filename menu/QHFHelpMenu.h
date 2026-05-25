#ifndef QHFHELPMENU_H
#define QHFHELPMENU_H

#include "QHFMenuBase.h"

namespace QHF {

    /**
     * @brief  (see description)
     */
    class QHFHelpMenu : public QHFMenuBase {
        Q_OBJECT

      public:
        static constexpr int SUGGESTED_ORDER = 4000;

        /**
         * @brief Constructor
         * @param parent Parent object
         */
        explicit QHFHelpMenu(QObject* parent = nullptr);

        /**
         * @brief  (see description)
         * @return menuname*/
        [[nodiscard]] QString menuName() const override;

        /**
         * @brief  (see description)
         * @return menu显示name*/
        [[nodiscard]] QString transName() const override;

      private:
        /**
         * @brief Initializes (see description)
         * @note 该函数由框架auto调用, 无需手动调用
         */
        void initializeImpl() override; // Initializes菜单项
    };

} // namespace QHF

#endif // HHELPMENU_H
