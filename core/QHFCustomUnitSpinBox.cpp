#include "QHFCustomUnitSpinBox.h"

#include "QHFThemeManager.h"

#include <QDebug>
#include <QDoubleValidator>
#include <QEvent>
#include <QHBoxLayout>
#include <QPainter>
#include <QPropertyAnimation>
#include <QStyleOption>
#include <QVBoxLayout>

namespace QHF {

    static constexpr int InitWidth = 18;       ///< Default按钮宽度
    static constexpr int InitHight = 18;       ///< Default按钮高度
    static constexpr float InputStep = 0.001f; ///< Default输入框步长

    /**
     * QHFCustomUnitSpinBox implementation代码
     */
    QHFCustomUnitSpinBox::QHFCustomUnitSpinBox(QWidget* parent) : QWidget(parent) {
        // InitializesParameter
        m_maskLabel = nullptr;
        m_maskReleasedColor = "";
        m_maskPressedColor = "";
        m_maskFocusedColor = "";
        m_showMask = nullptr;
        m_lineEdit = nullptr;
        m_unitLabel = "";
        m_incButton = nullptr;
        m_decButton = nullptr;
        m_lastValidValue = 0.0f;
        m_isEditMode = false;
        m_isDragging = false;
        m_dragStartPos = QPoint(0, 0);
        m_dragLastPos = QPoint(0, 0);
        m_incFactor = 1;
        m_incStep = InputStep;
        m_isBlockSignal = false;
        m_mouseWheelStopedTimer = nullptr;

        // setstyle, 圆角border
        setObjectName("QHFCustomUnitSpinBox");
        // setbackground为透明
        setAttribute(Qt::WA_StyledBackground, true);
        // InitializesTheme manager
        initTheme(QHFThemeManager::instance(), &QHFThemeManager::themeChanged);
        // Initializes
        initialize();
        // 接受theme
        applyTheme();
    }

    void QHFCustomUnitSpinBox::setIncFactor(int _factor) { m_incFactor = _factor; }

    int QHFCustomUnitSpinBox::getIncFactor() const
    {
        return m_incFactor;
    }

    void QHFCustomUnitSpinBox::setIncStep(float _step) { m_incStep = _step; }

    float QHFCustomUnitSpinBox::getIncStep() const
    {
        return m_incStep;
    }

    void QHFCustomUnitSpinBox::setFloatValueWithoutSignal(float _value) {
        blockSignals(true);
        m_isBlockSignal = true;
        // 小数点after保留3位 check数value 如果小于。0.001 则显示为 0
        if (std::abs(_value) < InputStep) {
            m_lineEdit->setText("0");
            m_maskLabel->setText("0");
        } else {
            m_lineEdit->setText(QString::number(_value, 'f', 3));
            m_maskLabel->setText(QString::number(_value, 'f', 3));
        }
        blockSignals(false);
        m_isBlockSignal = false;
    }

    float QHFCustomUnitSpinBox::getFloatValue() const
    {
        return m_lineEdit->text().toFloat();
    }

