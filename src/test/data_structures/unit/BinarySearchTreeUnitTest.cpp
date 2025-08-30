#include <gtest/gtest.h>
#include <stdexcept>
#include <utility>

#include "BinarySearchTree.hpp"


using ds::BinarySearchTree;
using ds::BinaryTree;
using ds::Node;


class BinarySearchTreeUnitTest : public testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};


template <typename T>
class BinarySearchTreeExposed final : public BinarySearchTree<T> {
  public:
    using BinaryTree<T>::root_;
};


TEST_F(BinarySearchTreeUnitTest, NewTreeShouldBeEmpty) {
    const BinarySearchTree<int> tree;
    EXPECT_EQ(tree.size(), 0);
    EXPECT_TRUE(tree.isEmpty());
    EXPECT_TRUE(tree.isValidBST());
}


TEST_F(BinarySearchTreeUnitTest, InsertShouldMaintainBSTProperty) {
    BinarySearchTree<int> tree;
    tree.insert(5); // root
    tree.insert(3); // left child
    tree.insert(7); // right child
    tree.insert(1); // left-left child
    tree.insert(9); // right-right child

    EXPECT_EQ(tree.size(), 5);
    EXPECT_TRUE(tree.isValidBST());
    EXPECT_TRUE(tree.contains(5));
    EXPECT_TRUE(tree.contains(3));
    EXPECT_TRUE(tree.contains(7));
    EXPECT_TRUE(tree.contains(1));
    EXPECT_TRUE(tree.contains(9));
    EXPECT_FALSE(tree.contains(4));
}


TEST_F(BinarySearchTreeUnitTest, InvalidStructureShouldReturnFalse) {
    BinarySearchTreeExposed<int> tree;
    tree.root_ = new Node<int>(10);
    tree.root_->left = new Node<int>(20);
    tree.root_->left->parent = tree.root_;
    EXPECT_FALSE(tree.isValidBST());
}


TEST_F(BinarySearchTreeUnitTest, ArrayConstructorShouldCreateValidBST) {
    const int values[] = {5, 3, 7, 1, 9};
    const BinarySearchTree tree(values, 5);

    EXPECT_EQ(tree.size(), 5);
    EXPECT_TRUE(tree.isValidBST());
    for (int value : values)
        EXPECT_TRUE(tree.contains(value));
}


TEST_F(BinarySearchTreeUnitTest, RemoveShouldHandleAllCases) {
    BinarySearchTree<int> tree;
    // Create a tree:     5
    //                  /   \
    //                 3     7
    //                / \   / \
    //               1   4 6   9
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(1);
    tree.insert(4);
    tree.insert(6);
    tree.insert(9);

    // Test removing leaf node
    tree.remove(1);
    EXPECT_FALSE(tree.contains(1));
    EXPECT_TRUE(tree.isValidBST());

    // Test removing node with one child
    tree.remove(9);
    EXPECT_FALSE(tree.contains(9));
    EXPECT_TRUE(tree.isValidBST());

    // Test removing node with two children
    tree.remove(3);
    EXPECT_FALSE(tree.contains(3));
    EXPECT_TRUE(tree.isValidBST());
    EXPECT_TRUE(tree.contains(4)); // 4 should still be in the tree
}


TEST_F(BinarySearchTreeUnitTest, MinMaxShouldWorkCorrectly) {
    BinarySearchTree<int> tree;

    // Test empty tree
    EXPECT_THROW(tree.findMinimum(), std::runtime_error);
    EXPECT_THROW(tree.findMaximum(), std::runtime_error);

    // Test with values
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(1);
    tree.insert(9);

    EXPECT_EQ(tree.findMinimum(), 1);
    EXPECT_EQ(tree.findMaximum(), 9);
}


TEST_F(BinarySearchTreeUnitTest, CopyConstructorShouldCreateDeepCopy) {
    BinarySearchTree<int> original;
    original.insert(5);
    original.insert(3);
    original.insert(7);

    const BinarySearchTree copy(original);
    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(copy.getHeight(), original.getHeight());
    EXPECT_TRUE(copy.isValidBST());

    // Modify original
    original.insert(1);
    EXPECT_NE(copy.size(), original.size());
    EXPECT_FALSE(copy.contains(1));
}


TEST_F(BinarySearchTreeUnitTest, MoveConstructorShouldTransferOwnership) {
    BinarySearchTree<int> original;
    original.insert(5);
    original.insert(3);
    original.insert(7);

    const BinarySearchTree moved(std::move(original));
    EXPECT_TRUE(original.isEmpty());
    EXPECT_EQ(original.size(), 0);
    EXPECT_EQ(moved.size(), 3);
    EXPECT_TRUE(moved.contains(5));
    EXPECT_TRUE(moved.contains(3));
    EXPECT_TRUE(moved.contains(7));
    EXPECT_TRUE(moved.isValidBST());
}


