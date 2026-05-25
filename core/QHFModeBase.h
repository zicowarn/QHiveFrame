// QHFModeBase.h
#ifndef QHFMODEBASE_H
#define QHFMODEBASE_H

#include "QHFGuiNotifier.h"
#include "QHFUIIEditorContext.h"
#include "QHFUIIModeContext.h"
#include "QHFUIUtils.h"

#include <QPaintEvent>
#include <QScrollArea>
#include <QString>
#include <QWidget>

namespace QHF {

    /**
     * @brief Editor (see description)
     * @details (see source) QWidget
     * 类主要是考虑到signal
     * connection和style表 应用.本类主要起到data管理方面 作用, 不suggest作为component直接使用.
     */
    class QHFModeBase : public QWidget, public QHFThemedCRTP<QHFModeBase>, public IModeContext {
        Q_OBJECT

      protected:
        IEditorContext* m_context;
        QWidget* m_parent;

      signals:
        /**
         * @brief  (see description)
         * @details (see source)
         */
        void contentChanged();

      public:
        /**
         * @brief Constructor
         * @param context editortopbottom文* @param parent Parent object
         */
        explicit QHFModeBase(IEditorContext* context, QWidget* parent = nullptr);
        ~QHFModeBase() override = default;

        // interfacerelated
        /**
         * @brief Initializes (see description)
         * @details (see source)
         */
        void initialize() override;

        /**
         * @brief Cleans up mode
         * @details (see source)
         */
        void cleanup() override;

        /**
         * @brief Updates content
         * @details (see source))
         */
        void update() override;

        /**
         * @brief  (see description)
         * @return Header contentwidget
         * @note component必须无Parent object, 父由 QHFEditorBase 管理
         */
        QWidget* createHeaderContent() override;

        /**
         * @brief  (see description)
         * @return Mode contentwidget
         * @note recommend在subclass initializeImpl()中完成Layout和widget, 
         * 若需要dynamiccreatecontentwidget可在subclass重载函数中create.noteReturns type
         */
        QWidget* createContent() override;

        /**
         * @brief  (see description)
         * @details (see source))
         */
        void connectSignals() override;

        /**
         * @brief  (see description)
         * @details (see source)
         */
        void disconnectSignals() override;

      protected:
        /**
         * @brief  (see description)
         * @param event 绘制eventpointer*/
        void paintEvent(QPaintEvent* event) override;

        /**
         * @brief Applies theme
         * @details (see source)
         */
        virtual void applyTheme();

      private:
        /**
         * @brief  (see description)
         * @details (see source)
         */
        virtual void initializeImpl();

        // --- notification处理 ---
        /**
         * @brief  (see description)
         */
        void handleNotify(const QHFNotifier& notifier) override;

        // --- Publishes notification ---
        /**
         * @brief  (see description)
         */
        void publishModeActive(const QString& reason, bool immediate = false) override;
    };

} // namespace QHF

#endif // HMODEBASE_H
