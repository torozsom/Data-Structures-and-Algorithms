//
// Created by toron on 2024. 12. 26..
//

#ifndef BINARYSEARCHTREE_HPP
#define BINARYSEARCHTREE_HPP

#include <iostream>
#include "BinaryTree.hpp"


/**
 * @class BinarySearchTree
 * A binary search tree data structure that stores elements of a given type.
 *
 * The binary search tree is a binary tree data structure in which each node has at most two children,
 * referred to as the left child and the right child. The tree is defined recursively,
 * with each child node being the root of its own subtree.
 *
 * The binary search tree is a special type of binary tree in which the elements are ordered in a specific way.
 * For each node, all elements in the left subtree are less than the node's element, and all elements in the right
 * subtree are greater than the node's element. This ordering property allows for efficient search, insertion,
 * and deletion operations.
 *
 * @tparam Type The type of elements stored in the binary search tree.
 */
template<typename Type>
class BinarySearchTree : public BinaryTree<Type> {

private:
    using BinaryTree<Type>::root;


    /**
     * Recursively inserts an element into a binary search tree.
     *
     * This method traverses the tree starting from the given node,
     * comparing the value of the element to decide the position
     * for insertion. If the subtree rooted at the current node is empty,
     * a new node is created to hold the element. If the element is smaller
     * than the current node's data, the method continues traversal to the
     * left subtree; otherwise, it traverses to the right subtree.
     *
     * @param node A reference to a pointer to the current node of the subtree being traversed.
     * @param element The element to be inserted into the tree.
     */
    void recursiveInsert(Node<Type>*& node, const Type& element) {
        if (node == nullptr) {
            node = new Node(element);
            return;
        }

        if (element < node->data)
            recursiveInsert(node->left, element);
        else if (element > node->data)
            recursiveInsert(node->right, element);
    }


    /**
     * Recursively removes an element from a binary search tree.
     *
     * This method traverses the tree to locate the node containing
     * the specified element. If the element is found, it is removed
     * according to standard binary search tree removal logic:
     * - If the node has no children, it is simply deleted.
     * - If the node has one child, the child replaces the node.
     * - If the node has two children, the node is replaced with
     *   the smallest element from its right subtree, and the removal
     *   is recursively applied to that subtree.
     *
     * @param node A pointer to the current node being examined or modified.
     * @param element The element to be removed from the tree.
     */
    void recursiveRemove(Node<Type>*& node, const Type& element) {
        if (node == nullptr)
            return;

        if (element < node->data) {
            recursiveRemove(node->left, element);
        } else if (element > node->data) {
            recursiveRemove(node->right, element);
        } else {
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                node = nullptr;
            } else if (node->left == nullptr) {
                Node<Type>* temp = node;
                node = node->right;
                delete temp;
            } else if (node->right == nullptr) {
                Node<Type>* temp = node;
                node = node->left;
                delete temp;
            } else {
                Node<Type>* temp = findMin(node->right);
                node->data = temp->data;
                recursiveRemove(node->right, temp->data);
            }
        }
    }


    /**
     * Finds the node with the minimum value in a subtree.
     *
     * This method traverses the left children of a given node
     * to locate the leftmost node, which contains the smallest value
     * in the subtree. If the provided node is nullptr, the method
     * returns nullptr.
     *
     * @param node A pointer to the root of the subtree in which to find the minimum node.
     * @return A pointer to the node containing the minimum value in the subtree,
     *         or nullptr if the subtree is empty.
     */
    Node<Type>* findMin(Node<Type>* node) const {
        if (node == nullptr)
            return nullptr;

        while (node->left != nullptr)
            node = node->left;
        return node;
    }



public:
    BinarySearchTree() = default;

    BinarySearchTree(const BinarySearchTree& other) = default;

    BinarySearchTree(BinarySearchTree&& other) noexcept = default;

    BinarySearchTree& operator=(const BinarySearchTree& other) = default;

    BinarySearchTree& operator=(BinarySearchTree&& other) noexcept = default;


    void insert(const Type& element) {
        recursiveInsert(root, element);
    }

    void insertLeft(const Type& element) = delete;
    void insertRight(const Type& element) = delete;

    void remove(const Type& element) {
        recursiveRemove(root, element);
    }

    ~BinarySearchTree() = default;

};


#endif //BINARYSEARCHTREE_HPP
