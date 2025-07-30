#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include <string>

#include "BinaryTree.hpp"


class BinaryTreeUnitTest : public ::testing::Test {
  protected:
    void SetUp() override {
        // Setup code that runs before each test
    }

    void TearDown() override {
        // Cleanup code that runs after each test
    }
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
    EXPECT_EQ(filledTree.getHeight(),
              3); // Level-order insertion will create a tree of height 3
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

    EXPECT_TRUE(tree.containsNode(1));  // root
    EXPECT_TRUE(tree.containsNode(2));  // left child
    EXPECT_TRUE(tree.containsNode(3));  // right child
    EXPECT_TRUE(tree.containsNode(4));  // left leaf
    EXPECT_TRUE(tree.containsNode(5));  // right leaf
    EXPECT_FALSE(tree.containsNode(6)); // non-existent value
}


TEST_F(BinaryTreeUnitTest, LevelOrderConstructionShouldWorkCorrectly) {
    const int values[] = {1, 2, 3, 4, 5};
    const BinaryTree tree(values, 5);

    EXPECT_EQ(tree.size(), 5);
    EXPECT_TRUE(tree.containsNode(1)); // Level 1 (root)
    EXPECT_TRUE(tree.containsNode(2)); // Level 2
    EXPECT_TRUE(tree.containsNode(3)); // Level 2
    EXPECT_TRUE(tree.containsNode(4)); // Level 3
    EXPECT_TRUE(tree.containsNode(5)); // Level 3
}


TEST_F(BinaryTreeUnitTest, CopyConstructorShouldCreateDeepCopy) {
    const int values[] = {1, 2, 3, 4, 5};
    BinaryTree original(values, 5);
    const BinaryTree copy(original);

    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(copy.getHeight(), original.getHeight());

    // Verify all values were copied
    for (int i = 1; i <= 5; ++i)
        EXPECT_TRUE(copy.containsNode(i));

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
    EXPECT_TRUE(moved.containsNode(1));
    EXPECT_TRUE(moved.containsNode(2));
    EXPECT_TRUE(moved.containsNode(3));
}


TEST_F(BinaryTreeUnitTest, MoveAssignmentShouldTransferOwnership) {
    constexpr int values[] = {1, 2, 3};
    BinaryTree original(values, 3);
    BinaryTree<int> moved;

    moved = std::move(original);
    EXPECT_TRUE(original.isEmpty());
    EXPECT_EQ(original.size(), 0);
    EXPECT_EQ(moved.size(), 3);
    EXPECT_TRUE(moved.containsNode(1));
    EXPECT_TRUE(moved.containsNode(2));
    EXPECT_TRUE(moved.containsNode(3));
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
        EXPECT_TRUE(copy.containsNode(i));

    // Verify deep copy by clearing original
    original.clear();
    EXPECT_EQ(original.size(), 0);
    EXPECT_EQ(copy.size(), 5);
}


TEST_F(BinaryTreeUnitTest, ShouldWorkWithNonTrivialTypes) {
    const std::string values[] = {"Hello", "World", "Test"};
    const BinaryTree tree(values, 3);

    EXPECT_EQ(tree.size(), 3);
    EXPECT_TRUE(tree.containsNode("Hello"));
    EXPECT_TRUE(tree.containsNode("World"));
    EXPECT_TRUE(tree.containsNode("Test"));
    EXPECT_FALSE(tree.containsNode("NotFound"));
}


TEST_F(BinaryTreeUnitTest, TraversalMethodsShouldNotCrashOnEmptyTree) {
    const BinaryTree<int> tree;
    // These should not throw or crash
    tree.printInOrder();
    tree.printPreOrder();
    tree.printPostOrder();
    tree.printLevelOrder();
}

TEST_F(BinaryTreeUnitTest, InsertMethodsShouldWorkCorrectly) {
    BinaryTree<int> tree;

    // Test insertLeft
    tree.insertLeft(10);
    EXPECT_EQ(tree.size(), 1);
    EXPECT_TRUE(tree.containsNode(10));

    tree.insertLeft(5);
    EXPECT_EQ(tree.size(), 2);
    EXPECT_TRUE(tree.containsNode(5));

    // Test insertRight
    tree.insertRight(15);
    EXPECT_EQ(tree.size(), 3);
    EXPECT_TRUE(tree.containsNode(15));
}

TEST_F(BinaryTreeUnitTest, FindNodeMethodsShouldWorkCorrectly) {
    const int values[] = {1, 2, 3, 4, 5};
    const BinaryTree tree(values, 5);

    // Test findNode
    Node<int>* node = tree.findNode(3);
    EXPECT_NE(node, nullptr);
    EXPECT_EQ(node->data, 3);

    // Test findNodeLevelOrder
    Node<int>* levelNode = tree.findNodeLevelOrder(4);
    EXPECT_NE(levelNode, nullptr);
    EXPECT_EQ(levelNode->data, 4);

    // Test non-existent value
    EXPECT_EQ(tree.findNode(99), nullptr);
    EXPECT_EQ(tree.findNodeLevelOrder(99), nullptr);
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
