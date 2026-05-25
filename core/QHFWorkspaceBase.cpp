#include "QHFWorkspaceBase.h"

#include "QHFEditorFactory.h"
#include "QHFGuiEventBus.h"
#include "QHFGuiNotifierCenter.h"
#include "QHFThemeManager.h"

#include <QApplication>
#include <cmath>
#include <QDebug>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QSplitter>
#include <QStyle>
#include <QStyleOption>
#include <QTableWidget>
#include <QTimer>
#include <QVBoxLayout>

namespace QHF {

    constexpr int EditorMinSize = 30;
    // constexpr int SplitterHandleSize = 30; // 未使用, 可以移除或保留

    /// --- LayoutNode implementation ---
    LayoutNode::LayoutNode(EditorType etype) : editorType(etype) {
        constexpr float defaultSplitRatio = 0.5;
        type                              = Type::LEAF;
        orientation                       = Orientation::HORIZONTAL;
        firstChild                        = nullptr;
        secondChild                       = nullptr;
        splitRatio                        = {defaultSplitRatio, defaultSplitRatio};
        modeType                          = QHFModeType::NONE;
    }

    LayoutNode::~LayoutNode() {
        delete firstChild;
        delete secondChild;
    }

    void LayoutNode::split(Orientation orien, EditorType newEditorType) {
        if (type != Type::LEAF) {
            return;
        }
        type        = Type::SPLIT;
        orientation = orien;
        firstChild  = new LayoutNode(editorType);
        secondChild = new LayoutNode(newEditorType);
    }

    LayoutNode* LayoutNode::clone() const {
        auto* newNode        = new LayoutNode(editorType);
        newNode->type        = type;
        newNode->orientation = orientation;
        newNode->splitRatio  = splitRatio;
        newNode->modeType    = modeType;

        if (firstChild != nullptr) {
            newNode->firstChild = firstChild->clone();
        }
        if (secondChild != nullptr) {
            newNode->secondChild = secondChild->clone();
        }
        return newNode;
    }

    /// --- QHFWorkspaceBase implementation ---
    QHFWorkspaceBase::QHFWorkspaceBase(QWidget* parent, bool isInitialize)
        : QWidget(parent), m_isInitialize(isInitialize) {
        m_parent     = parent;
        m_root       = nullptr;
        m_activeNode = nullptr;
        m_layout     = nullptr;
        m_nodeToWidget.clear();
        m_widgetToNode.clear();
    }

    void QHFWorkspaceBase::initialize() {
        constexpr int layoutMargin = 5;
        m_layout                   = new QVBoxLayout(this);
        m_layout->setContentsMargins(layoutMargin, layoutMargin, layoutMargin, layoutMargin);
        m_layout->setSpacing(0);
        setLayout(m_layout);
        // 尺寸策略
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        // subclassInitializes
        initializeImpl();
        // event处理
        connect(&QHFNotifierCenter::instance(), &QHFNotifierCenter::notify, this,
                &QHFWorkspaceBase::handleNotify);
        // 调试
        qDebug() << "QHFWorkspaceBase initialized: SizePolicy =" << sizePolicy()
                 << "Size =" << size();
    }

    void QHFWorkspaceBase::setRoot(LayoutNode* root) {
        if (m_root == root) {
            return;
        }
        delete m_root;
        m_root = root;
        rebuild();
    }

    LayoutNode* QHFWorkspaceBase::root() const
    {
        return m_root;
    }

    LayoutNode* QHFWorkspaceBase::cloneRoot() const {
        if (m_root == nullptr) {
            return nullptr;
        }
        return m_root->clone();
    }

    void QHFWorkspaceBase::setActiveNode(QWidget* editor) {
        if (editor == nullptr) {
            return;
        }
        m_activeNode = findNodeForWidget(editor);
        if (m_activeNode == nullptr) {
            qDebug() << "SetActiveNode: No node found for widget" << editor;
        }
        qDebug() << "SetActiveNode: m_activeNode =" << m_activeNode;
    }

    void QHFWorkspaceBase::rebuild() {
        qDebug() << "Rebuild: Starting layout rebuild, QHFWorkspaceBase size =" << size();
        QLayoutItem* item;
        while ((item = m_layout->takeAt(0)) != nullptr) {
            if (auto* widget = item->widget()) {
                qDebug() << "Rebuild: Deleting widget" << widget;
                widget->deleteLater();
            }
            delete item;
        }
        clearMappings();

        if (m_root != nullptr) {
            QWidget* newWidget = build(m_root);
            if (newWidget != nullptr) {
                m_layout->addWidget(newWidget);
                qDebug() << "Rebuild: Added new widget" << newWidget
                         << "Visible:" << newWidget->isVisible() << "Size:" << newWidget->size()
                         << "Geometry:" << newWidget->geometry();
            } else {
                qDebug() << "Rebuild: Failed to build widget from root node";
            }
        } else {
            qDebug() << "Rebuild: Root node is null";
        }
        qDebug() << "Rebuild: Layout rebuild completed, QHFWorkspaceBase size =" << size();
    }

