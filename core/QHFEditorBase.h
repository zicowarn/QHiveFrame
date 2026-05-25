/**
 *  Layout管理: 提供general editorLayout (Header + 主体content区  QHBoxLayout), 包括 Header
  编辑器切换按钮、自定义内容区域和分割/关闭按钮, 以及主体  Tabbar 和内容区。 Mode 切换: 管理 Mode
  注册、切换和内容更新, 委托具体内容 (如 Header 内容、Tabbar、内容区)给 QHFModeBase  子类。
    公共事务: 处理编辑器 公共功能, 如主题应用、Event bus集成 (QHFGuiEventBus)、编辑器激活信号等。
 */
#ifndef QHFEDITORBASE_H
#define QHFEDITORBASE_H

#include "QHFGuiNotifier.h"
#include "QHFUIIEditorContext.h"
#include "QHFUIIModeContext.h"
#include "QHFUIUtils.h"

#include <QBoxLayout>
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QSize>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

namespace QHF {

    /**
     * @brief Editor (see description)
     * @details (see source)
     */
    class QHFEditorBase : public QWidget, QHFThemedCRTP<QHFEditorBase>, public IEditorContext {
        Q_OBJECT

      public:
        static constexpr int   ComboEditorMinWidth  = 100; ///< Edit器切换下拉框最小宽度
        static constexpr int   ComboModeMinWidth    = 100; ///< mode选择下拉框最小宽度
        static constexpr QSize HeaderIconSize= {22, 22}; ///< Header 按钮图标大小
        static constexpr QSize HeaderBiggerIconSize = {24, 24}; ///< Header Combo 大小

      protected:
        QWidget*                m_parent;          ///< Parent object
        QGridLayout*            m_headerLayout;    ///< Header Layout
        QWidget*                m_header;          ///< Header Container
        QComboBox*              m_editorCombo;     ///< Edit器切换下拉框
        QComboBox*              m_modeCombo;       ///< mode选择下拉框
        QWidget*                m_headerComboArea; ///< Header Combo 区域
        QWidget*                m_headerContent;   ///< Header 中间内容
        QWidget*                m_buttonArea;      ///< 右侧按钮区域
        QHBoxLayout*            m_contentLayout;   ///< 主体Layout
        QScrollArea*            m_contentScroll;   ///< 主体内容
        IModeContext*           m_currentMode;     ///< Current模式
        QMap<QString, QVariant> m_modeStates;      ///< modestate存储

      private:
        bool m_hasModeCombo;///< is 否有mode选择下拉框

      public:/**
         * @brief Constructor
         * @param parent Parent objectpointer
         */
        QHFEditorBase(QWidget* parent = nullptr);
        ~QHFEditorBase() override; ///< Virtual Destructor// 接口实现 IEditorContext
        /**
         * @brief Initializes
         */
        void initialize() override;

        /**
         * @brief  (see description)
         */
        void requestUpdate() override;

        /**
         * @brief  (see description)
         * @return Header Layout pointer
         */
        [[nodiscard]] QGridLayout* headerLayout() const override;

        /**
         * @brief  (see description)
         * @return Content Layout pointer
         */
        [[nodiscard]] QHBoxLayout* contentLayout() const override;

        /**
         * @brief Returns current  (see description)
         * @return current mode pointer*/
        [[nodiscard]] QString currentModeName() const override;

        /**
         * @brief  (see description)
         * @param modeName modename*/
        void setEditorPreSelectMode(const QString& modeName) override;

        /**
         * @brief  (see description)
         * @param modeName modename* @param state statedata*/
        void setModeState(const QString& modeName, const QVariant& state) override;

        /**
         * @brief  (see description)
         * @param modeName modename* @return statedata*/
        [[nodiscard]] QVariant getModeState(const QString& modeName) const override;

        // 非interfacetype
        /**
         * @brief Creates current  (see description)
         * @return 新mode pointer*/
        virtual IModeContext* createMode(const QString& modeName);

        /**
         * @brief  (see description)
         * @return default modename* @note Subclass must implement this function以Returns defaultmode name
         */
        [[nodiscard]] virtual QString defaultModeName() const = 0;

      protected:
        /**
         * @brief  (see description)
         * @param event 绘制eventpointer*/
        void paintEvent(QPaintEvent* event) override;

