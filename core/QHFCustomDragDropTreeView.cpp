#include "QHFCustomDragDropTreeView.h"

#include <QAbstractItemModel>
#include <QDebug>
#include <QDrag>
#include <QDragMoveEvent>
#include <QMimeData>
#include <QModelIndexList>
#include <QPainter>
#include <QPixmap>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>

namespace QHF {
    /// --- QHFCustomDragDropTreeView implementation ---
    QHFCustomDragDropTreeView::QHFCustomDragDropTreeView(QWidget* parent) : QTreeView(parent) {
        // InitializesMember variable
        m_treeType = QHFTreeType::GENERAL_TREE;
        // setallow拖拽
        setDragDropMode(QAbstractItemView::DragDrop);
        // setallow拖拽
        setDropIndicatorShown(true);
        // setallow拖拽
        setDragDropOverwriteMode(false);
    }

    void QHFCustomDragDropTreeView::setTreeType(QHFTreeType type) { m_treeType = type; }

    QHFTreeType QHFCustomDragDropTreeView::getTreeType() const { return m_treeType; }

    void QHFCustomDragDropTreeView::startDrag(Qt::DropActions supportedActions) {
        // Returns current 选中项
        QModelIndex currentIndex = this->selectionModel()->currentIndex();
        if (!currentIndex.isValid()) { // 无效 索引
            return;
        }

        // checktype
        if (m_treeType == QHFTreeType::PROCESS_TREE) {
            // via model() get关联 模型
            auto* currentModel = model();
            if (currentModel == nullptr) {
                return;
            }
            // Returns current 目标项
            auto* standardModel = qobject_cast<QStandardItemModel*>(currentModel);
            if (standardModel == nullptr) {
                return;
            }
            auto* dragedItem = standardModel->itemFromIndex(currentIndex);
            // ensure目标项valid
            if (dragedItem == nullptr) {
                return;
            }
            // 只allow放置在非root node child nodetop, determine该dragedItem是否有parent node
            if (dragedItem->parent() == nullptr) {
                return;
            }
        }
        //
        emit onDragDropStarted(m_treeType, currentIndex);
        // default父拖拽操
        QModelIndexList indexes = selectionModel()->selectedIndexes();
        if (indexes.isEmpty()) {
            return;
        }

        auto* data = new QMimeData();
        data->setText("Some text data");

        /// NOLINTNEXTLINE(readability-magic-numbers)
        QPixmap pixmap(80, 20); // 自定义拖拽 图像
        pixmap.fill(Qt::white);

        QPainter painter(&pixmap);
        // 绘制拖拽 图像
        painter.drawText(pixmap.rect(), Qt::AlignLeft, tr("Dragging..."));
        painter.end();

        auto* drag = new QDrag(this);
        drag->setPixmap(pixmap);
        drag->setMimeData(data);
        drag->setHotSpot(QPoint(pixmap.width() / 2, pixmap.height() / 2));

        if (drag->exec(supportedActions, Qt::MoveAction) == Qt::MoveAction) {
            return; // 清理拖拽后 选中项
        }
    }

    void QHFCustomDragDropTreeView::dragEnterEvent(QDragEnterEvent* event) {
        // checkitselftype
        if (m_treeType == QHFTreeType::MODEL_TREE) {
            // check源是否为自己
            if (event->source() == this) {
                event->ignore(); // 允许放置 指示
                return;
            }
        }
        if (m_treeType == QHFTreeType::PROCESS_TREE) {
            // check源是否为自己
            if (event->source() == this) {
                event->ignore(); // 允许放置 指示
                return;
            }
        }

        // allow放置 指示
        event->accept();
    }