    // NOLINTNEXTLINE(readability-function-cognitive-complexity)
    void QHFWorkspaceBase::refresh(LayoutNode* node) {
        qDebug() << "Refresh: Starting incremental update for node" << node;
        if (node == nullptr) {
            node = m_root;
        }
        if (node == nullptr) {
            qDebug() << "Refresh: Root node is null";
            return;
        }

        QWidget* updatedWidget = updateNode(node, m_nodeToWidget.value(node, nullptr));
        if (updatedWidget == nullptr) {
            qDebug() << "Refresh: Failed to update node";
            return;
        }

        if (node == m_root) {
            QLayoutItem* item;
            while ((item = m_layout->takeAt(0)) != nullptr) {
                if (auto* widget = item->widget()) {
                    widget->deleteLater();
                }
                delete item;
            }
            m_layout->addWidget(updatedWidget);
            QTimer::singleShot(0, this, [this, updatedWidget]() {
                updatedWidget->updateGeometry();
                updatedWidget->update();
                qDebug() << "Refresh: Updated root widget" << updatedWidget
                         << "Visible:" << updatedWidget->isVisible()
                         << "Size:" << updatedWidget->size()
                         << "Geometry:" << updatedWidget->geometry();
                update();
                QApplication::processEvents();
            });
        } else {
            LayoutNode* parentNode     = nullptr;
            LayoutNode* parentChildPtr = nullptr;
            parentNode                 = findParentNode(node, m_root, &parentChildPtr);
            if (parentNode != nullptr) {
                auto* parentSplitter =
                    qobject_cast<QSplitter*>(m_nodeToWidget.value(parentNode, nullptr));
                if (parentSplitter != nullptr) {
                    int index = (parentNode->firstChild == node) ? 0 : 1;
                    if (parentNode->type == LayoutNode::Type::SPLIT &&
                        parentNode->firstChild != nullptr && parentNode->secondChild != nullptr) {
                        QWidget* firstWidget =
                            m_nodeToWidget.value(parentNode->firstChild, nullptr);
                        QWidget* secondWidget =
                            m_nodeToWidget.value(parentNode->secondChild, nullptr);
                        if (firstWidget == nullptr) {
                            firstWidget = updateNode(parentNode->firstChild, nullptr);
                            m_nodeToWidget[parentNode->firstChild] = firstWidget;
                            m_widgetToNode[firstWidget]            = parentNode->firstChild;
                        }
                        if (secondWidget == nullptr) {
                            secondWidget = updateNode(parentNode->secondChild, nullptr);
                            m_nodeToWidget[parentNode->secondChild] = secondWidget;
                            m_widgetToNode[secondWidget]            = parentNode->secondChild;
                        }
                        if (parentSplitter->count() < 2) {
                            if (parentSplitter->count() == 0) {
                                parentSplitter->addWidget(firstWidget);
                                parentSplitter->addWidget(secondWidget);
                            } else if (parentSplitter->count() == 1) {
                                if (parentSplitter->widget(0) != firstWidget) {
                                    parentSplitter->replaceWidget(0, firstWidget);
                                }
                                parentSplitter->addWidget(secondWidget);
                            }
                        } else {
                            if (parentSplitter->widget(0) != firstWidget) {
                                parentSplitter->replaceWidget(0, firstWidget);
                            }
                            if (parentSplitter->widget(1) != secondWidget) {
                                parentSplitter->replaceWidget(1, secondWidget);
                            }
                        }
                    } else if (parentNode->type == LayoutNode::Type::LEAF) {
                        LayoutNode* grandParentNode     = nullptr;
                        LayoutNode* grandParentChildPtr = nullptr;
                        grandParentNode = findParentNode(parentNode, m_root, &grandParentChildPtr);
                        if (grandParentNode != nullptr) {
                            auto* grandParentSplitter = qobject_cast<QSplitter*>(
                                m_nodeToWidget.value(grandParentNode, nullptr));
                            if (grandParentSplitter != nullptr) {
                                int parentIndex =
                                    (grandParentNode->firstChild == parentNode) ? 0 : 1;
                                grandParentSplitter->replaceWidget(parentIndex, updatedWidget);
                                m_nodeToWidget[parentNode]    = updatedWidget;
                                m_widgetToNode[updatedWidget] = parentNode;
                            }
                        }
                    }
                    if (parentNode->type == LayoutNode::Type::SPLIT &&
                        index < parentSplitter->count() &&
                        parentSplitter->widget(index) != updatedWidget) {
                        parentSplitter->replaceWidget(index, updatedWidget);
                    } else if (parentNode->type == LayoutNode::Type::SPLIT &&
                               index >= parentSplitter->count()) {
                        qDebug() << "Refresh: Index" << index
                                 << "out of range, count =" << parentSplitter->count();
                        parentSplitter->addWidget(updatedWidget);
                    }
                    QTimer::singleShot(0, this, [this, parentSplitter, parentNode]() {
                        if (parentSplitter) {
                            int halfSize =
                                (parentSplitter->orientation() == Qt::Horizontal ? width()
                                                                                 : height()) /
                                2;
                            parentSplitter->setSizes({halfSize, halfSize});
                            parentSplitter->updateGeometry();
                            parentSplitter->update();
                            for (int i = 0; i < parentSplitter->count(); ++i) {
                                if (auto* widget = parentSplitter->widget(i)) {
                                    widget->updateGeometry();
                                    widget->update();
                                }
                            }
                            qDebug() << "Refresh: Updated splitter" << parentSplitter
                                     << "Visible:" << parentSplitter->isVisible()
                                     << "Size:" << parentSplitter->size()
                                     << "Sizes:" << parentSplitter->sizes()
                                     << "Geometry:" << parentSplitter->geometry();
                        }
                        qDebug() << "Refresh: Post-update layout tree:";
                        printNode(m_root, 0);
                        update();
                        QApplication::processEvents();
                    });
                } else {
                    qDebug() << "Refresh: Parent widget is not a splitter";
                }
            } else {
                qDebug() << "Refresh: Failed to find parent node for" << node;
            }
        }
        qDebug() << "Refresh: Incremental update completed";
    }