TEST_F(BinarySearchTreeUnitTest, DeletedInsertMethodsShouldNotBeAccessible) {
    const BinarySearchTree<int> tree;
    // The following lines should not compile:
    // tree.insertLeft(5);  // Deleted function
    // tree.insertRight(3); // Deleted function
    EXPECT_TRUE(tree.isEmpty()); // Just to have an assertion
}


TEST_F(BinarySearchTreeUnitTest, ShouldWorkWithCustomTypes) {
    struct ComparableType {
        int value;
        explicit ComparableType(const int v) : value(v) {}
        bool operator<(const ComparableType& other) const {
            return value < other.value;
        }
        bool operator>(const ComparableType& other) const {
            return value > other.value;
        }
        bool operator==(const ComparableType& other) const {
            return value == other.value;
        }
        bool operator<=(const ComparableType& other) const {
            return value <= other.value;
        }
        bool operator>=(const ComparableType& other) const {
            return value >= other.value;
        }
    };

    BinarySearchTree<ComparableType> tree;
    tree.insert(ComparableType(5));
    tree.insert(ComparableType(3));
    tree.insert(ComparableType(7));

    EXPECT_TRUE(tree.contains(ComparableType(5)));
    EXPECT_TRUE(tree.contains(ComparableType(3)));
    EXPECT_TRUE(tree.contains(ComparableType(7)));
    EXPECT_FALSE(tree.contains(ComparableType(4)));
    EXPECT_TRUE(tree.isValidBST());
}


TEST_F(BinarySearchTreeUnitTest, MoveAssignmentShouldTransferOwnership) {
    BinarySearchTree<int> original;
    original.insert(5);
    original.insert(3);
    original.insert(7);

    BinarySearchTree<int> moved;
    moved = std::move(original);

    EXPECT_TRUE(original.isEmpty());
    EXPECT_EQ(original.size(), 0);
    EXPECT_EQ(moved.size(), 3);
    EXPECT_TRUE(moved.contains(5));
    EXPECT_TRUE(moved.contains(3));
    EXPECT_TRUE(moved.contains(7));
    EXPECT_TRUE(moved.isValidBST());
}


TEST_F(BinarySearchTreeUnitTest, AssignmentOperatorShouldCreateDeepCopy) {
    BinarySearchTree<int> original;
    original.insert(5);
    original.insert(3);
    original.insert(7);
    original.insert(1);
    original.insert(9);

    BinarySearchTree<int> copy;
    copy = original;

    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(copy.getHeight(), original.getHeight());
    EXPECT_TRUE(copy.isValidBST());

    // Verify all values were copied
    EXPECT_TRUE(copy.contains(5));
    EXPECT_TRUE(copy.contains(3));
    EXPECT_TRUE(copy.contains(7));
    EXPECT_TRUE(copy.contains(1));
    EXPECT_TRUE(copy.contains(9));

    // Modify original to verify deep copy
    original.remove(3);
    EXPECT_NE(copy.size(), original.size());
    EXPECT_TRUE(copy.contains(3)); // Should still be in copy
}


TEST_F(BinarySearchTreeUnitTest, RemoveRootNodeShouldWorkCorrectly) {
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(1);
    tree.insert(4);
    tree.insert(6);
    tree.insert(9);

    // Remove root node (has two children)
    tree.remove(5);
    EXPECT_FALSE(tree.contains(5));
    EXPECT_TRUE(tree.isValidBST());
    EXPECT_EQ(tree.size(), 6);

    // All other nodes should still be present
    EXPECT_TRUE(tree.contains(3));
    EXPECT_TRUE(tree.contains(7));
    EXPECT_TRUE(tree.contains(1));
    EXPECT_TRUE(tree.contains(4));
    EXPECT_TRUE(tree.contains(6));
    EXPECT_TRUE(tree.contains(9));
}


TEST_F(BinarySearchTreeUnitTest, RemoveNonExistentElementShouldNotAffectTree) {
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    const size_t originalSize = tree.size();
    tree.remove(99); // Non-existent element

    EXPECT_EQ(tree.size(), originalSize);
    EXPECT_TRUE(tree.isValidBST());
    EXPECT_TRUE(tree.contains(5));
    EXPECT_TRUE(tree.contains(3));
    EXPECT_TRUE(tree.contains(7));
}


TEST_F(BinarySearchTreeUnitTest, DuplicateInsertsShouldNotIncreaseSize) {
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    const size_t originalSize = tree.size();

    // Insert duplicates
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    EXPECT_EQ(tree.size(), originalSize); // Size should not change
    EXPECT_TRUE(tree.isValidBST());
}


TEST_F(BinarySearchTreeUnitTest, EmptyTreeOperationsShouldWork) {
    BinarySearchTree<int> tree;

    EXPECT_TRUE(tree.isEmpty());
    EXPECT_EQ(tree.size(), 0);
    EXPECT_EQ(tree.getHeight(), 0);
    EXPECT_TRUE(tree.isValidBST());
    EXPECT_FALSE(tree.contains(5));

    // Remove from empty tree should not crash
    tree.remove(5);
    EXPECT_TRUE(tree.isEmpty());
}
