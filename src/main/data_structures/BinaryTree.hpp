#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP


#include <iostream>
#include "Queue.hpp"


/**
 * Represents a node in a binary tree structure.
 *
 * @tparam Type The data type stored in the node.
 */
template <typename Type>
struct Node {
    Type data;
    Node* parent;
    Node* left;
    Node* right;
    explicit Node(const Type& data)
        : data(data), parent(nullptr), left(nullptr), right(nullptr) {}
};


/**
 * @class BinaryTree
 * A binary tree data structure that stores elements of a given type.
 *
 * The binary tree is a tree data structure in which each node has at most two
 * children, referred to as the left child and the right child. The tree is
 * defined recursively, with each child node being the root of its own subtree.
 *
 * @tparam Type The type of elements stored in the binary tree.
 */
template <typename Type>
class BinaryTree {

  protected:
    Node<Type>* root_;
    std::size_t size_;


    /**
     * Creates a new copy of a binary tree node and all its child nodes
     * recursively.
     *
     * @param otherNode A pointer to the node to be copied. If nullptr, returns
     * nullptr.
     * @param parent A pointer to the parent node of the current node being
     * copied.
     * @return A pointer to the newly created node, which is a copy of the input
     * node along with its subtree.
     */
    Node<Type>* recursiveCopyNode(Node<Type>* otherNode,
                                  Node<Type>* parent = nullptr) {
        if (otherNode == nullptr)
            return nullptr;

        Node<Type>* newNode = new Node(otherNode->data);
        newNode->parent = parent;

        newNode->left = recursiveCopyNode(otherNode->left, newNode);
        newNode->right = recursiveCopyNode(otherNode->right, newNode);
        return newNode;
    }


    /**
     * Recursively copies all elements from another binary tree into the current
     * tree.
     *
     * @param other The binary tree whose elements are to be copied into the
     * current tree.
     */
    void recursiveCopy(const BinaryTree& other) {
        root_ = recursiveCopyNode(other.root_);
    }


    /**
     * Recursively calculates the height of the binary tree.
     *
     * @param node Pointer to the current node being visited in the recursive
     * process.
     * @return The height of the binary tree.
     */
    std::size_t recursiveHeight(Node<Type>* node) const {
        if (node == nullptr)
            return 0;

        const std::size_t leftHeight = recursiveHeight(node->left);
        const std::size_t rightHeight = recursiveHeight(node->right);
        return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
    }


    /**
     * Performs an in-order traversal of the binary tree recursively and outputs
     * the data of each node.
     *
     * @param node Pointer to the current node being visited during the in-order
     * traversal.
     */
    void recursiveInOrder(Node<Type>* node) const {
        if (node == nullptr)
            return;

        recursiveInOrder(node->left);
        std::cout << node->data << " ";
        recursiveInOrder(node->right);
    }


    /**
     * Performs a pre-order traversal of the binary tree recursively, visiting
     * nodes in the order: root, left subtree, right subtree.
     *
     * @param node The current node in the binary tree being visited during the
     * traversal.
     */
    void recursivePreOrder(Node<Type>* node) const {
        if (node == nullptr)
            return;

        std::cout << node->data << " ";
        recursivePreOrder(node->left);
        recursivePreOrder(node->right);
    }


    /**
     * Performs a recursive post-order traversal of the binary tree and
     * processes each node. The traversal visits the left subtree, then the
     * right subtree, and finally the current node.
     *
     * @param node Pointer to the current node being visited in the traversal
     * process.
     */
    void recursivePostOrder(Node<Type>* node) const {
        if (node == nullptr)
            return;

        recursivePostOrder(node->left);
        recursivePostOrder(node->right);
        std::cout << node->data << " ";
    }


    /**
     * Recursively clears the binary tree by deallocating memory occupied by all
     * nodes.
     *
     * @param node Pointer to the current node in the binary tree that is being
     * cleared.
     */
    void recursiveClear(Node<Type>* node) {
        if (node == nullptr)
            return;

        recursiveClear(node->left);
        recursiveClear(node->right);
        delete node;
    }


    /**
     * Recursively checks if a binary tree contains a node with a specific
     * value.
     *
     * @param node Pointer to the current node being visited in the recursive
     * process.
     * @param value The value to search for in the binary tree.
     * @return True if the value is found in the binary tree, false otherwise.
     */
    bool recursiveContainsNode(const Node<Type>* node,
                               const Type& value) const {
        if (node == nullptr)
            return false;

        if (node->data == value)
            return true;

        return recursiveContainsNode(node->left, value) ||
               recursiveContainsNode(node->right, value);
    }