    void QHFWorkspaceBase::splitActive(EditorType newEditorType, LayoutNode::Orientation orientation,
                                     bool useFullRebuild) {
        constexpr float defaultSplitRatio = 0.5;
        // ensureactivenodevalid
        if (m_activeNode == nullptr || m_activeNode->type != LayoutNode::Type::LEAF) {
            qDebug() << "SplitActive: Invalid active node or not a LEAF node";
            return;
        }

        QWidget* activeWidget = m_nodeToWidget.value(m_activeNode, nullptr);
        if (activeWidget == nullptr) {
            qDebug() << "SplitActive: No widget found for active node";
            return;
        }

        EditorType splitType =
            newEditorType == EditorType::NONE ? m_activeNode->editorType : newEditorType;
        m_activeNode->split(orientation, splitType);
        m_activeNode->splitRatio = {defaultSplitRatio, defaultSplitRatio}; // 设置默认比例

        m_nodeToWidget[m_activeNode->firstChild] = activeWidget;
        m_widgetToNode[activeWidget]             = m_activeNode->firstChild;
        m_nodeToWidget.remove(m_activeNode);
        m_widgetToNode.remove(m_nodeToWidget.value(m_activeNode, nullptr));

        if (useFullRebuild) {
            rebuild();
        } else {
            refresh(m_activeNode);
        }
    }

    void QHFWorkspaceBase::removeActive(bool useFullRebuild) {
        if (m_activeNode == m_root) {
            qDebug() << "RemoveActive: Cannot remove root node";
            return;
        }

        if (m_activeNode == nullptr || m_activeNode->type != LayoutNode::Type::LEAF) {
            qDebug() << "RemoveActive: Invalid active node or not a LEAF node";
            return;
        }

        LayoutNode* parentNode     = nullptr;
        LayoutNode* parentChildPtr = nullptr;
        parentNode                 = findParentNode(m_activeNode, m_root, &parentChildPtr);
        if (parentNode == nullptr) {
            qDebug() << "RemoveActive: Failed to find parent node for" << m_activeNode;
            return;
        }

        LayoutNode* siblingNode = (parentNode->firstChild == m_activeNode) ? parentNode->secondChild
                                                                           : parentNode->firstChild;
        if (siblingNode == nullptr) {
            qDebug() << "RemoveActive: No sibling node found";
            return;
        }

        QWidget* activeWidget  = m_nodeToWidget.value(m_activeNode, nullptr);
        QWidget* siblingWidget = m_nodeToWidget.value(siblingNode, nullptr);

        // updateparent node 属性为兄弟node 属性
        parentNode->type        = siblingNode->type;
        parentNode->orientation = siblingNode->orientation;
        parentNode->editorType  = siblingNode->editorType;
        parentNode->firstChild  = siblingNode->firstChild;
        parentNode->secondChild = siblingNode->secondChild;

        // updatemap
        if (siblingWidget != nullptr) {
            m_nodeToWidget[parentNode]    = siblingWidget;
            m_widgetToNode[siblingWidget] = parentNode;
        }

        // cleanupactivenode map和widget
        if (activeWidget != nullptr) {
            m_nodeToWidget.remove(m_activeNode);
            m_widgetToNode.remove(activeWidget);
            activeWidget->deleteLater();
        }

        // removenode
        siblingNode->firstChild  = nullptr;
        siblingNode->secondChild = nullptr;
        delete m_activeNode;
        delete siblingNode;
        m_activeNode = nullptr;

        qDebug() << "RemoveActive: Pre-refresh layout tree:";
        printNode(m_root, 0);

        if (useFullRebuild) {
            rebuild();
        } else {
            refresh(parentNode);
        }
    }

    void QHFWorkspaceBase::switchActive(EditorType newType) {
        qDebug() << "switchActive: Attempting to switch active editor to type"
                 << static_cast<int>(newType);

        if (m_activeNode == nullptr) {
            qDebug() << "switchActive: No active node to switch.";
            return;
        }

        if (m_activeNode->type != LayoutNode::Type::LEAF) {
            qDebug() << "switchActive: Active node is not a LEAF, cannot switch editor type.";
            return;
        }

        if (m_activeNode->editorType == newType) {
            qDebug() << "switchActive: Active editor is already of the requested type"
                     << static_cast<int>(newType);
            return;
        }

        QWidget* oldWidget = m_nodeToWidget.value(m_activeNode, nullptr);

        if (oldWidget != nullptr) {
            qDebug() << "switchActive: Removing old widget" << oldWidget << "for node"
                     << m_activeNode;
            m_nodeToWidget.remove(
                m_activeNode); // 关键: 移除映射, 让 updateNode 认为没有 existingWidget
            m_widgetToNode.remove(oldWidget);

            // oldWidget->setParent(nullptr); // 从父Layout中移除通常由 QSplitter::replaceWidget 或
            // QLayout::replaceWidget 完成 refresh 函数会处理UI层面 替换。 deleteLater()
            // ensure在event循环empty闲时安全remove。 updateNode 在其 !canReuse 分支本身就会对
            // existingWidget 调用 deleteLater,  但由于我们这里让 existingWidget 变为 nullptr
            // 传给它, 所以需要在这里处理。 或者, 我们可以不从map移除, 而是修改 updateNode  
            // canReuse 逻辑 (更复杂)。 目before这种做法 (移除map + 主动deleteLater)更直接, 对
            // updateNode 改动小。
            oldWidget->deleteLater(); // 安排旧控件 销毁
        } else {
            qDebug() << "switchActive: No existing widget found for active node" << m_activeNode
                     << "Proceeding to create new.";
        }

        m_activeNode->editorType = newType; // 更新节点中 编辑器类型
        qDebug() << "switchActive: Set active node" << m_activeNode << "to new type"
                 << static_cast<int>(newType);

        refresh(m_activeNode); // 刷新Layout, 这将导致创建并显示新类型 编辑器

        qDebug() << "switchActive: Refresh called for node" << m_activeNode;
    }

