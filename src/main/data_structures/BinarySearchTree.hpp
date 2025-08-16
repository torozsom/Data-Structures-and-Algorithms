#ifndef BINARYSEARCHTREE_HPP
#define BINARYSEARCHTREE_HPP


#include "BinaryTree.hpp"


/**
 * @class BinarySearchTree
 *
 * A binary search tree data structure that stores elements of a given type.
 *
 * The binary search tree is a binary tree data structure in which each node has
 * at most two children, referred to as the left child and the right child. The
 * tree is defined recursively, with each child node being the root of its own
 * subtree.
 *
 * The binary search tree is a special type of binary tree in which the elements
 * are ordered in a specific way. For each node, all elements in the left
 * subtree are less than the node's element, and all elements in the right
 * subtree are greater than the node's element. This ordering property allows
 * for efficient search, insertion, and deletion operations.
 *
 * @tparam Type The type of elements stored in the binary search tree.
 */
template <typename Type>
class BinarySearchTree : public BinaryTree<Type> {

  private:
    /**
     * Recursively inserts an element into a binary search tree.
     *
     * This method traverses the tree to find the appropriate position for the
     * new element. If the tree is empty, the element becomes the root. If the
     * element is less than the current node's data, it is inserted into the
     * left subtree; if greater, into the right subtree.
     *
     * @complexity Average and worst case insertion is O(n).
     *
     * @tparam U The type of the element to be inserted. Must be constructible
     * into Type.
     *
     * @param node A reference to a pointer to the current node being examined
     * or modified.
     * @param element The element to be inserted into the tree.
     * @param parent A pointer to the parent node of the current node being
     * examined (optional).
     */
    template <typename U>
    void recursiveInsert(Node<Type>*& node, U&& element,
                         Node<Type>* parent = nullptr) {
        static_assert(std::is_constructible_v<Type, U&&>,
                      "Element must be constructible into Type");

        if (node == nullptr) {
            node = new Node<Type>(std::forward<U>(element));
            node->parent = parent;
            ++this->size_;
            return;
        }

        if (element < node->data)
            recursiveInsert(node->left, std::forward<U>(element), node);
        else if (element > node->data)
            recursiveInsert(node->right, std::forward<U>(element), node);
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
     * @complexity Average and worst case removal is O(n).
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
                --this->size_;
                delete node;
                node = nullptr;
            } else if (node->left == nullptr) {
                --this->size_;
                Node<Type>* temp = node;
                node = node->right;
                if (node)
                    node->parent = temp->parent;
                delete temp;
            } else if (node->right == nullptr) {
                --this->size_;
                Node<Type>* temp = node;
                node = node->left;
                if (node)
                    node->parent = temp->parent;
                delete temp;
            } else {
                Node<Type>* temp = findMinNode(node->right);
                node->data = std::move(temp->data);
                recursiveRemove(node->right, node->data);
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
     * @complexity O(h), where h is the height of the subtree.
     *
     * @param node A pointer to the root of the subtree in which to find the
     * minimum node.
     * @return A pointer to the node containing the minimum value in the
     * subtree, or nullptr if the subtree is empty.
     */
    Node<Type>* findMinNode(Node<Type>* node) const {
        if (node == nullptr)
            return nullptr;

        while (node->left != nullptr)
            node = node->left;
        return node;
    }


    /**
     * Finds the node with the maximum value in a subtree.
     *
     * This method traverses the right children of a given node
     * to locate the rightmost node, which contains the largest value
     * in the subtree. If the provided node is nullptr, the method
     * returns nullptr.
     *
     * @complexity O(h), where h is the height of the subtree.
     *
     * @param node A pointer to the root of the subtree in which to find the
     * maximum node.
     * @return A pointer to the node containing the maximum value in the
     * subtree, or nullptr if the subtree is empty.
     */
    const Node<Type>* findMaxNode(Node<Type>* node) const {
        if (node == nullptr)
            return nullptr;

        while (node->right != nullptr)
            node = node->right;
        return node;
    }


    /// Checks if the binary tree is a valid BST.
    bool isValidBSTHelper(Node<Type>* node, const Type* minVal = nullptr,
                          const Type* maxVal = nullptr) const {
        if (node == nullptr)
            return true;

        if ((minVal != nullptr && node->data <= *minVal) ||
            (maxVal != nullptr && node->data >= *maxVal))
            return false;

        return isValidBSTHelper(node->left, minVal, &node->data) &&
               isValidBSTHelper(node->right, &node->data, maxVal);
    }


  public:
    /// Default constructor
    BinarySearchTree() : BinaryTree<Type>() {}

    /**
     * Constructor that initializes the binary search tree with an array of
     * elements.
     *
     * This constructor takes an array of elements and inserts each element
     * into the binary search tree, maintaining the properties of a binary
     * search tree.
     *
     * @param array Pointer to the array of elements to be inserted into the
     * binary search tree.
     * @param size The number of elements in the array.
     */
    BinarySearchTree(const Type* array, const std::size_t size)
        : BinaryTree<Type>() {
        for (std::size_t i = 0; i < size; ++i)
            this->insert(array[i]);
    }

    /// Copy constructor
    BinarySearchTree(const BinarySearchTree& other) : BinaryTree<Type>(other) {}

    /// Move constructor
    BinarySearchTree(BinarySearchTree&& other) noexcept
        : BinaryTree<Type>(std::move(other)) {}

    /// Copy assignment operator
    BinarySearchTree& operator=(const BinarySearchTree& other) {
        BinaryTree<Type>::operator=(other);
        return *this;
    }

    /// Move assignment operator
    BinarySearchTree& operator=(BinarySearchTree&& other) noexcept {
        BinaryTree<Type>::operator=(std::move(other));
        return *this;
    }


    /// Checks if the binary tree is a valid BST.
    [[nodiscard]]
    bool isValidBST() const noexcept {
        return isValidBSTHelper(this->root_);
    }


    /**
     * Inserts an element into the binary search tree.
     *
     * This method inserts a new element into the binary search tree by
     * recursively finding the appropriate position for the new element.
     * If the tree is empty, the new element becomes the root. If the
     * element is less than the current node's data, it is inserted into
     * the left subtree; if greater, into the right subtree.
     *
     * @complexity Average and worst case insertion is O(n).
     *
     * @tparam U The type of the element to be inserted. Must be constructible
     * into Type.
     *
     * @param element The element to be inserted into the tree.
     */
    template <typename U>
    void insert(U&& element) {
        recursiveInsert(this->root_, std::forward<U>(element));
    }


    void insertLeft(const Type& element) = delete;
    void insertRight(const Type& element) = delete;


    /**
     * Removes an element from the binary search tree.
     *
     * This method removes a specified element from the binary search tree
     * by recursively traversing the tree to locate the node containing the
     * element. If the element is found, it is removed according to standard
     * binary search tree removal logic. If the element is not found, the tree
     * remains unchanged.
     *
     * @param element The element to be removed from the tree.
     */
    void remove(const Type& element) { recursiveRemove(this->root_, element); }


    /**
     * Checks if the binary search tree contains a specified element.
     *
     * This method traverses the binary search tree to determine if
     * a given element is present in the tree. It starts from the root
     * and compares the element with the data of each node, moving left
     * or right as appropriate until it either finds the element or reaches
     * a leaf node.
     *
     * @param element The element to check for presence in the tree.
     * @return True if the element is found, false otherwise.
     */
    bool contains(const Type& element) const noexcept {
        Node<Type>* current = this->root_;
        while (current != nullptr) {
            if (element < current->data) {
                current = current->left;
            } else if (element > current->data) {
                current = current->right;
            } else {
                return true;
            }
        }
        return false;
    }


    /**
     * Finds the node with the minimum value in the binary search tree.
     *
     * This method traverses the left children of the root node to find
     * the leftmost node, which contains the smallest value in the tree.
     * If the tree is empty, it throws an exception.
     *
     * @return The minimum value in the binary search tree.
     * @throws std::runtime_error If the tree is empty.
     */
    Type findMinimum() const {
        if (this->isEmpty())
            throw std::runtime_error("Tree is empty");
        return findMinNode(this->root_)->data;
    }


    /**
     * Finds the node with the maximum value in the binary search tree.
     *
     * This method traverses the right children of the root node to find
     * the rightmost node, which contains the largest value in the tree.
     * If the tree is empty, it throws an exception.
     *
     * @return The maximum value in the binary search tree.
     * @throws std::runtime_error If the tree is empty.
     */
    Type findMaximum() const {
        if (this->isEmpty())
            throw std::runtime_error("Tree is empty");
        return findMaxNode(this->root_)->data;
    }


    ~BinarySearchTree() override = default;
};


#endif // BINARYSEARCHTREE_HPP
