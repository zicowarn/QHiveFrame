#ifndef QHFEDITMENU_H
#define QHFEDITMENU_H

#include "QHFMenuBase.h"

namespace QHF {

    /**
     * @brief Editor (see description)
     */
    class QHFEditMenu : public QHFMenuBase {
        Q_OBJECT

      public:
        static constexpr int SUGGESTED_ORDER = 2000;

        /**
         * @brief Constructor
         * @param parent Parent object
         */
        explicit QHFEditMenu(QObject* parent = nullptr);

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

#endif // HEDITMENU_H