    void QHFWorkspaceBase::printNode(LayoutNode* node, int indent) const {
        if (node == nullptr) {
            return;
        }
        QString indentStr;
        for (int i = 0; i < indent; ++i) {
            indentStr += "  ";
        }
        QString typeStr = node->type == LayoutNode::Type::LEAF ? "LEAF" : "SPLIT";
        QString orientationStr =
            node->orientation == LayoutNode::Orientation::HORIZONTAL ? "HORIZONTAL" : "VERTICAL";
        QString editorTypeStr;
        if (node->editorType == EditorType::NONE) {
            editorTypeStr = "None";
        } else if (node->editorType == EditorType::DUMMY) {
            editorTypeStr = "Dummy";
        } else if (node->editorType == EditorType::SPECIAL) {
            editorTypeStr = "Special";
        } else {
            editorTypeStr = "Unknown";
        }

        if (node->type == LayoutNode::Type::LEAF) {
            auto     iter    = m_nodeToWidget.find(node);
            QWidget* editor  = iter != m_nodeToWidget.end() ? iter.value() : nullptr;
            QString  sizeStr = editor != nullptr ? QString("QSize(%1,%2)")
                                                      .arg(editor->size().width())
                                                      .arg(editor->size().height())
                                                 : "None";
            qDebug() << indentStr << typeStr << " " << orientationStr
                     << " EditorType:" << editorTypeStr << " Size:" << sizeStr
                     << " Widget:" << editor;
        } else {
            qDebug() << indentStr << typeStr << " " << orientationStr
                     << " EditorType:" << editorTypeStr;
        }
        printNode(node->firstChild, indent + 1);
        printNode(node->secondChild, indent + 1);
    }

    LayoutNode* QHFWorkspaceBase::findNodeForWidget(QWidget* editor) const {
        auto iter = m_widgetToNode.find(editor);
        return iter != m_widgetToNode.end() ? iter.value() : nullptr;
    }

    QWidget* QHFWorkspaceBase::findWidgetForNode(LayoutNode* node) const {
        auto iter = m_nodeToWidget.find(node);
        return iter != m_nodeToWidget.end() ? iter.value() : nullptr;
    }

