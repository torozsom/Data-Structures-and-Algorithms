

#include "Tests.h"



/**
 * @brief Tests various functionalities of a binary tree implementation.
 *
 * This method evaluates the correctness of a BinaryTree class by performing the following actions:
 * - Verifying if the tree is initially empty by checking its height.
 * - Inserting a root node and verifying its properties through in-order traversal and height measurement.
 * - Adding nodes to the left subtree and testing the updated tree structure using in-order traversal and height analysis.
 * - Adding nodes to the right subtree and confirming the updated tree configuration through in-order traversal and height verification.
 * - Testing different traversal methods such as in-order, pre-order, and post-order to verify proper ordering of nodes in the tree.
 * - Clearing the tree to ensure all nodes are removed and confirming the state of the tree as empty.
 *
 * The function outputs intermediate results to assist in debugging and validation of the binary tree's behavior.
 */
void testBinaryTree() {
    BinaryTree<int> tree;

    std::cout << "\n\n==== Testing Binary Tree ====" << std::endl;

    // **Test Empty Tree**
    std::cout << "Is the tree empty initially? " << (tree.getHeight() == 0 ? "Yes" : "No") << std::endl;

    // **Insert Root Node**
    std::cout << "\n-- Inserting Root Node --" << std::endl;
    tree.insertLeft(10); // The first insert will become the root (if logic permits)
    std::cout << "In-Order Traversal after inserting root: ";
    tree.printInOrder();
    std::cout << "\nTree Height: " << tree.getHeight() << std::endl;

    // **Insert Left Nodes**
    std::cout << "\n-- Inserting Left Nodes --" << std::endl;
    tree.insertLeft(5);
    tree.insertLeft(2);
    std::cout << "In-Order Traversal after inserting left nodes: ";
    tree.printInOrder();
    std::cout << "\nTree Height: " << tree.getHeight() << std::endl;

    // **Insert Right Nodes**
    std::cout << "\n-- Inserting Right Nodes --" << std::endl;
    tree.insertRight(15);
    tree.insertRight(20);
    std::cout << "In-Order Traversal after inserting right nodes: ";
    tree.printInOrder();
    std::cout << "\nTree Height: " << tree.getHeight() << std::endl;

    // **Test Different Traversals**
    std::cout << "\n-- Traversals --" << std::endl;
    std::cout << "In-Order Traversal: ";
    tree.printInOrder();
    std::cout << "\nPre-Order Traversal: ";
    tree.printPreOrder();
    std::cout << "\nPost-Order Traversal: ";
    tree.printPostOrder();
    std::cout << "\nTree Height: " << tree.getHeight() << std::endl;

    // **Test Tree Clearing**
    std::cout << "\n-- Clearing the Tree --" << std::endl;
    tree.clear();
    std::cout << "Tree cleared.\n";
    std::cout << "Is the tree empty after clearing? " << (tree.getHeight() == 0 ? "Yes" : "No") << std::endl;
}
