#ifndef QHFWORKSPACEBASE_H
#define QHFWORKSPACEBASE_H

#include "QHFGuiNotifierCenter.h"
#include "QHFTypeDefs.h"
#include "QHFUIIEditorContext.h"
#include "QHFUIIModeContext.h"

#include <QMap>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>
#include <memory>

namespace QHF {

    /**
     * @brief Layout (see description)
     * @details Pure data structure, Builds a layout tree
     */
    struct LayoutNode {

        /**
         * @brief Node type
         * @details (see source)
         */
        enum class Type : std::uint8_t {
            LEAF, ///< Leaf node
            SPLIT ///< Split node
        };

        /**
         * @brief  (see description)
         * @details (see source)
         */
        enum class Orientation : std::uint8_t {
            HORIZONTAL, ///< Horizontal
            VERTICAL    ///< Vertical
        };

        Type                type;        ///< Node type
        Orientation         orientation; ///< 方向
        EditorType          editorType;///< Edit器类型, Split Node时有效
        LayoutNode*         firstChild;  ///< 第一个子节点
        LayoutNode*         secondChild; ///< 第二个子节点
        QPair<float, float> splitRatio;  ///< 分割比例, 仅SPLIT节点有效
        QHFModeType           modeType;///< modetype, 基类leaf node modetype
/**
 * @brief Constructor.
         * @details Constructor.
         * @param type Node type
         */
        explicit LayoutNode(EditorType etype = EditorType::DUMMY);
        ~LayoutNode(); ///< Destructor
/**
 * @brief Split node.
         * @details Split node.
         * @param orien orientation* @param newEditorType 新node editortype*/
        void split(Orientation orien, EditorType newEditorType);

        /**
         * @brief  (see description)
         * @details (see source).
         * @return clone nodepointer*/
        [[nodiscard]] LayoutNode* clone() const;
    };

    class QHFWorkspaceBase : public QWidget {
        Q_OBJECT

      protected:
        bool         m_isInitialize; ///< is 否Initializes
        QWidget*     m_parent;       ///< Parent widget
        QVBoxLayout* m_layout;       ///< Layout
        LayoutNode*  m_root;         ///< Root node
        LayoutNode*  m_activeNode;   ///< Active node

      private:
        QMap<LayoutNode*, QWidget*> m_nodeToWidget; ///< 节点to widget 映射关系
        QMap<QWidget*, LayoutNode*> m_widgetToNode; ///< widgetto 节点 映射关系

      public:/**
         * @brief Constructor.
         * @details Constructor.
         * @param parent Parent widgetpointer
         * @param isInitialize is 否需要Initializes*/
        explicit QHFWorkspaceBase(QWidget* parent = nullptr, bool isInitialize = true);
        ~QHFWorkspaceBase() override = default; ///< Destructor
/**
 * @brief Initializes.
         * @details Initializes.
         */
        void initialize();

        /**
         * @brief Returns workspace name.
         * @details Returns workspace name.
         * @return Workspace name string
         * @note Virtual function, implemented by subclass。
         */
        [[nodiscard]] virtual QString workspaceName() const = 0; // 虚, 子类实现

        /**
         * @brief Returns workspace type.
         * @details Returns workspace type.
         * @return workspacetype* @note Virtual function, implemented by subclass。
         */
        [[nodiscard]] virtual QHFWorkspaceType workspaceType() const = 0; // 虚, 子类实现

        /**
         * @brief  (see description)
         * @details (see source).
         * @param root root nodepointer*/
        void setRoot(LayoutNode* root);

        /**
         * @brief  (see description)
         * @details (see source).
         * @return root nodepointer*/
        [[nodiscard]] LayoutNode* root() const;

        /**
         * @brief  (see description)
         * @details (see source).
         * @return clone root nodepointer*/
        [[nodiscard]] LayoutNode* cloneRoot() const;

      protected:
        /**
         * @brief  (see description)
         * @details (see source).
         * @param editor widgetpointer*/
        void setActiveNode(QWidget* editor);

        /**
         * @brief  (see description)
         * @details (see source).
         */
        void rebuild();

        /**
         * @brief  (see description)
         * @details (see source).
         * @param node nodepointer, for指定refresh哪个node Layout.ifnullptr, 则refresh全部node Layout。
         */
        void refresh(LayoutNode* node = nullptr);

        /**
         * @brief Split node
         * @details Split node.
         * @param newEditorType 新node editortype* @param orientation 新node orientation* @param useFullRebuild is 否全量rebuild*/
        void splitActive(EditorType              newEditorType = EditorType::NONE,
                         LayoutNode::Orientation orientation = LayoutNode::Orientation::HORIZONTAL,
                         bool                    useFullRebuild = false);