    void QHFWorkspaceBase::paintEvent(QPaintEvent* event) {
        QStyleOption opt;
        opt.initFrom(this);
        QPainter painter(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
        QWidget::paintEvent(event);
    }

    void QHFWorkspaceBase::clearMappings() {
        m_nodeToWidget.clear();
        m_widgetToNode.clear();
        m_activeNode = nullptr;
    }

    LayoutNode* QHFWorkspaceBase::findParentNode(LayoutNode* node, LayoutNode* current,
                                               LayoutNode** parentChildPtr) {
        if (current == nullptr) {
            return nullptr;
        }
        if (current->firstChild == node) {
            *parentChildPtr = current->firstChild;
            return current;
        }
        if (current->secondChild == node) {
            *parentChildPtr = current->secondChild;
            return current;
        }
        LayoutNode* parent = findParentNode(node, current->firstChild, parentChildPtr);
        if (parent != nullptr) {
            return parent;
        }
        return findParentNode(node, current->secondChild, parentChildPtr);
    }

    QWidget* QHFWorkspaceBase::build(LayoutNode* node) {
        if (node == nullptr) {
            qDebug() << "Build: Node is null";
            return nullptr;
        }

        if (node->type == LayoutNode::Type::LEAF) {
            return createAndMapEditorWidget(node);
        }
        // node->type == LayoutNode::Type::SPLIT
        return createAndMapSplitterWidget(node, [this](LayoutNode* childNode) {
            return build(childNode); // Recursively call build for children
        });
    }

    // NOLINTNEXTLINE(readability-function-cognitive-complexity)
    QWidget* QHFWorkspaceBase::updateNode(LayoutNode* node, QWidget* existingWidget) {
        if (node == nullptr) {
            qDebug() << "UpdateNode: Node is null";
            return nullptr;
        }

        bool canReuse = false;
        if (existingWidget != nullptr) {
            auto iter = m_nodeToWidget.find(node);
            if (iter != m_nodeToWidget.end() && iter.value() == existingWidget) {
                if (qobject_cast<QSplitter*>(existingWidget) == nullptr) {
                    if (node->type == LayoutNode::Type::LEAF) {
                        canReuse = true;
                    }
                } else {
                    if (node->type == LayoutNode::Type::SPLIT &&
                        static_cast<QSplitter*>(existingWidget)->orientation() ==
                            (node->orientation == LayoutNode::Orientation::HORIZONTAL
                                 ? Qt::Horizontal
                                 : Qt::Vertical)) {
                        canReuse = true;
                    }
                }
            }
        }

        if (canReuse) {
            qDebug() << "UpdateNode: Reusing existing widget" << existingWidget << "for node"
                     << node
                     << "Type:" << (node->type == LayoutNode::Type::LEAF ? "LEAF" : "SPLIT");
            if (node->type == LayoutNode::Type::SPLIT) {
                auto* splitter = qobject_cast<QSplitter*>(existingWidget);
                if (splitter != nullptr && node->firstChild != nullptr &&
                    node->secondChild != nullptr) {
                    // Recursively update children's widgets
                    QWidget* firstChildWidget = updateNode(
                        node->firstChild, m_nodeToWidget.value(node->firstChild, nullptr));
                    QWidget* secondChildWidget = updateNode(
                        node->secondChild, m_nodeToWidget.value(node->secondChild, nullptr));

                    if (firstChildWidget != nullptr && secondChildWidget != nullptr) {
                        // 调用新 辅助函数来处理 splitter 子项 update
                        updateReusedSplitterChildren(splitter, node, firstChildWidget,
                                                     secondChildWidget);
                    } else {
                        qDebug() << "UpdateNode: Failed to update one or both children for "
                                    "reusable splitter for node"
                                 << node << "First valid:" << (bool)firstChildWidget
                                 << "Second valid:" << (bool)secondChildWidget;
                        // 此处原有代码没有明确处理子widgetupdatefailure 情况, 保持行为一致
                        // 如果严格要求, 当子widgetupdatefailure时, 可能不应该重用父 splitter, 或者父
                        // splitter 需要清empty
                    }
                }
            }
            return existingWidget; // Return reused widget
        }

        // Cannot reuse: Remove old widget (if any), create new one
        if (existingWidget != nullptr) {
            m_widgetToNode.remove(existingWidget);
            existingWidget->deleteLater();
        }

        // Create new widget using previously refactored helper functions
        if (node->type == LayoutNode::Type::LEAF) {
            return createAndMapEditorWidget(node);
        }
        // node->type == LayoutNode::Type::SPLIT
        return createAndMapSplitterWidget(node, [this](LayoutNode* childNode) {
            return updateNode(childNode, m_nodeToWidget.value(childNode, nullptr));
        });
    }

    QWidget* QHFWorkspaceBase::createAndMapEditorWidget(LayoutNode* leafNode) {
        if (leafNode->editorType == EditorType::NONE) {
            qDebug() << "CreateAndMapEditorWidget: Invalid editor type for LEAF node";
            return nullptr;
        }

        QWidget* editor;
        editor = QHFEditorRegistry::instance().create(leafNode->editorType, this);
        if (editor == nullptr) {
            qDebug() << "CreateAndMapEditorWidget: Failed to create editor for type"
                     << static_cast<int>(leafNode->editorType);
            // create个 Dummy editor, 防止程序崩溃
            editor = QHFEditorRegistry::instance().create(EditorType::DUMMY, this);
            if (editor == nullptr) {
                leafNode->editorType = EditorType::DUMMY; // 标记为无效编辑器类型
                qDebug() << "CreateAndMapEditorWidget: Failed to create dummy editor";
                return nullptr;
            }
        }
        //
        auto* editorContext = dynamic_cast<IEditorContext*>(editor);
        if (editorContext != nullptr && leafNode->modeType != QHFModeType::NONE) {
            auto editorModeName = QHFModeTypeUtils::toString(leafNode->modeType);
            qDebug() << "CreateAndMapEditorWidget: Setting editor mode to" << editorModeName;
            editorContext->setEditorPreSelectMode(editorModeName);
        }

        editor->setMinimumSize(EditorMinSize, EditorMinSize);
        m_nodeToWidget[leafNode] = editor;
        m_widgetToNode[editor]   = leafNode;

        qDebug() << "CreateAndMapEditorWidget: Created editor" << editor
                 << "Parent:" << editor->parent() << "Visible:" << editor->isVisible()
                 << "Size:" << editor->size() << "Geometry:" << editor->geometry();
        return editor;
    }

    QSplitter* QHFWorkspaceBase::createAndMapSplitterWidget(
        LayoutNode* splitNode, const std::function<QWidget*(LayoutNode*)>& childWidgetBuilder) {
        auto* splitter = new QSplitter(splitNode->orientation == LayoutNode::Orientation::HORIZONTAL
                                           ? Qt::Horizontal
                                           : Qt::Vertical,
                                       this); // Pass 'this' as parent
        splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        splitter->setChildrenCollapsible(false);
        if (splitNode->orientation == LayoutNode::Orientation::HORIZONTAL) {
            splitter->setMinimumSize(EditorMinSize * 2, EditorMinSize);
        } else {
            splitter->setMinimumSize(EditorMinSize, EditorMinSize * 2);
        }

        QWidget* firstWidget  = childWidgetBuilder(splitNode->firstChild);
        QWidget* secondWidget = childWidgetBuilder(splitNode->secondChild);

        if (firstWidget != nullptr && secondWidget != nullptr) {
            if (splitNode->orientation == LayoutNode::Orientation::HORIZONTAL) {
                firstWidget->setMinimumHeight(EditorMinSize);
                secondWidget->setMinimumHeight(EditorMinSize);
            } else {
                firstWidget->setMinimumWidth(EditorMinSize);
                secondWidget->setMinimumWidth(EditorMinSize);
            }
            splitter->addWidget(firstWidget);
            splitter->addWidget(secondWidget);

            /*
             * suggest在将split器Adds to Layout并set大小after再set大小。
             * 然而, 为了保持与原始“构建”功能一致, 该“构建”会立即set大小: 
             */
            constexpr int defaulTotalSize = 200; // Default reasonable size
            int           totalSize =
                (splitNode->orientation == LayoutNode::Orientation::HORIZONTAL ? width()
                                                                               : height());
            if (totalSize <= 0) {
                totalSize = defaulTotalSize; // Default value, 防止Initializes时无尺寸
            }
            totalSize = std::max(totalSize, EditorMinSize * 2);
            /// NOLINTNEXTLINE(bugprone-narrowing-conversions)
            int firstSize  = static_cast<int>(std::round(totalSize * splitNode->splitRatio.first));
            int secondSize = totalSize - firstSize;
            firstSize      = std::max(firstSize, EditorMinSize);
            secondSize     = std::max(secondSize, EditorMinSize);
            splitter->setSizes({firstSize, secondSize});

            // update比例 (以实际set 尺寸为准, 防止最小尺寸约束导致偏差)
            // 将int转换为float是为了ensure在计算比例时进行浮点数运算, 从而避免整数除法导致 精度问题
            float actualTotal = static_cast<float>(firstSize) + static_cast<float>(secondSize);

            if (actualTotal > 0) {
                /// NOLINTNEXTLINE(bugprone-narrowing-conversions)
                splitNode->splitRatio = {firstSize / actualTotal, secondSize / actualTotal};
            }

            // connection splitterMoved signal
            connect(splitter, &QSplitter::splitterMoved, this,
                    [this, splitter, splitNode](int pos, int index) {
                        onSplitterMoved(splitter, pos, index);
                    });

            m_nodeToWidget[splitNode] = splitter;
            m_widgetToNode[splitter]  = splitNode;
            qDebug() << "CreateAndMapSplitterWidget: Created splitter" << splitter
                     << "with children" << firstWidget << secondWidget
                     << "Visible:" << splitter->isVisible() << "Size:" << splitter->size()
                     << "Sizes:" << splitter->sizes() << "Geometry:" << splitter->geometry();
        } else {
            qDebug() << "CreateAndMapSplitterWidget: Failed to create children for splitter, "
                        "firstWidget:"
                     << firstWidget << "secondWidget:" << secondWidget;
            delete splitter; // Clean up the created splitter if children failed
            return nullptr;
        }
        splitter->setVisible(true); // Ensure splitter is visible
        return splitter;
    }

    // NOLINTNEXTLINE(readability-function-cognitive-complexity)
    void QHFWorkspaceBase::updateReusedSplitterChildren(QSplitter* splitter, LayoutNode* splitterNode,
                                                      QWidget* newFirstChild,
                                                      QWidget* newSecondChild) {
        if (splitter == nullptr || splitterNode == nullptr || newFirstChild == nullptr ||
            newSecondChild == nullptr) {
            qDebug() << "updateReusedSplitterChildren: Invalid arguments provided.";
            // 如果allow子widget为empty, 这里 逻辑需要调整
            if (splitter != nullptr && (newFirstChild == nullptr || newSecondChild == nullptr)) {
                qDebug() << "updateReusedSplitterChildren: One or both child widgets are null. "
                            "Clearing splitter.";
            }
            return;
        }

        if (splitter->count() == 0) {
            splitter->addWidget(newFirstChild);
            splitter->addWidget(newSecondChild);
        } else if (splitter->count() == 1) {
            QWidget* actualOldW0 = splitter->widget(0); // 即 currentW0_before_ops
            if (actualOldW0 != newFirstChild) {
                splitter->replaceWidget(0, newFirstChild);
                // 原有 “修正”逻辑块, for处理 replaceWidget 可能未按预期工作 情况
                if (splitter->widget(0) != newFirstChild) {
                    qDebug() << "updateReusedSplitterChildren: Corrective logic triggered for "
                                "replaceWidget(0). Current widget:"
                             << splitter->widget(0) << "Target:" << newFirstChild;
                    // 此时 splitter->widget(0) 仍然是 actualOldW0
                    // delete actualOldW0; // 直接 delete 可能不安全, 如果它仍在event循环中
                    if (actualOldW0 != nullptr) {
                        actualOldW0->deleteLater();
                    }
                    splitter->insertWidget(0, newFirstChild); // 插入新 
                } else {
                    // replaceWidget success, actualOldW0 已被移除
                    if (actualOldW0 != nullptr && actualOldW0 != newSecondChild &&
                        actualOldW0->parent() == nullptr) { // 确保它没被用作另一个子项且已被移除
                        actualOldW0->deleteLater();
                    }
                }
            }
            splitter->addWidget(newSecondChild);
        } else {                                        // count is 2 or more (通常应该是 2)
            QWidget* actualOldW0 = splitter->widget(0); // 即 currentW0_before_ops
            if (actualOldW0 != newFirstChild) {
                splitter->replaceWidget(0, newFirstChild);
                if (actualOldW0 != nullptr && actualOldW0 != newSecondChild &&
                    actualOldW0->parent() == nullptr) {
                    actualOldW0->deleteLater();
                }
            }

            QWidget* actualOldW1 = splitter->widget(1); // 即 currentW1_before_ops
            if (actualOldW1 != newSecondChild) {
                splitter->replaceWidget(1, newSecondChild);
                if (actualOldW1 != nullptr && actualOldW1 != newFirstChild &&
                    actualOldW1->parent() == nullptr) {
                    actualOldW1->deleteLater();
                }
            }
        }

        // 应用save 比例
        constexpr int defaulTotalSize = 200; // Default reasonable size
        int           totalSize =
            (splitterNode->orientation == LayoutNode::Orientation::HORIZONTAL ? width() : height());
        if (totalSize <= 0) {
            totalSize = defaulTotalSize; // Default value, 防止Initializes时无尺寸
        }
        totalSize = std::max(totalSize, EditorMinSize * 2);
        /// NOLINTNEXTLINE(bugprone-narrowing-conversions)
        int firstSize  = static_cast<int>(std::round(totalSize * splitterNode->splitRatio.first));
        int secondSize = totalSize - firstSize;
        firstSize      = std::max(firstSize, EditorMinSize);
        secondSize     = std::max(secondSize, EditorMinSize);
        splitter->setSizes({firstSize, secondSize});

        // update比例 (以实际set 尺寸为准, 防止最小尺寸约束导致偏差)
        // 将int转换为float是为了ensure在计算比例时进行浮点数运算, 从而避免整数除法导致 精度问题
        float actualTotal = static_cast<float>(firstSize) + static_cast<float>(secondSize);

        if (actualTotal > 0) {
            /// NOLINTNEXTLINE(bugprone-narrowing-conversions)
            splitterNode->splitRatio = {firstSize / actualTotal, secondSize / actualTotal};
        }

        // 重新connection splitterMoved signal
        disconnect(splitter, &QSplitter::splitterMoved, this, nullptr);
        connect(splitter, &QSplitter::splitterMoved, this,
                [this, splitter, splitterNode](int pos, int index) {
                    onSplitterMoved(splitter, pos, index);
                });

        // updateparent node (即 splitterNode) map
        m_nodeToWidget[splitterNode] = splitter;
        m_widgetToNode[splitter]     = splitterNode;

        qDebug() << "updateReusedSplitterChildren: Splitter" << splitter << "updated with children"
                 << newFirstChild << newSecondChild;
    }

    /// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    void QHFWorkspaceBase::onSplitterMoved(QSplitter* splitter, int pos, int index) {
        auto iter = m_widgetToNode.find(splitter);
        if (iter == m_widgetToNode.end()) {
            qDebug() << "Splitter not found in m_widgetToNode:" << splitter;
            return;
        }
        // Emit signal, notificationeditorupdateLayout
        publishWorkspaceActive("Splitter moved", false);
        //
        LayoutNode* splitNode = iter.value();
        auto        sizes     = splitter->sizes();
        if (sizes.size() != 2) {
            qDebug() << "Invalid splitter sizes count:" << sizes.size();
            return;
        }
        float total = static_cast<float>(sizes[0]) + static_cast<float>(sizes[1]);
        if (total > 0) {
            splitNode->splitRatio = {static_cast<float>(sizes[0]) / total,
                                     static_cast<float>(sizes[1]) / total};
            qDebug() << "Updated split ratio for node" << splitNode << ":"
                     << splitNode->splitRatio.first << "," << splitNode->splitRatio.second;
        }
    }

