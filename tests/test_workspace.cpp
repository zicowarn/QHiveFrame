#include <QTest>
#include "../core/QHFWorkspaceBase.h"

using namespace QHF;

class TestWorkspace : public QObject {
    Q_OBJECT

  private slots:

    // ── LayoutNode construction ──
    void testLayoutNodeDefaultConstructor()
    {
        LayoutNode node;
        QCOMPARE(node.type, LayoutNode::Type::LEAF);
        QCOMPARE(node.editorType, EditorType::DUMMY);
        QVERIFY(node.firstChild == nullptr);
        QVERIFY(node.secondChild == nullptr);
    }

    void testLayoutNodeWithEditorType()
    {
        LayoutNode node(EditorType::NONE);
        QCOMPARE(node.editorType, EditorType::NONE);
        QCOMPARE(node.type, LayoutNode::Type::LEAF);
    }

    // ── LayoutNode split ──
    void testSplitLeafNode()
    {
        LayoutNode node(EditorType::DUMMY);
        node.split(LayoutNode::Orientation::HORIZONTAL, EditorType::DUMMY);

        QCOMPARE(node.type, LayoutNode::Type::SPLIT);
        QCOMPARE(node.orientation, LayoutNode::Orientation::HORIZONTAL);
        QVERIFY(node.firstChild != nullptr);
        QVERIFY(node.secondChild != nullptr);
        QCOMPARE(node.firstChild->type, LayoutNode::Type::LEAF);
        QCOMPARE(node.firstChild->editorType, EditorType::DUMMY);
        QCOMPARE(node.secondChild->type, LayoutNode::Type::LEAF);
        QCOMPARE(node.secondChild->editorType, EditorType::DUMMY);
    }

    void testSplitVertical()
    {
        LayoutNode node(EditorType::DUMMY);
        node.split(LayoutNode::Orientation::VERTICAL, EditorType::NONE);

        QCOMPARE(node.orientation, LayoutNode::Orientation::VERTICAL);
    }

    // ── LayoutNode clone ──
    void testCloneLeaf()
    {
        LayoutNode node(EditorType::DUMMY);
        LayoutNode* clone = node.clone();
        QVERIFY(clone != nullptr);
        QCOMPARE(clone->type, LayoutNode::Type::LEAF);
        QCOMPARE(clone->editorType, EditorType::DUMMY);
        QVERIFY(clone->firstChild == nullptr);
        QVERIFY(clone->secondChild == nullptr);
        delete clone;
    }

    void testCloneSplitNode()
    {
        LayoutNode node(EditorType::DUMMY);
        node.split(LayoutNode::Orientation::HORIZONTAL, EditorType::DUMMY);

        LayoutNode* clone = node.clone();
        QVERIFY(clone != nullptr);
        QCOMPARE(clone->type, LayoutNode::Type::SPLIT);
        QCOMPARE(clone->orientation, LayoutNode::Orientation::HORIZONTAL);
        QVERIFY(clone->firstChild != nullptr);
        QVERIFY(clone->secondChild != nullptr);
        QCOMPARE(clone->firstChild->editorType, EditorType::DUMMY);
        QCOMPARE(clone->secondChild->editorType, EditorType::DUMMY);

        // Verify deep copy: different pointers
        QVERIFY(clone->firstChild != node.firstChild);
        QVERIFY(clone->secondChild != node.secondChild);
        delete clone;
    }

    // ── Split ratio ──
    void testSplitRatioDefault()
    {
        LayoutNode node;
        node.split(LayoutNode::Orientation::HORIZONTAL, EditorType::DUMMY);
        // Default ratio should be 50:50
        QCOMPARE(node.splitRatio.first, 0.5f);
        QCOMPARE(node.splitRatio.second, 0.5f);
    }

    // ── Deep clone tree ──
    void testDeepCloneTree()
    {
        LayoutNode root(EditorType::DUMMY);
        root.split(LayoutNode::Orientation::HORIZONTAL, EditorType::NONE);
        // Split the first child further
        root.firstChild->split(LayoutNode::Orientation::VERTICAL, EditorType::DUMMY);

        LayoutNode* clone = root.clone();
        QVERIFY(clone != nullptr);
        QCOMPARE(clone->type, LayoutNode::Type::SPLIT);
        QVERIFY(clone->firstChild != nullptr);
        QCOMPARE(clone->firstChild->type, LayoutNode::Type::SPLIT);
        QCOMPARE(clone->firstChild->orientation, LayoutNode::Orientation::VERTICAL);
        QVERIFY(clone->firstChild->firstChild != nullptr);
        QCOMPARE(clone->firstChild->firstChild->editorType, EditorType::DUMMY);
        QCOMPARE(clone->firstChild->secondChild->editorType, EditorType::DUMMY);
        delete clone;
    }

    // ── EditorType lifetime ──
    void testEditorTypePersistence()
    {
        LayoutNode node(EditorType::DUMMY);
        QCOMPARE(node.editorType, EditorType::DUMMY);

        node.split(LayoutNode::Orientation::HORIZONTAL, EditorType::NONE);
        // Original editor type should be on first child
        QCOMPARE(node.firstChild->editorType, EditorType::DUMMY);
        QCOMPARE(node.secondChild->editorType, EditorType::NONE);
    }

    // ── Split on already-split node ──
    void testDoubleSplit()
    {
        LayoutNode node;
        node.split(LayoutNode::Orientation::HORIZONTAL, EditorType::DUMMY);
        // Split second child
        node.secondChild->split(LayoutNode::Orientation::VERTICAL, EditorType::DUMMY);

        QCOMPARE(node.secondChild->type, LayoutNode::Type::SPLIT);
        QVERIFY(node.secondChild->firstChild != nullptr);
        QVERIFY(node.secondChild->secondChild != nullptr);
    }
};

QTEST_MAIN(TestWorkspace)
#include "test_workspace.moc"
