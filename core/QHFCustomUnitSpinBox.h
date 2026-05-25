#ifndef QHFCUSTOMUNITSPINBOX_H
#define QHFCUSTOMUNITSPINBOX_H

#include "QHFUIUtils.h"

#include <QColor>
#include <QEvent>
#include <QFocusEvent>
#include <QLabel>
#include <QLineEdit>
#include <QPoint>
#include <QPushButton>
#include <QTimer>
#include <QWidget>

namespace QHF {
    /**
     * @brief QHFCustomUnitSpinBox  (see description)
     */
    class QHFCustomUnitSpinBox : public QWidget, public QHFThemedCRTP<QHFCustomUnitSpinBox> {
        Q_OBJECT

      private:
        QLabel* m_maskLabel;             ///< 遮罩标签
        QString m_maskReleasedColor;///< 遮罩标签释放color
        QString m_maskPressedColor;///< 遮罩标签编辑color
        QString m_maskFocusedColor;///< 遮罩标签焦点color
        QWidget* m_showMask;             ///< 现实标签
        QLineEdit* m_lineEdit;           ///< 数value输入框
        QString m_unitLabel;///< 单位标签
        QPushButton* m_incButton;        ///< 增加按钮
        QPushButton* m_decButton;        ///< 减少按钮
        float m_lastValidValue;///< 用来Save 上一个有效 数value
        bool m_isEditMode;///< is 否正at 编辑
        bool m_isDragging;///< is 否正at 拖动
        QPoint m_dragStartPos;///< 拖动开始position
        QPoint m_dragLastPos;///< 拖动上一次position
        int m_incFactor;///< 增加倍数
        float m_incStep;///< 增加步长
        bool m_isBlockSignal;///< is 否阻塞signal
        QTimer* m_mouseWheelStopedTimer; ///< 定时器, 用于延迟数valueUpdate 

      public:
        explicit QHFCustomUnitSpinBox(QWidget* parent = nullptr);
        ~QHFCustomUnitSpinBox() override = default;

        /**
         * @brief  (see description)
         * @param factor 倍数*/
        void setIncFactor(int factor);

        /**
         * @brief Returns current  (see description)
         * @return 倍数*/
        [[nodiscard]] int getIncFactor() const;

        /**
         * @brief  (see description)
         * @param step float 增加步长
         */
        void setIncStep(float _step);

        /**
         * @brief  (see description)
         */
        [[nodiscard]] float getIncStep() const;

        /**
         * @brief  (see description)
         * @param value
         */
        void setFloatValueWithoutSignal(float value);

        /**
         * @brief  (see description)
         * @return
         */
        [[nodiscard]] float getFloatValue() const;

      signals:
        /**
         * @brief  (see description)
         * @param value 输入数value*/
        void eventUserInput(float _value);

      protected:
        /**
         * @brief Event (see description)
         * @param obj eventobject* @param event event* @return bool, 真valueRepresents event已被处理, 否则未被处理
         */
        bool eventFilter(QObject* obj, QEvent* event) override;

        /**
         * @brief  (see description)
         * @param event 进入eventpointer*/
        void enterEvent(QEvent* event) override;

        /**
         * @brief  (see description)
         * @param event 离开eventpointer*/
        void leaveEvent(QEvent* event) override;

        /**
         * @brief  (see description)
         * @param event 失去焦点eventpointer*/
        void focusOutEvent(QFocusEvent* event) override;

        /**
         * @brief  (see description)
         * @param event 绘制eventpointer*/
        void paintEvent(QPaintEvent* event) override;

        /**
         * @brief Applies theme
         */
        void applyTheme();

      private:
        /**
         * @brief Initializes
         */
        void initialize();

        /**
         * @brief Slot function,  (see description)
         */
        void onIncreaseValue();

        /**
         * @brief Slot function,  (see description)
         */
        void onDecreaseValue();

        /**
         * @brief Slot function,  (see description)
         * @param valueText 输入框value*/
        void onTextChanged(const QString& valueText);

        /**
         * @brief  (see description)
         */
        void enterEditMode();

        /**
         * @brief  (see description)
         */
        void leaveEditMode();

        /**
         * @brief  (see description)
         */
        void dynamicMaskColor(const QString& color);
    };
} // namespace QHF

#endif // HCUSTOMUNITSPINBOX_H
