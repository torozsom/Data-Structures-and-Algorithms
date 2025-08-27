#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include <utility>

#include "BinaryTree.hpp"
#include "Record.hpp"


using data_structs::BinaryTree;
using data_structs::Node;


class BinaryTreeUnitTest : public testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};


TEST_F(BinaryTreeUnitTest, NewTreeShouldBeEmpty) {
    const BinaryTree<int> tree;
    EXPECT_EQ(tree.size(), 0);
    EXPECT_TRUE(tree.isEmpty());
}


TEST_F(BinaryTreeUnitTest, HeightOfEmptyTreeShouldBeZero) {
    const BinaryTree<int> tree;
    EXPECT_EQ(tree.getHeight(), 0);
}


TEST_F(BinaryTreeUnitTest, HeightShouldReflectTreeStructure) {
    const BinaryTree<int> tree;
    const int values[] = {1, 2, 3, 4, 5};
    const BinaryTree filledTree(values, 5);

    EXPECT_EQ(tree.getHeight(), 0);
    // Level-order insertion will create a tree of height 3
    EXPECT_EQ(filledTree.getHeight(), 3);
}


TEST_F(BinaryTreeUnitTest, ClearShouldRemoveAllNodes) {
    const int values[] = {1, 2, 3, 4, 5};
    BinaryTree tree(values, 5);
    EXPECT_EQ(tree.size(), 5);

    tree.clear();
    EXPECT_TRUE(tree.isEmpty());
    EXPECT_EQ(tree.size(), 0);
    EXPECT_EQ(tree.getHeight(), 0);
}


TEST_F(BinaryTreeUnitTest, ContainsShouldFindExistingValues) {
    const int values[] = {1, 2, 3, 4, 5};
    const BinaryTree tree(values, 5);

    EXPECT_TRUE(tree.contains(1));  // root
    EXPECT_TRUE(tree.contains(2));  // left child
    EXPECT_TRUE(tree.contains(3));  // right child
    EXPECT_TRUE(tree.contains(4));  // left leaf
    EXPECT_TRUE(tree.contains(5));  // right leaf
    EXPECT_FALSE(tree.contains(6)); // non-existent value
}


TEST_F(BinaryTreeUnitTest, LevelOrderConstructionShouldWorkCorrectly) {
    const int values[] = {1, 2, 3, 4, 5};
    const BinaryTree tree(values, 5);

    EXPECT_EQ(tree.size(), 5);
    EXPECT_TRUE(tree.contains(1)); // Level 1 (root)
    EXPECT_TRUE(tree.contains(2)); // Level 2
    EXPECT_TRUE(tree.contains(3)); // Level 2
    EXPECT_TRUE(tree.contains(4)); // Level 3
    EXPECT_TRUE(tree.contains(5)); // Level 3
}


TEST_F(BinaryTreeUnitTest, CopyConstructorShouldCreateDeepCopy) {
    const int values[] = {1, 2, 3, 4, 5};
    BinaryTree original(values, 5);
    const BinaryTree copy(original);

    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(copy.getHeight(), original.getHeight());

    // Verify all values were copied
    for (int i = 1; i <= 5; ++i)
        EXPECT_TRUE(copy.contains(i));

    // Verify deep copy by clearing original
    original.clear();
    EXPECT_EQ(original.size(), 0);
    EXPECT_EQ(copy.size(), 5);
}


TEST_F(BinaryTreeUnitTest, MoveConstructorShouldTransferOwnership) {
    constexpr int values[] = {1, 2, 3};
    BinaryTree original(values, 3);
    EXPECT_EQ(original.size(), 3);

    const BinaryTree moved(std::move(original));
    EXPECT_TRUE(original.isEmpty());
    EXPECT_EQ(original.size(), 0);
    EXPECT_EQ(moved.size(), 3);
    EXPECT_TRUE(moved.contains(1));
    EXPECT_TRUE(moved.contains(2));
    EXPECT_TRUE(moved.contains(3));
}


TEST_F(BinaryTreeUnitTest, MoveAssignmentShouldTransferOwnership) {
    constexpr int values[] = {1, 2, 3};
    BinaryTree original(values, 3);
    BinaryTree<int> moved;

    moved = std::move(original);
    EXPECT_TRUE(original.isEmpty());
    EXPECT_EQ(original.size(), 0);
    EXPECT_EQ(moved.size(), 3);
    EXPECT_TRUE(moved.contains(1));
    EXPECT_TRUE(moved.contains(2));
    EXPECT_TRUE(moved.contains(3));
}


TEST_F(BinaryTreeUnitTest, AssignmentOperatorShouldCreateDeepCopy) {
    const int values[] = {1, 2, 3, 4, 5};
    BinaryTree original(values, 5);
    BinaryTree<int> copy;

    copy = original;
    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(copy.getHeight(), original.getHeight());

    // Verify all values were copied
    for (int i = 1; i <= 5; ++i)
        EXPECT_TRUE(copy.contains(i));

    // Verify deep copy by clearing original
    original.clear();
    EXPECT_EQ(original.size(), 0);
    EXPECT_EQ(copy.size(), 5);
}


