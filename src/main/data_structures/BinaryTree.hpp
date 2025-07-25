

#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP


#include <iostream>


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
    unsigned int size_;


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
    Node<Type>* copyNode(Node<Type>* otherNode, Node<Type>* parent = nullptr) {
        if (otherNode == nullptr)
            return nullptr;

        Node<Type>* newNode = new Node(otherNode->data);
        newNode->parent = parent;

        newNode->left = copyNode(otherNode->left, newNode);
        newNode->right = copyNode(otherNode->right, newNode);
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
        root_ = copyNode(other.root_);
    }


    /**
     * Recursively calculates the height of the binary tree.
     *
     * @param node Pointer to the current node being visited in the recursive
     * process.
     * @return The height of the binary tree.
     */
    unsigned recursiveHeight(Node<Type>* node) const {
        if (node == nullptr)
            return 0;

        unsigned leftHeight = recursiveHeight(node->left);
        unsigned rightHeight = recursiveHeight(node->right);
        return 1 + std::max(leftHeight, rightHeight);
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


  public:
    /// Default constructor
    BinaryTree() : root_(nullptr), size_(0) {}

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


    bool isEmpty() const { return root_ == nullptr; }

    unsigned int size() const { return size_; }

    const Node<Type>* getRoot() const { return root_; }

    unsigned getHeight() const { return recursiveHeight(root_); }


    /**
     * Inserts an element as the rightmost node of the binary tree.
     *
     * @param element The element to be inserted into the rightmost position.
     */
    void insertRight(const Type& element) {
        if (this->isEmpty()) {
            root_ = new Node(element);
            size_++;
            return;
        }

        Node<Type>* current = root_;
        while (current->right != nullptr)
            current = current->right;
        current->right = new Node(element);

        Node<Type>* newNode = new Node(element);
        newNode->parent = current;
        current->right = newNode;
        size_++;
    }


    /**
     * Inserts an element as the leftmost child of the binary tree.
     *
     * @param element The element to be inserted at the leftmost position of the
     * binary tree.
     */
    void insertLeft(const Type& element) {
        if (this->isEmpty()) {
            root_ = new Node(element);
            size_++;
            return;
        }

        Node<Type>* current = root_;
        while (current->left != nullptr)
            current = current->left;

        Node<Type>* newNode = new Node(element);
        newNode->parent = current;
        current->left = newNode;
        size_++;
    }


    /**
     * Checks if the binary tree contains a node with a specific value.
     *
     * @param value The value to search for in the binary tree.
     * @return True if the value is found in the binary tree, false otherwise.
     */
    bool containsNode(const Type& value) const {
        return recursiveContainsNode(root_, value);
    }


    /// Prints the elements of the binary tree in in-order traversal.
    void printInOrder() const { recursiveInOrder(root_); }


    /// Prints the elements of the binary tree in pre-order traversal.
    void printPreOrder() const { recursivePreOrder(root_); }


    /// Prints the elements of the binary tree in post-order traversal.
    void printPostOrder() const { recursivePostOrder(root_); }


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