    /// NOLINTNEXTLINE(readability-function-cognitive-complexity)
    bool QHFCustomUnitSpinBox::eventFilter(QObject* obj, QEvent* event) {
        if (obj == m_lineEdit && event->type() == QEvent::FocusOut) {
            // 失去焦点 离开编辑mode
            if (m_isEditMode) {
                leaveEditMode();
            }
        }

        // 鼠标left键双击
        if (obj == m_lineEdit && event->type() == QEvent::MouseButtonDblClick) {
            // 转换成鼠标event
            auto* mouseEvent = static_cast<QMouseEvent*>(event);
            // 是否鼠标left键点击
            if (mouseEvent->button() == Qt::LeftButton) {
                if (m_isEditMode) {
                    // 选择全部
                    m_lineEdit->selectAll();
                }
            }
        }

        // 鼠标释放
        if (obj == m_lineEdit && event->type() == QEvent::MouseButtonRelease) {
            // 转换成鼠标event
            auto* mouseEvent = static_cast<QMouseEvent*>(event);
            // 鼠标right键 释放 隐藏menu
            if (mouseEvent->button() == Qt::RightButton) {
                if (m_isEditMode) {
                    // 进入编辑mode
                    leaveEditMode();
                }
            }
        }

        // 鼠标滚轮
        if (obj == m_maskLabel && event->type() == QEvent::Wheel) {
            // 转换成滚轮event
            auto* wheelEvent = static_cast<QWheelEvent*>(event);
            // 鼠标滚轮 stop 则认为滚动stop
            if (wheelEvent->angleDelta().y() > 0) {
                // 向top滚动
                onDecreaseValue();
            } else if (wheelEvent->angleDelta().y() < 0) {
                // 向bottom滚动
                onIncreaseValue();
            }
            // set鼠标光标 为topbottom放大
            m_maskLabel->setCursor(Qt::SizeVerCursor);
            // setlabel 为白色
            dynamicMaskColor(m_maskReleasedColor);
            // 重设Timer
            /// NOLINTNEXTLINE(readability-magic-numbers)
            m_mouseWheelStopedTimer->start(200); // 200ms 无滚轮事件则认为滚动停止
            // 阻止event传递
            return true;
        }

        if (obj == m_maskLabel && event->type() == QEvent::MouseButtonPress) {
            // 转换成鼠标event
            auto* mouseEvent = static_cast<QMouseEvent*>(event);
            // 是否鼠标left键点击
            if (mouseEvent->button() == Qt::LeftButton) {
                if (!m_isEditMode) {
                    // 记录鼠标x 坐标
                    m_dragStartPos = mouseEvent->pos();
                    m_dragLastPos = mouseEvent->pos();
                }
            }
        }

        if (obj == m_maskLabel && event->type() == QEvent::MouseMove) {
            // 转换成鼠标event
            auto* mouseEvent = static_cast<QMouseEvent*>(event);
            // 是否鼠标left键点击
            if (mouseEvent->buttons().testFlag(Qt::LeftButton)) {
                if (!m_isDragging) {
                    // 开始拖动
                    m_isDragging = true;
                    // 隐藏光标
                    m_maskLabel->setCursor(Qt::BlankCursor);
                    // set label
                    dynamicMaskColor(m_maskReleasedColor);
                }
                // mouseEvent->y(); 计算偏移量
                int offsetX = mouseEvent->x() - m_dragLastPos.x();
                // save
                m_dragLastPos = mouseEvent->pos();
                // qDebug() << "offsetX: " << offsetX;
                //
                if (offsetX < 0) {
                    // 向left滑动, 减小value
                    onDecreaseValue();
                } else if (offsetX > 0) {
                    // 向right滑动, 增加value
                    onIncreaseValue();

                } else {
                    // 鼠标到达x轴边界时, reset鼠标位置
                    m_dragLastPos = m_dragStartPos;
                }
                // 移动光标到current位置
                QCursor::setPos(m_maskLabel->mapToGlobal(m_dragLastPos));
            }
        }

        if (obj == m_maskLabel && event->type() == QEvent::MouseButtonRelease) {
            // 转换成鼠标event
            auto* mouseEvent = static_cast<QMouseEvent*>(event);
            // 是否鼠标left键点击
            if (mouseEvent->button() == Qt::LeftButton) {
                if (!m_isDragging && !m_isEditMode) {
                    // 进入编辑mode
                    enterEditMode();
                } else if (m_isDragging) {
                    // 松开鼠标
                    m_isDragging = false;
                    // 恢复default cursor
                    m_maskLabel->unsetCursor();
                    // 恢复default label background
                    dynamicMaskColor(m_maskReleasedColor);
                    // 恢复鼠标到开始位置
                    QCursor::setPos(m_maskLabel->mapToGlobal(m_dragStartPos));
                }
            }
        }
        return QWidget::eventFilter(obj, event);
    }