    /**
     * Recursively searches for a node with a specific value in the binary tree.
     *
     * @param node Pointer to the current node being visited in the recursive
     * process.
     * @param value The value to search for in the binary tree.
     * @return A pointer to the node containing the value, or nullptr if not
     * found.
     */
    Node<Type>* recursiveFindNode(Node<Type>* node, const Type& value) const {
        if (node == nullptr)
            return nullptr;

        if (node->data == value)
            return node;

        Node<Type>* leftResult = recursiveFindNode(node->left, value);
        if (leftResult != nullptr)
            return leftResult;

        return recursiveFindNode(node->right, value);
    }


    /**
     * Performs a level-order traversal of the binary tree using a queue to
     * visit nodes level by level, from left to right.
     *
     * @param node Pointer to the root node of the binary tree.
     */
    void levelOrder() const {
        if (isEmpty()) {
            std::cout << "Tree is empty" << std::endl;
            return;
        }

        Queue<Node<Type>*> queue;
        queue.enqueue(root_);

        std::cout << "Level-order: ";
        while (!queue.isEmpty()) {
            Node<Type>* current = queue.dequeue();
            std::cout << current->data << " ";

            if (current->left != nullptr)
                queue.enqueue(current->left);
            if (current->right != nullptr)
                queue.enqueue(current->right);
        }

        std::cout << std::endl;
    }


  public:
    /// Default constructor
    BinaryTree() noexcept : root_(nullptr), size_(0) {}

    /**
     * Constructor that creates a binary tree from an array using level-order
     * insertion.
     * @param array Pointer to the array of elements.
     * @param size Number of elements in the array.
     */
    BinaryTree(const Type* array, const std::size_t size)
        : root_(nullptr), size_(0) {
        for (std::size_t i = 0; i < size; i++)
            BinaryTree::insert(array[i]);
    }

    /// Copy constructor
    BinaryTree(const BinaryTree& other) : root_(nullptr), size_(other.size_) {
        recursiveCopy(other);
    }

    /// Move constructor
    BinaryTree(BinaryTree&& other) noexcept
        : root_(other.root_), size_(other.size_) {
        other.root_ = nullptr;
        other.size_ = 0;
    }


    /// Assignment operator
    BinaryTree& operator=(const BinaryTree& other) {
        if (this == &other)
            return *this;

        clear();
        recursiveCopy(other);
        size_ = other.size_;
        return *this;
    }


    /// Move assignment operator
    BinaryTree& operator=(BinaryTree&& other) noexcept {
        if (this == &other)
            return *this;

        clear();
        root_ = other.root_;
        size_ = other.size_;
        other.root_ = nullptr;
        other.size_ = 0;
        return *this;
    }


    bool isEmpty() const noexcept { return root_ == nullptr; }

    std::size_t size() const noexcept { return size_; }

    const Node<Type>* getRoot() const noexcept { return root_; }

    std::size_t getHeight() const noexcept { return recursiveHeight(root_); }


    /// Checks if the binary tree is a complete binary tree.
    bool isCompleteTree() const {
        if (this->isEmpty())
            return true;

        Queue<Node<Type>*> queue;
        queue.enqueue(this->root_);
        bool foundNull = false;

        while (!queue.isEmpty()) {
            Node<Type>* current = queue.dequeue();

            if (current == nullptr) {
                foundNull = true;
            } else {
                if (foundNull) // Found non-null after null
                    return false;
                queue.enqueue(current->left);
                queue.enqueue(current->right);
            }
        }
        return true;
    }


    /**
     * Inserts an element as the rightmost node of the binary tree.
     *
     * @param element The element to be inserted into the rightmost position.
     */
    void insertRight(const Type& element) {
        if (this->isEmpty()) {
            root_ = new Node<Type>(element);
            size_++;
            return;
        }

        Node<Type>* current = root_;
        while (current->right != nullptr)
            current = current->right;

        Node<Type>* newNode = new Node<Type>(element);
        newNode->parent = current;
        current->right = newNode;
        size_++;
    }


