#ifndef QHFMESSAGEBOX_H
#define QHFMESSAGEBOX_H

#include <QDialog>
#include <QFlags>
#include <QIcon>
#include <QLabel>
#include <QMap>
#include <QMouseEvent>
#include <QPushButton>
#include <QShowEvent>

namespace QHF {

    /**
     * @brief Custom (see description)
     * @details (see source).
     */
    class QHFMessageBox : public QDialog {
        Q_OBJECT

      public:
        /**
         * @brief  (see description)
         * @details (see source).
         * 使用非强制性位运算符定义按钮, 可以方便地set按钮.
         */
        enum QHFMsgBoxButton : std::uint8_t {
            NONE = 0x0,      ///< None按钮
            OK = 0x1,        ///< 确定按钮
            CANCEL= 0x2,    ///< 取消按钮
            YES= 0x4,       ///< is 按钮
            NO= 0x8,        ///< 否按钮
            RETRY= 0x10,    ///< 重试按钮
            CLOSE= 0x40,    ///< Close按钮
            IGNOREBTN = 0x80 ///< ignore button
        };

        // 定义按钮集合type
        Q_DECLARE_FLAGS(QHFMsgBoxButtons, QHFMsgBoxButton)

        /**
         * @brief  (see description)
         * @details (see source).
         * 图标可以set在消息框 left侧.使用非强制性位运算符定义图标, 可以方便地set图标。
         */
        enum QHFMsgBoxIcon : std::uint8_t {
            NICON, ///< None图标
            INFO,  ///< 信息icon
            WARN,///< 警告icon
            CRIT,///< 危险icon
            QUES ///< question icon
        };

      private:
        QHFMsgBoxIcon m_msgIcon;
        QString m_titleLabel;
        QString m_textLabel;
        QMap<QHFMsgBoxButton, QPushButton*> m_buttons;
        QHFMsgBoxButton m_clickedButton;
        // 拖动related
        bool m_isDragging;
        QPoint m_dragStartPosition;

      public:
        // static便捷interface
        static QHFMsgBoxButton information(QWidget* parent, const QString& title, const QString& text,
                                         QHFMsgBoxButtons buttons = QHFMsgBoxButton::OK,
                                         QHFMsgBoxButton defaultButton = QHFMsgBoxButton::NONE);
        static QHFMsgBoxButton warning(QWidget* parent, const QString& title, const QString& text,
                                     QHFMsgBoxButtons buttons = QHFMsgBoxButton::OK,
                                     QHFMsgBoxButton defaultButton = QHFMsgBoxButton::NONE);
        static QHFMsgBoxButton critical(QWidget* parent, const QString& title, const QString& text,
                                      QHFMsgBoxButtons buttons = QHFMsgBoxButton::OK,
                                      QHFMsgBoxButton defaultButton = QHFMsgBoxButton::NONE);
        static QHFMsgBoxButton question(QWidget* parent, const QString& title, const QString& text,
                                      QHFMsgBoxButtons buttons = QHFMsgBoxButtons(QHFMsgBoxButton::YES |
                                                                              QHFMsgBoxButton::NO),
                                      QHFMsgBoxButton defaultButton = QHFMsgBoxButton::NONE);

        /**
         * @brief Constructor
         * @details (see source).
         * @param title title* @param icon icon* @param text 消息content* @param buttons 按钮集合* @param parent Parent window
         */
        QHFMessageBox(QString title, QHFMsgBoxIcon icon, QString text,
                    QHFMsgBoxButtons buttons = QHFMsgBoxButton::OK, QWidget* parent = nullptr);

        /**
         * @brief  (see description)
         * @details (see source).
         * @param button 按钮*/
        void addButton(QHFMsgBoxButtons button);

        /**
         * @brief  (see description)
         * @details (see source).
         * @param text 按钮text* @param button 按钮*/
        void addButton(const QString& text, QHFMsgBoxButton button);

        /**
         * @brief  (see description)
         * @details (see source).
         * @param button 按钮*/
        void setDefaultButton(QHFMsgBoxButton button);

        /**
         * @brief  (see description)
         * @details (see source).
         */
        [[nodiscard]] QHFMessageBox::QHFMsgBoxButton result() const;

        /**
         * @brief  (see description)
         * @details (see source).
         * @param button 按钮* @param text 按钮text*/
        void setButtonText(QHFMessageBox::QHFMsgBoxButton button, const QString& text);

      protected:
        /**
         * @brief  (see description)
         * @details (see source).
         */
        void showEvent(QShowEvent* event) override;

        /**
         * @brief  (see description)
         * @details (see source).
         * @param event 鼠标event*/
        void mousePressEvent(QMouseEvent* event) override;

        /**
         * @brief  (see description)
         * @details (see source).
         * @param event 鼠标event*/
        void mouseMoveEvent(QMouseEvent* event) override;

        /**
         * @brief  (see description)
         * @details (see source).
         * @param event 鼠标event*/
        void mouseReleaseEvent(QMouseEvent* event) override;

      private:
        /**
         * @brief Initializes (see description)
         * @details (see source).
         */
        void initialize();

        /**
         * @brief Applies theme
         * @details (see source).
         */
        void applyTheme();

        /**
         * @brief  (see description)
         * @details (see source)
         * @param icon icontype* @return icon标签*/
        [[nodiscard]] QWidget* createIcon() const;

        /**
         * @brief  (see description)
         * @details (see source).
         * @param button 按钮枚举value* @return 按钮text*/
        static QString buttonText(QHFMsgBoxButton button);

        /// --- event处理 ---
        /**
         * @brief  (see description)
         * @details (see source).
         * 当用户点击消息框中 按钮时, 将trigger此event.
         * @note signalconnection到 onButtonClicked 函数。
         */
        void onButtonClicked();
    };
} // namespace QHF

// 定义 Q_DECLARE_OPERATORS_FOR_FLAGS 宏, 使得 QHFMsgBoxButtons 支持位运算符
Q_DECLARE_OPERATORS_FOR_FLAGS(QHF::QHFMessageBox::QHFMsgBoxButtons)

#endif // HMESSAGEBOX_H
