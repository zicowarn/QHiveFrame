#ifndef QHFCOLLAPSIBLESECTION_H
#define QHFCOLLAPSIBLESECTION_H

#include "QHFUIUtils.h"

#include <QFrame>
#include <QGridLayout>
#include <QParallelAnimationGroup>
#include <QScrollArea>
#include <QToolButton>
#include <QWidget>

namespace QHF {
    /**
     * @brief QHFCollapsibleSection  (see description)
     */
    class QHFCollapsibleSection : public QWidget, public QHFThemedCRTP<QHFCollapsibleSection> {
        Q_OBJECT

      public:
        static const int DEFAULT_DURATION = 0; ///< Default动画持续时间

      private:
        QString m_title;                            ///< 折叠区域title
        QGridLayout* m_mainLayout;                  ///< 主Layout
        QToolButton* m_toggleButton;                ///< 切换按钮
        QFrame* m_headerLine;                       ///< title线
        QParallelAnimationGroup* m_toggleAnimation; ///< 切换动画
        QScrollArea* m_contentArea;                 ///< 内容区域
        int m_animationDuration;///< 动画持续时间
        int m_collapsedHeight;///< 折叠高度
        bool m_isExpanded;///< is 否展开

      public:/**
         * @brief Constructor
         * @param title 折叠areatitle* @param m_animationDuration  动画持续时间* @param parent Parent object
         */
        explicit QHFCollapsibleSection(QString title = "", int animationDuration = DEFAULT_DURATION,
                                     QWidget* parent = nullptr);

        /**
         * @brief  (see description)
         * @param contentarea Layout
         */
        void setContentLayout(QLayout& contentLayout);

        /**
         * @brief  (see description)
         * @param title titletext*/
        void setTitle(const QString& title);

        /**
         * @brief  (see description)
         */
        void updateHeights();

        /**
         * @brief  (see description)
         * @param collapsed 折叠state*/
        void toggle(bool collapsed);

      protected:
        /**
         * @brief  (see description)
         * @param event 绘制eventpointer*/
        void paintEvent(QPaintEvent* event) override;

        /**
         * @brief  (see description)
         * @details (see source)
         */
        void applyTheme();

      private:
        /**
         * @brief Initializes
         */
        void initialize();
    };

} // namespace QHF

#endif // HCOLLAPSIBLESECTION_H