    void QHFCustomUnitSpinBox::enterEvent(QEvent* event) {
        // sethorizontalleftright箭header光标
        m_maskLabel->setCursor(Qt::SizeHorCursor);
        // 鼠标进入时显示按钮
        if (!m_isEditMode && m_showMask->isVisible()) {
            m_incButton->setVisible(true);
            m_decButton->setVisible(true);
        }
        QWidget::enterEvent(event);
    }

    void QHFCustomUnitSpinBox::leaveEvent(QEvent* event) {
        //
        m_maskLabel->unsetCursor();
        // 鼠标离开时隐藏按钮
        if (!m_isEditMode && m_showMask->isVisible()) {
            m_incButton->setVisible(false);
            m_decButton->setVisible(false);
        }
        QWidget::leaveEvent(event);
    }

    void QHFCustomUnitSpinBox::focusOutEvent(QFocusEvent* event) {
        if (m_isEditMode && !m_showMask->isVisible()) {
            leaveEditMode();
        }
        // 父类event处理
        QWidget::focusOutEvent(event);
    }

    void QHFCustomUnitSpinBox::paintEvent(QPaintEvent* event) {
        // 只调用base class  paintEvent, 让style表生效
        QStyleOption opt;
        opt.initFrom(this);
        QPainter painter(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
        QWidget::paintEvent(event);
    }

    void QHFCustomUnitSpinBox::applyTheme() {
        // Theme
        const auto& theme = QHFThemeManager::instance().theme();

        // Set color
        m_maskReleasedColor = theme.spinBoxMaskReleasedColor;
        m_maskPressedColor = theme.spinBoxMaskPressedColor;
        m_maskFocusedColor = theme.spinBoxMaskFocusedColor;

        QString maskStyle = QString(R"(
            QLabel#QHFCustomUnitSpinBoxMaskLabel {background-color: %1;}
            )")
                                .arg(m_maskReleasedColor);

        // rander button
        QString randBtnStyle = QString(R"(
            QPushButton#QHFCustomUnitSpinBoxButtonLeft {
                font-size: 16px;
                font-weight: bold;
                border: 1px solid #4CAF50;  /* 设置边框 */
                border-radius: 3px;         /* 设置圆角 (全部角) */
            }

            QPushButton#QHFCustomUnitSpinBoxButtonLeft::hover {
                background-color: #87BFFF;   /* 鼠标悬停时 背景颜色 */
            }

            /* 设置右上和右下角为0, 左上和左下角为圆角 */
            QPushButton#QHFCustomUnitSpinBoxButtonLeft {
                border-top-right-radius: 0px;
                border-bottom-right-radius: 0px;
            }