    void QHFWorkspaceBase::handleNotify(const QHFNotifier& notifier) {
        switch (notifier.type) {
        case QHFNotifierType::NEW_APP_WINDOW_INFO_WINDOW:
            handleNewAppWindowInfoWindow(notifier);
            break;
        case QHFNotifierType::APP_WINDOW_ACTIVATED:
            handleAppWindowActivated(notifier);
            break;
        case QHFNotifierType::APP_WINDOW_MOVED:
            handleAppWindowMoved(notifier);
            break;
        case QHFNotifierType::REPORT_ACTIVATION:
            handleReportActivation(notifier);
            break;
        case QHFNotifierType::SPLIT_EDITOR_HORIZONTAL:
            handleSplitHorizontal(notifier);
            break;
        case QHFNotifierType::SPLIT_EDITOR_VERTICAL:
            handleSplitVertical(notifier);
            break;
        case QHFNotifierType::REMOVE_EDITOR:
            handleRemove(notifier);
            break;
        case QHFNotifierType::SWITCH_EDITOR_TYPE:
            handleSwitchEditorType(notifier);
            break;
        case QHFNotifierType::SWITCH_MODE_TYPE:
            handleSwitchEditorModeType(notifier);
            break;
        default:
            break;
        }
        // 调用subclass处理notification
        handleNotifyImpl(notifier);
    }