        /**
         * @brief  (see description)
         * @details (see source).
         * @param node nodepointer* @param useFullRebuild is 否全量rebuild*/
        void removeActive(bool useFullRebuild = false);

        /**
         * @brief  (see description)
         * @details (see source).
         * @param newType 新editortype*/
        void switchActive(EditorType newType);

        /**
         * @brief  (see description)
         * @details (see source).
         * @param node nodepointer* @param indent 缩进*/
        void printNode(LayoutNode* node, int indent = 0) const;

        /**
         * @brief  (see description)
         * @details (see source).
         * @param editor widgetpointer* @return nodepointer*/
        [[nodiscard]] LayoutNode* findNodeForWidget(QWidget* editor) const;

        /**
         * @brief  (see description)
         * @details (see source).
         * @param node nodepointer* @return widgetpointer*/
        [[nodiscard]] QWidget* findWidgetForNode(LayoutNode* node) const;

        /**
         * @brief  (see description)
         * @details (see source).
         * @param event eventpointer*/
        void paintEvent(QPaintEvent* event) override;

      private:
        /**
         * @brief  (see description)
         * @details (see source).
         */
        virtual void initializeImpl() = 0; // 虚, 子类实现

        /**
         * @brief  (see description)
         * @details (see source).
         */
        void clearMappings();

        /**
         * @brief  (see description)
         * @details (see source).
         * @param node nodepointer* @param current current nodepointer* @param parentChildPtr parent node child nodepointerpointer* @return parent nodepointer*/
        LayoutNode* findParentNode(LayoutNode* node, LayoutNode* current,
                                   LayoutNode** parentChildPtr); // 新增: 查找父节点

        /**
         * @brief  (see description)
         * @details (see source).
         * @param node nodepointer* @return 构建 widgetpointer*/
        QWidget* build(LayoutNode* node);

        /**
         * @brief  (see description)
         * @details (see source).
         * @param node nodepointer* @param existingWidget 已存at  widgetpointer* @return Update after widgetpointer*/
        QWidget* updateNode(LayoutNode* node,
                            QWidget*    existingWidget = nullptr); // 新增: 节点增量更新

        /**
         * @brief  (see description)
         * @details (see source).
         * @param leafNode Leaf nodepointer
         * @return editorwidgetpointer*/
        QWidget* createAndMapEditorWidget(LayoutNode* leafNode);

        /**
         * @brief  (see description)
         * @details (see source).
         * @param splitNode Split nodepointer
         * @param childWidgetBuilder 子widget构建function* @return split器widgetpointer*/
        QSplitter*
        createAndMapSplitterWidget(LayoutNode*                                 splitNode,
                                   const std::function<QWidget*(LayoutNode*)>& childWidgetBuilder);

        /**
         * @brief  (see description)
         * @details (see source) SPLIT
         * type时, update其子widget 逻辑部分
         * @param splitter split器pointer* @param splitterNode Split nodepointer
         * @param newFirstChild 新第一个子widgetpointer* @param newSecondChild 新第二个子widgetpointer*/
        void updateReusedSplitterChildren(QSplitter* splitter, LayoutNode* splitterNode,
                                          QWidget* newFirstChild, QWidget* newSecondChild);

        /// --- Slot function ----
        /**
         * @brief  (see description)
         * @details (see source).
         * @param splitter split器pointer* @param pos position* @param index index* @note Signal-slot
         */
        void onSplitterMoved(QSplitter* splitter, int pos, int index);

        /// --- 处理notification ----
        /*
         * @brief  (see description)
         * @param notifier notification*/
        void handleNotify(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @details (see source).
         * @param notifier notification*/
        void handleNewAppWindowInfoWindow(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @details (see source).
         * @param notifier notification*/
        void handleAppWindowActivated(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @details (see source).
         * @param notifier notification*/
        void handleAppWindowMoved(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @details (see source).
         * @param notifier notification*/
        void handleReportActivation(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @details (see source).
         * @param notifier notification*/
        void handleSplitHorizontal(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @details (see source).
         * @param notifier notification*/
        void handleSplitVertical(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @details (see source).
         * @param notifier notification*/
        void handleRemove(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @details (see source).
         * @param senderEditor 发送者widgetpointer* @param newType 新editortype*/
        void handleSwitchEditorType(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @details (see source).
         * @param notifier notification*/
        void handleSwitchEditorModeType(const QHFNotifier& notifier);

        /**
         * @brief  (see description)
         * @param notifier notification*/
        virtual void handleNotifyImpl(const QHFNotifier& notifier) = 0;

        /// --- notification ---
        /**
         * @brief  (see description)
         * @details (see source).
         * @param reason 原因string*/
        void publishWorkspaceActive(const QString& reason, bool immediate = false);
    };

} // namespace QHF

#endif // HWORKSPACEBASE_H