TEST_F(BinaryTreeUnitTest, ShouldWorkWithNonTrivialTypes) {
    const std::string values[] = {"Hello", "World", "Test"};
    const BinaryTree tree(values, 3);

    EXPECT_EQ(tree.size(), 3);
    EXPECT_TRUE(tree.contains("Hello"));
    EXPECT_TRUE(tree.contains("World"));
    EXPECT_TRUE(tree.contains("Test"));
    EXPECT_FALSE(tree.contains("NotFound"));
}


TEST_F(BinaryTreeUnitTest, InsertMethodsShouldWorkCorrectly) {
    BinaryTree<int> tree;

    // Test insertLeft
    tree.insertLeft(10);
    EXPECT_TRUE(tree.contains(10));

    tree.insertLeft(5);
    EXPECT_TRUE(tree.contains(5));

    tree.insertLeft(15);
    EXPECT_TRUE(tree.contains(15));

    tree.insertLeft(20);
    EXPECT_TRUE(tree.contains(20));

    // Test insertRight
    tree.insertRight(3);
    EXPECT_TRUE(tree.contains(3));

    tree.insertRight(7);
    EXPECT_TRUE(tree.contains(7));

    tree.insertRight(12);
    EXPECT_TRUE(tree.contains(12));

    tree.insertRight(17);
    EXPECT_TRUE(tree.contains(17));

    EXPECT_EQ(tree.size(), 8);
}


TEST_F(BinaryTreeUnitTest, SelfAssignmentShouldMaintainTree) {
    const int values[] = {1, 2, 3, 4, 5};
    BinaryTree tree(values, 5);

    const auto originalSize = tree.size();
    const auto originalHeight = tree.getHeight();

    tree = tree;

    EXPECT_EQ(tree.size(), originalSize);
    EXPECT_EQ(tree.getHeight(), originalHeight);

    for (const int value : values)
        EXPECT_TRUE(tree.contains(value));
}


TEST_F(BinaryTreeUnitTest, SelfMoveAssignmentShouldMaintainTree) {
    const int values[] = {1, 2, 3, 4, 5};
    BinaryTree tree(values, 5);

    const auto originalSize = tree.size();
    const auto originalHeight = tree.getHeight();

    tree = std::move(tree);

    EXPECT_EQ(tree.size(), originalSize);
    EXPECT_EQ(tree.getHeight(), originalHeight);

    for (const int value : values)
        EXPECT_TRUE(tree.contains(value));
}


TEST_F(BinaryTreeUnitTest, FindNodeMethodsShouldWorkCorrectly) {
    const int values[] = {1, 2, 3, 4, 5};
    const BinaryTree tree(values, 5);

    // Test findNode
    const Node<int>* node = tree.findNode(3);
    EXPECT_NE(node, nullptr);
    EXPECT_EQ(node->data, 3);

    // Test findNodeLevelOrder
    const Node<int>* levelNode = tree.findNodeLevelOrder(4);
    EXPECT_NE(levelNode, nullptr);
    EXPECT_EQ(levelNode->data, 4);

    // Test non-existent value
    EXPECT_EQ(tree.findNode(99), nullptr);
    EXPECT_EQ(tree.findNodeLevelOrder(99), nullptr);
}


TEST_F(BinaryTreeUnitTest, FindNodeLevelOrderEmptyTree) {
    const BinaryTree<int> tree;
    EXPECT_EQ(tree.findNodeLevelOrder(42), nullptr);
}


TEST_F(BinaryTreeUnitTest, IsCompleteTreeShouldWorkCorrectly) {
    // Empty tree is complete
    const BinaryTree<int> emptyTree;
    EXPECT_TRUE(emptyTree.isCompleteTree());

    // Single node is complete
    BinaryTree<int> singleNode;
    singleNode.insert(1);
    EXPECT_TRUE(singleNode.isCompleteTree());

    // Complete tree with level-order insertion
    const int values[] = {1, 2, 3, 4, 5};
    const BinaryTree completeTree(values, 5);
    EXPECT_TRUE(completeTree.isCompleteTree());
}


TEST_F(BinaryTreeUnitTest, IncompleteTreeShouldBeDetected) {
    BinaryTree<int> tree;
    tree.insertRight(1); // root
    tree.insertRight(2); // only right child, left is missing
    EXPECT_FALSE(tree.isCompleteTree());
}


TEST_F(BinaryTreeUnitTest, ShouldStoreReferences) {
    int x = 5;
    BinaryTree<Record> tree;
    tree.insert(Record{x});
    const Node<Record>* root = tree.getRoot();
    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->data.value, 5);
    x = 10;
    EXPECT_EQ(root->data.value, 10);
}


TEST_F(BinaryTreeUnitTest, ConstructorThrowsOnNullArray) {
    EXPECT_THROW(BinaryTree<int>(nullptr, 3), std::invalid_argument);
}


TEST_F(BinaryTreeUnitTest, NullArrayWithZeroSizeSucceeds) {
    const BinaryTree<int> tree(nullptr, 0);
    EXPECT_TRUE(tree.isEmpty());
    EXPECT_EQ(tree.size(), 0);
}