    void QHFCustomDragDropTreeView::dragMoveEvent(QDragMoveEvent* event) {
        // Returns current 拖拽 node
        QModelIndex targetIndex = indexAt(event->pos());
        if (!targetIndex.isValid()) { // 无效 拖拽位置
            event->ignore();
            return;
        }

        if (m_treeType == QHFTreeType::MODEL_TREE) {

            // via model() get关联 模型
            auto* currentModel = model();
            if (currentModel == nullptr) {
                event->ignore();
                return;
            }
            // 原始type为CustomFilterProxyModel
            auto* filterModel = qobject_cast<QSortFilterProxyModel*>(currentModel);
            if (filterModel == nullptr) {
                event->ignore();
                return;
            }
            // 得到 原始  index
            auto sourceIndex = filterModel->mapToSource(targetIndex);
            // 将 filterModel 转换为 QStandardItemModel
            auto* standardModel = qobject_cast<QStandardItemModel*>(filterModel->sourceModel());
            if (standardModel == nullptr) {
                event->ignore();
                return;
            }
            auto* targetItem = standardModel->itemFromIndex(sourceIndex);
            // ensure目标项valid
            if (targetItem == nullptr) {
                event->ignore();
                return;
            }
            // 只allow放置在root nodetop
            if (targetItem->parent() == nullptr) {
                event->ignore(); // 阻止放置 指示
                return;
            }
        } else if (m_treeType == QHFTreeType::PROCESS_TREE) {
            // via model() get关联 模型
            auto* currentModel = model();
            if (currentModel == nullptr) {
                event->ignore();
                return;
            }
            // Returns current 目标项
            auto* standardModel = qobject_cast<QStandardItemModel*>(currentModel);
            if (standardModel == nullptr) {
                event->ignore();
                return;
            }
            auto* targetItem = standardModel->itemFromIndex(targetIndex);
            // ensure目标项valid
            if (targetItem == nullptr) {
                event->ignore();
                return;
            }
            // 只allow放置在非root node child nodetop
            if (targetItem->parent() == nullptr) {
                event->ignore(); // 阻止放置 指示
                return;
            }
            // 只allow放置在非child node child nodetop
            if (targetItem->parent()->parent() == nullptr) {
                event->ignore(); // 阻止放置 指示
                return;
            }
        }
        event->accept(); // 允许放置 指示
    }

    void QHFCustomDragDropTreeView::dropEvent(QDropEvent* event) {
        // Returns current 拖拽 node
        QModelIndex targetIndex = indexAt(event->pos());
        if (!targetIndex.isValid()) { // 无效 拖拽位置
            emit onDragDropAborted();
            event->ignore(); // 阻止放置
            return;
        }
        if (m_treeType == QHFTreeType::MODEL_TREE) {
            // via model() get关联 模型
            auto* currentModel = model();
            if (currentModel == nullptr) {
                emit onDragDropAborted();
                event->ignore();
                return;
            }
            // 原始type为CustomFilterProxyModel 为 QSortFilterProxyModel subclass
            auto* filterModel = qobject_cast<QSortFilterProxyModel*>(currentModel);
            if (filterModel == nullptr) {
                event->ignore();
                emit onDragDropAborted();
                return;
            }
            // 得到 原始  index
            auto sourceIndex = filterModel->mapToSource(targetIndex);
            // 将 filterModel 转换为 QStandardItemModel
            auto* standardModel = qobject_cast<QStandardItemModel*>(filterModel->sourceModel());
            if (standardModel == nullptr) {
                emit onDragDropAborted();
                event->ignore();
                return;
            }
            auto* targetItem = standardModel->itemFromIndex(sourceIndex);
            // ensure目标项valid
            if (targetItem == nullptr) {
                emit onDragDropAborted();
                event->ignore();
                return;
            }
            // 只allow放置在root nodetop
            if (targetItem->parent() == nullptr) {
                emit onDragDropAborted();
                event->ignore(); // 阻止放置 指示
                return;
            }
            // get event 中 data
            emit onDragDropCompleted(m_treeType, targetIndex);
        } else if (m_treeType == QHFTreeType::PROCESS_TREE) {
            // via model() get关联 模型
            auto* currentModel = model();
            if (currentModel == nullptr) {
                emit onDragDropAborted();
                event->ignore(); // 阻止放置
                return;
            }

            // Returns current 目标项
            auto* standardModel = qobject_cast<QStandardItemModel*>(currentModel);
            if (standardModel == nullptr) {
                emit onDragDropAborted();
                event->ignore(); // 阻止放置
                return;
            }

            auto* targetItem = standardModel->itemFromIndex(targetIndex);
            // ensure目标项valid
            if (targetItem == nullptr) {
                emit onDragDropAborted();
                event->ignore(); // 阻止放置
                return;
            }

            // 如果目标为empty或目标是root node (即没有parent node), 则取消放置
            if (targetItem->parent() == nullptr) {
                //
                emit onDragDropAborted();
                event->ignore(); // 阻止放置
                return;
            }

            // get event 中 data
            emit onDragDropCompleted(m_treeType, targetIndex);
        }
        event->accept(); // 允许放置 指示
    }

} // namespace QHF
