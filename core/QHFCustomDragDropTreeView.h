#ifndef QHFCUSTOMDRAGDROPTREEVIEW_H
#define QHFCUSTOMDRAGDROPTREEVIEW_H

#include <QTreeView>

namespace QHF {

    /**
     * @brief  (see description)
     */
    enum class QHFTreeType : std::uint8_t {
        MODEL_TREE,   ///< 模型树
        PROCESS_TREE,///< 工艺树
        GENERAL_TREE,///< General树
        NONE          ///< None树
    };

    /**
     * @brief Custom (see description)
     * @details (see source)
     */
    class QHFCustomDragDropTreeView : public QTreeView {
        Q_OBJECT

      signals:
        void onDragDropStarted(QHFTreeType treeType, QModelIndex index);
        void onDragDropCompleted(QHFTreeType treeType, QModelIndex index);
        void onDragDropAborted();

      private:
        QHFTreeType m_treeType;

      public:
        explicit QHFCustomDragDropTreeView(QWidget* parent = nullptr);
        void setTreeType(QHFTreeType type);
        [[nodiscard]] QHFTreeType getTreeType() const;

      protected:
        void startDrag(Qt::DropActions supportedActions) override;
        void dragEnterEvent(QDragEnterEvent* event) override;
        void dragMoveEvent(QDragMoveEvent* event) override;
        void dropEvent(QDropEvent* event) override;
    };
} // namespace QHF

#endif // HCUSTOMDRAGDROPTREEVIEW_H