    void QHFWorkspaceBase::handleNewAppWindowInfoWindow(const QHFNotifier& notifier) {
        auto windowOpt = notifier.get<QWidget*>("window");
        if (windowOpt) {
            auto* window = *windowOpt; // 新建 窗口为自己 父亲
            if (window != nullptr && m_parent != nullptr && window == m_parent) {
                QHFNotifier noti(QHFNotifierType::NEW_APP_WINDOW_INFO_WORKSPACE);
                noti.set("workspace", this);
                QHFNotifierCenter::instance().publish(noti);
            }
        }
    }

    void QHFWorkspaceBase::handleAppWindowActivated(const QHFNotifier& notifier) {
        auto windowOpt = notifier.get<QWidget*>("window");
        if (windowOpt) {
            auto* window = *windowOpt;
            if (window != nullptr && m_parent != nullptr && window == m_parent) {
                QHFNotifier noti(QHFNotifierType::APP_WINDOW_ACTIVATED_WORKSPACE);
                // noti.set("window", window);
                noti.set("workspace", this);
                publishEvent(this, noti, 250);
            }
        }
    }

    void QHFWorkspaceBase::handleAppWindowMoved(const QHFNotifier& notifier) {
        //
        auto windowOpt = notifier.get<QWidget*>("window");
        if (windowOpt) {
            auto* window = *windowOpt;
            if (window != nullptr && m_parent != nullptr && window == m_parent) {
                QHFNotifier noti(QHFNotifierType::APP_WINDOW_MOVED_WORKSPACE);
                // noti.set("window", window);
                noti.set("workspace", this);
                QHFNotifierCenter::instance().publish(noti);
            }
        }
    }