        /**
         * @brief  (see description)
         * @return SpecialMode namelist, QPair<显示name, name>
         * @note Subclass must implement this function以Returns all SpecialMode name, QPair<显示name, name>
         */
        [[nodiscard]] virtual QList<QPair<QString, QString>> collectModeNames() const = 0;

        /**
         * @brief  (see description)
         * @param modeNames modenamelist, QPair<显示name, mode name>
         * @note Subclass must implement this function以完成Initializes工作
         */
        virtual void initializeImpl(const QList<QPair<QString, QString>>& modeNames) = 0;

        /**
         * @brief  (see description)
         * @return contentarea pointer* @note
         * 非必要subclass尽量不要重载此函数, subclassensure在重载 setupHeaderLayout()函数中结束处调用此函数以ensure按钮area在Layout中.
         */
        virtual QWidget* createHeaderComboArea();

        /**
         * @brief  (see description)
         * @return 按钮area pointer* @note
         * 非必要subclass尽量不要重载此函数, subclassensure在重载 setupHeaderLayout()函数中结束处调用此函数以ensure按钮area在Layout中.
         */
        virtual QWidget* createHeaderButtonArea();

        /**
         * @brief  (see description)
         * @return is 否需要* @note Subclass may override this function以determine是否需要mode选择bottom拉框
         */
        [[nodiscard]] bool hasModeCombo() const;

        /**
         * @brief  (see description)
         * @param hasModeCombo is 否有*/
        void setHasModeCombo(bool hasModeCombo);

        /**
         * @brief Initializes (see description)
         * @param preSelectEditorName default editorname, 为empty则不setdefault
         */
        void initializeEditorCombo(const QString& preSelectEditorName = ""); // 非虚调用

        /**
         * @brief Initializes (see description)
         * @note
         * subclassConstructor中调用此函数以Initializes, subclass Constructor中调用setupLayout()函数之after调用此函数以updatemode选择bottom拉框, 因为Signal-slot connection是再
         * setupLayout() 函数中完成 .
         * @param modes modelist, QPair<显示name, mode name>
         * @param defaultMode default modename, non-emptymode name必须在 modes
         * list中exist, 会triggercombo defaultmode
         */
        void initializeModeCombo(const QList<QPair<QString, QString>>& modes,
                                 const QString& defaultMode = ""); // 虚调用

        /**
         * @brief  (see description)
         * @details (see source)
         */
        virtual void applyTheme();

        /**
         * @brief  (see description)
         * @details (see source)Layout
         * @note subclass Constructor中应调用此函数以InitializesLayout
         */
        virtual void setupLayout();

        /**
         * @brief  (see description)
         * @details (see source) Header Layout
         * @note subclass  setupLayout() 函数中应调用此函数以Initializes Header Layout
         */
        virtual void setupHeaderLayout(QBoxLayout* mainLayout);

        /**
         * @brief  (see description)
         * @details (see source)Updates contentLayout
         * @note subclass  updateContent() 函数中应调用此函数以Updates contentLayout
         */
        virtual void setupContentLayout(QBoxLayout* mainLayout);

        /**
         * @brief Updates content
         */
        void updateContent();

        /**
         * @brief Slot function:  (see description)
         * @param displayName 显示name* @note Subclass may override this function以处理mode切换event
         */
        virtual void handleModeSwitch(const QString& displayName);

        /**
         * @brief  (see description)
         * @param modeName modename*/
        void switchMode(const QString& modeName);

      private:
        // --- 处理notification ---
        /**
         * @brief  (see description)
         * @param notifier notification*/
        void handleNotify(const QHFNotifier& notifier) override;

        /**
         * @brief  (see description)
         * @param notifier notification*/
        void handleNewAppWindowInfoWorkspace(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @param notifier notification*/
        void handleAppWindowActivatedWorkspace(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @param notifier notification*/
        void handleAppWindowMovedWorkspace(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @param notifier notification*/
        void handleReportActivation(const QHFNotifier& notifier);

        // --- Publishes notification ---
        /**
         * @brief Publishes notification
         * @param reason notification原因*/
        void publishEditorActive(const QString& reason, bool immediate = false) override;
    };

} // namespace QHF

#endif // HEDITORBASE_H