            QPushButton#QHFCustomUnitSpinBoxButtonRight {
                font-size: 16px;
                font-weight: bold;
                border: 1px solid #4CAF50;  /* 设置边框 */
                border-radius: 3px;         /* 设置圆角 (全部角) */
            }

            QPushButton#QHFCustomUnitSpinBoxButtonRight::hover {
                background-color: #87BFFF;   /* 鼠标悬停时 背景颜色 */
            }

            /* 设置右上和右下角为0, 左上和左下角为圆角 */
            QPushButton#QHFCustomUnitSpinBoxButtonRight {
                border-top-left-radius: 0px;
                border-bottom-left-radius: 0px;
            }

        )");

        setStyleSheet(maskStyle + randBtnStyle);
    }

    void QHFCustomUnitSpinBox::initialize() {
        // setdefault高度
        setFixedHeight(InitHight);

        auto* mainLayout = new QVBoxLayout(this);
        mainLayout->setSpacing(0);                  // 去除控件之间 空隙
        mainLayout->setContentsMargins(0, 0, 0, 0); // 去除边框

        // Initializeswidget
        m_lineEdit = new QLineEdit("0", this);
        /// NOLINTNEXTLINE(readability-magic-numbers)
        m_lineEdit->setValidator(new QDoubleValidator(-1000.0, 1000.0, 3, this)); // 限制为整数数值
        m_lineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        m_lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        m_lineEdit->setAlignment(Qt::AlignLeft);
        m_lineEdit->setVisible(false);
        mainLayout->addWidget(m_lineEdit);

        m_showMask = new QWidget(this);
        m_showMask->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        mainLayout->addWidget(m_showMask);

        setLayout(mainLayout);

        // m_unitLabel = new QLabel(" m", this);  // Unit标签
        m_maskLabel = new QLabel("0");
        m_maskLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        m_maskLabel->setAlignment(Qt::AlignCenter);
        // background灰色
        m_maskLabel->setObjectName("QHFCustomUnitSpinBoxMaskLabel");

        // setbackgroundcolor
        m_decButton = new QPushButton("<");
        m_decButton->setToolTip(tr("Decrease"));
        m_decButton->setFixedWidth(InitWidth);
        m_decButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        // set按钮初始为隐藏
        m_decButton->setVisible(false);
        // righttop和rightbottom border 角度为0
        m_decButton->setObjectName("QHFCustomUnitSpinBoxButtonLeft");

        // light blue hex
        // #4CAF50

        m_incButton = new QPushButton(">");
        m_incButton->setToolTip(tr("Increase"));
        m_incButton->setFixedWidth(InitWidth);
        m_incButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        // set按钮初始为隐藏
        m_incButton->setVisible(false);
        // righttop和rightbottom border 角度为0
        m_incButton->setObjectName("QHFCustomUnitSpinBoxButtonRight");
        // Layout
        auto* showMaskLayout = new QHBoxLayout();
        showMaskLayout->addWidget(m_decButton, 1);
        /// NOLINTNEXTLINE(readability-magic-numbers)
        showMaskLayout->addWidget(m_maskLabel, 8);
        showMaskLayout->addWidget(m_incButton, 1);
        showMaskLayout->setSpacing(0);                  // 去除控件之间 空隙
        showMaskLayout->setContentsMargins(0, 0, 0, 0); // 去除边框

        m_showMask->setLayout(showMaskLayout);

        // Signal-slotconnection
        connect(m_incButton, &QPushButton::pressed,
                [&]() { dynamicMaskColor(m_maskPressedColor); });
        connect(m_decButton, &QPushButton::pressed,
                [&]() { dynamicMaskColor(m_maskPressedColor); });
        connect(m_incButton, &QPushButton::released,
                [&]() { dynamicMaskColor(m_maskReleasedColor); });
        connect(m_decButton, &QPushButton::released,
                [&]() { dynamicMaskColor(m_maskReleasedColor); });
        connect(m_incButton, &QPushButton::clicked, this, &QHFCustomUnitSpinBox::onIncreaseValue);
        connect(m_decButton, &QPushButton::clicked, this, &QHFCustomUnitSpinBox::onDecreaseValue);
        connect(m_lineEdit, &QLineEdit::textChanged, this, &QHFCustomUnitSpinBox::onTextChanged);
        connect(m_lineEdit, &QLineEdit::editingFinished, this, &QHFCustomUnitSpinBox::leaveEditMode);

        // Initializes定时器
        m_mouseWheelStopedTimer = new QTimer(this);
        /// NOLINTNEXTLINE(readability-magic-numbers)
        m_mouseWheelStopedTimer->setInterval(200); // 200ms 无滚轮事件则认为滚动停止
        m_mouseWheelStopedTimer->setSingleShot(true);

        // 监听timer 结束
        connect(m_mouseWheelStopedTimer, &QTimer::timeout, [&]() {
            // set鼠标光标为 leftright 放大
            m_maskLabel->setCursor(Qt::SizeHorCursor);
            // resetlabel background
            dynamicMaskColor(m_maskFocusedColor);
        });

        m_maskLabel->installEventFilter(this);
        m_lineEdit->installEventFilter(this);
    }

    void QHFCustomUnitSpinBox::onIncreaseValue() {
        if (m_incFactor < 2) {
            float currentValue = m_lineEdit->text().toFloat();
            currentValue += m_incStep;
            m_lineEdit->setText(QString::number(currentValue));
            m_maskLabel->setText(QString::number(currentValue));
        } else {
            for (int i = 0; i < m_incFactor; ++i) {
                float currentValue = m_lineEdit->text().toFloat();
                currentValue += m_incStep;
                m_lineEdit->setText(QString::number(currentValue));
                m_maskLabel->setText(QString::number(currentValue));
            }
        }
    }

    void QHFCustomUnitSpinBox::onDecreaseValue() {
        if (m_incFactor < 2) {
            float currentValue = m_lineEdit->text().toFloat();
            currentValue -= m_incStep;
            m_lineEdit->setText(QString::number(currentValue));
            m_maskLabel->setText(QString::number(currentValue));
        } else {
            for (int i = 0; i < m_incFactor; ++i) {
                float currentValue = m_lineEdit->text().toFloat();
                currentValue -= m_incStep;
                m_lineEdit->setText(QString::number(currentValue));
                m_maskLabel->setText(QString::number(currentValue));
            }
        }
    }

    void QHFCustomUnitSpinBox::onTextChanged(const QString& valueText) {
        // 输入框content改变时, 移除单位after校验数value部分
        if (m_isBlockSignal) {
            return;
        }
        // qDebug() << "onTextChanged:" << text
        bool okay;
        float value = valueText.toFloat(&okay);
        if (okay) {
            // 如果是valid 数value, update
            m_isBlockSignal = true;
            /// NOLINTNEXTLINE(readability-magic-numbers)
            if (std::abs(value) < 0.001) {
                m_lineEdit->setText("0");
                m_maskLabel->setText("0");
            } else {
                m_lineEdit->setText(QString::number(value, 'f', 3));
                m_maskLabel->setText(QString::number(value, 'f', 3));
            }
            m_isBlockSignal = false;
            //
            emit eventUserInput(value);
            // 记录top一个validvalue
            m_lastValidValue = value;
        } else {
            // 如果不是valid 数value, 恢复到top一个validvalue
            m_isBlockSignal = true;
            if (m_lastValidValue == 0) {
                m_lineEdit->setText("0");
                m_maskLabel->setText("0");
            } else {
                m_lineEdit->setText(QString::number(m_lastValidValue, 'f', 3));
                m_maskLabel->setText(QString::number(m_lastValidValue, 'f', 3));
            }
            m_isBlockSignal = false;
        }
    }

    void QHFCustomUnitSpinBox::enterEditMode() {
        // 进入编辑mode
        m_isEditMode = true;
        m_isBlockSignal = true;
        m_lineEdit->setVisible(true);
        m_lineEdit->setFocus();
        m_lineEdit->selectAll();
        m_showMask->setVisible(false);
        m_incButton->setVisible(false);
        m_decButton->setVisible(false);
    }

    void QHFCustomUnitSpinBox::leaveEditMode() {
        // 离开编辑mode
        if (m_isEditMode) {
            QString valueText = m_lineEdit->text();
            m_isBlockSignal = false;
            onTextChanged(valueText);
        }
        m_isEditMode = false;
        m_isBlockSignal = false;
        m_lineEdit->setVisible(false);
        m_showMask->setVisible(true);
    }

    void QHFCustomUnitSpinBox::dynamicMaskColor(const QString& color) {
        m_maskLabel->setStyleSheet(
            "QLabel#QHFCustomUnitSpinBoxMaskLabel {background-color: " + color + ";}");
    }
} // namespace QHF