    /**
     * Inserts an element as the leftmost child of the binary tree.
     *
     * @param element The element to be inserted at the leftmost position.
     */
    void insertLeft(const Type& element) {
        if (this->isEmpty()) {
            root_ = new Node<Type>(element);
            size_++;
            return;
        }

        Node<Type>* current = root_;
        while (current->left != nullptr)
            current = current->left;

        Node<Type>* newNode = new Node<Type>(element);
        newNode->parent = current;
        current->left = newNode;
        size_++;
    }


    /**
     * Inserts an element into the binary tree using level-order insertion.
     * The insertion maintains the complete binary tree property. If the tree is
     * empty, the element becomes the root node. Otherwise, the element is
     * inserted as the first available position when traversing level by level
     * from left to right.
     *
     * @param element The element to be inserted into the binary tree.
     */
    virtual void insert(const Type& element) {
        if (this->isEmpty()) {
            root_ = new Node<Type>(element);
            size_++;
            return;
        }

        Queue<Node<Type>*> queue;
        queue.enqueue(root_);

        while (!queue.isEmpty()) {
            Node<Type>* current = queue.dequeue();

            // Check if left child is available
            if (current->left == nullptr) {
                Node<Type>* newNode = new Node<Type>(element);
                newNode->parent = current;
                current->left = newNode;
                size_++;
                return;
            }

            // Check if right child is available
            if (current->right == nullptr) {
                Node<Type>* newNode = new Node<Type>(element);
                newNode->parent = current;
                current->right = newNode;
                size_++;
                return;
            }

            // Both children exist, add them to queue for next level
            queue.enqueue(current->left);
            queue.enqueue(current->right);
        }
    }


    /**
     * Checks if the binary tree contains a node with a specific value.
     *
     * @param value The value to search for in the binary tree.
     * @return True if the value is found in the binary tree, false otherwise.
     */
    bool containsNode(const Type& value) const noexcept {
        return recursiveContainsNode(root_, value);
    }


    /**
     * Finds a node with a specific value in the binary tree.
     *
     * @param value The value to search for in the binary tree.
     * @return A pointer to the node containing the value, or nullptr if not
     * found.
     */
    Node<Type>* findNode(const Type& value) const {
        return recursiveFindNode(root_, value);
    }


    /**
     * Finds a node with a specific value using level-order traversal.
     *
     * @param value The value to search for in the binary tree.
     * @return A pointer to the node containing the value, or nullptr if not
     * found.
     */
    Node<Type>* findNodeLevelOrder(const Type& value) const {
        if (isEmpty())
            return nullptr;

        Queue<Node<Type>*> queue;
        queue.enqueue(root_);

        while (!queue.isEmpty()) {
            Node<Type>* current = queue.dequeue();

            if (current->data == value)
                return current;

            if (current->left != nullptr)
                queue.enqueue(current->left);

            if (current->right != nullptr)
                queue.enqueue(current->right);
        }

        return nullptr;
    }


    /// Prints the elements of the binary tree in in-order traversal.
    void printInOrder() const { recursiveInOrder(root_); }

    /// Prints the elements of the binary tree in pre-order traversal.
    void printPreOrder() const { recursivePreOrder(root_); }

    /// Prints the elements of the binary tree in post-order traversal.
    void printPostOrder() const { recursivePostOrder(root_); }

    /// Prints the elements of the binary tree in level-order traversal.
    void printLevelOrder() const { levelOrder(); }


    /// Prints the structure and contents of the binary tree.
    void print() const {
        if (isEmpty()) {
            std::cout << "Binary Tree is empty" << std::endl;
            return;
        }

        std::cout << "\n=== Binary Tree Structure ===" << std::endl;
        std::cout << "Size: " << size_ << ", Height: " << getHeight()
                  << std::endl;

        std::cout << "Level-order: ";
        printLevelOrder();

        std::cout << "In-order:    ";
        printInOrder();
        std::cout << std::endl;

        std::cout << "Pre-order:   ";
        printPreOrder();
        std::cout << std::endl;

        std::cout << "Post-order:  ";
        printPostOrder();
        std::cout << std::endl;
    }


    /**
     * Clears the binary tree by deallocating all nodes and resetting the tree
     * to an empty state.
     *
     * This method ensures that all allocated memory for the tree nodes is
     * released, leaving the root pointer as nullptr.
     */
    void clear() {
        recursiveClear(root_);
        root_ = nullptr;
        size_ = 0;
    }


    /// Destructor
    virtual ~BinaryTree() { clear(); }
};

#endif // BINARYTREE_HPP