    void QHFWorkspaceBase::handleReportActivation(const QHFNotifier& notifier) {
        //
        auto eventOpt = notifier.get<QHF::QHFActivationEvent>("event");
        if (eventOpt) {
            const auto& event = *eventOpt;
            if (event.type == QHFActivationEvent::Type::EDITOR_ACTIVE) {
                auto* editor = qobject_cast<QWidget*>(event.source);
                auto* node   = findNodeForWidget(editor);
                if (node != nullptr) {
                    setActiveNode(editor);
                    publishWorkspaceActive("editor reported", event.immediate);
                }
            }
        }
    }

    void QHFWorkspaceBase::handleSplitHorizontal(const QHFNotifier& notifier) {
        //
        publishWorkspaceActive("Split editor horizontally", false);
        //
        qDebug() << "HandleSplitHorizontalRequest";
        auto editorOpt = notifier.get<QWidget*>("editor");
        if (editorOpt) {
            auto* editor = *editorOpt;
            auto* node   = findNodeForWidget(editor);
            if (editor != nullptr && node == m_activeNode) {
                setActiveNode(editor);
                splitActive(EditorType::NONE, LayoutNode::Orientation::HORIZONTAL, false);
            }
        }
    }

    void QHFWorkspaceBase::handleSplitVertical(const QHFNotifier& notifier) {
        //
        publishWorkspaceActive("Split editor vertically", false);
        //
        qDebug() << "HandleSplitVerticalRequest";
        auto editorOpt = notifier.get<QWidget*>("editor");
        if (editorOpt) {
            auto* editor = *editorOpt;
            auto* node   = findNodeForWidget(editor);
            if (editor != nullptr && node == m_activeNode) {
                setActiveNode(editor);
                splitActive(EditorType::NONE, LayoutNode::Orientation::VERTICAL, false);
            }
        }
    }

    void QHFWorkspaceBase::handleRemove(const QHFNotifier& notifier) {
        //
        publishWorkspaceActive("Remove editor", false);
        //
        qDebug() << "handleRemoveRequest";
        auto editorOpt = notifier.get<QWidget*>("editor");
        if (editorOpt) {
            auto* editor = *editorOpt;
            auto* node   = findNodeForWidget(editor);
            if (editor != nullptr && node == m_activeNode) {
                setActiveNode(editor);
                removeActive(false);
            }
        }
    }

    void QHFWorkspaceBase::handleSwitchEditorType(const QHFNotifier& notifier) {
        //
        publishWorkspaceActive("Remove editor", false);
        //
        qDebug() << "handleSwitchEditorType";
        auto editorOpt = notifier.get<QWidget*>("editor");
        if (editorOpt) {
            auto* editor = *editorOpt;
            auto* node   = findNodeForWidget(editor);
            if (editor != nullptr && node != nullptr) {
                setActiveNode(editor);
                auto newTypeOpt = notifier.get<QString>("newType");
                if (newTypeOpt) {
                    const auto& newTypeStr = *newTypeOpt;
                    auto        newType    = EditorTypeUtils::fromString(newTypeStr);
                    if (newType != EditorType::NONE) {
                        switchActive(newType);
                    }
                }
            }
        }
    }

    void QHFWorkspaceBase::handleSwitchEditorModeType(const QHFNotifier& notifier) {
        //
        publishWorkspaceActive("Switch editor mode", false);
        //
        qDebug() << "handleSwitchEditorModeType";
        auto editorOpt = notifier.get<QWidget*>("editor");
        if (editorOpt) {
            auto* editor = *editorOpt;
            auto* node   = findNodeForWidget(editor);
            if (editor != nullptr && m_activeNode != nullptr && node == m_activeNode) {
                // editor 转换 IModeContexttype
                auto newModeOpt = notifier.get<QString>("newType");
                if (newModeOpt) {
                    const auto& newModeStr = *newModeOpt;
                    auto        newMode    = QHFModeTypeUtils::fromString(newModeStr);
                    if (newMode != QHFModeType::NONE) {
                        m_activeNode->modeType = newMode;
                    }
                }
            }
        }
    }

    void QHFWorkspaceBase::publishWorkspaceActive(const QString& reason, bool immediate) {
        QHFActivationEvent event(QHFActivationEvent::Type::WORKSPACE_ACTIVE, this, reason, immediate);
        event.context["workspace"] = QVariant::fromValue(this);
        QHFWorkspaceBase* workspace  = this;
        publishActivation(workspace, event);
    }

} // namespace QHF
