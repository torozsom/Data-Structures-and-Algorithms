#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP


#include <cassert>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <memory>

#include "Queue.hpp"


namespace containers {

using std::size_t;


/**
 * @struct Node
 * @brief Represents a node in a binary tree.
 *
 * Each node contains data of type `Type`, pointers to its parent, left child,
 * and right child. The `Node` class is templated to allow for any data type.
 * The constructor allows for constructing a node with any type `U` that is
 * constructible from `Type`, while preventing direct instantiation of `Node`
 * with `Node` itself.
 *
 * @tparam Type Element type stored in the node.
 */
template <typename Type>
struct Node {
    Type data;
    Node* parent = nullptr;
    Node* left = nullptr;
    Node* right = nullptr;

    template <typename U>
        requires(std::is_constructible_v<Type, U &&> &&
                 !std::is_same_v<std::remove_cvref_t<U>, Node>)
    explicit Node(U&& data)
        : data(std::forward<U>(data)), parent(nullptr), left(nullptr), right(nullptr) {}

    ~Node() {
        delete left;
        delete right;
    }

    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;

    Node(Node&&) = delete;
    Node& operator=(Node&&) = delete;
};


/**
 * @class BinaryTree
 * @brief A generic binary tree storing elements of type `Type`.
 *
 * @tparam Type Element type stored by the tree.
 */
template <typename Type>
class BinaryTree {

  protected:
    Node<Type>* root_ = nullptr;
    size_t size_ = 0;


    /**
     * Recursively copies a subtree rooted at `otherNode`, creating new nodes
     * with the same data and structure. The `parent` parameter is used to set
     * the parent pointer of the newly created node.
     *
     * @param otherNode Pointer to the root of the subtree to copy.
     * @param parent Pointer to the parent node for the new subtree (default nullptr).
     * @return Pointer to the root of the newly copied subtree.
     */
    Node<Type>* recursiveCopyNode(const Node<Type>* otherNode,
                                  Node<Type>* parent = nullptr) {
        if (otherNode == nullptr)
            return nullptr;

        std::unique_ptr<Node<Type>> newNode(new Node<Type>(otherNode->data));
        newNode->parent = parent;

        newNode->left = recursiveCopyNode(otherNode->left, newNode.get());
        newNode->right = recursiveCopyNode(otherNode->right, newNode.get());

        return newNode.release();
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
     * @param node Pointer to the current node being visited.
     * @return Height using the convention: empty subtree => 0; leaf => 1.
     */
    size_t recursiveHeight(Node<Type>* node) const {
        if (node == nullptr)
            return 0;

        const size_t leftHeight = recursiveHeight(node->left);
        const size_t rightHeight = recursiveHeight(node->right);
        return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
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
    const Node<Type>* recursiveFindNode(const Node<Type>* node,
                                        const Type& value) const {
        if (node == nullptr)
            return nullptr;

        if (node->data == value)
            return node;

        if (const Node<Type>* leftResult = recursiveFindNode(node->left, value);
            leftResult != nullptr)
            return leftResult;

        return recursiveFindNode(node->right, value);
    }


    /**
     * Performs a level-order traversal and returns node values in BFS order.
     *
     * @return A `Queue<Type>` containing a snapshot of the tree's values from
     *         the current root, level by level, left-to-right.
     *
     * @note The returned queue is a copy of values, not a live view.
     */
    Queue<Type> levelOrder() const {
        if (isEmpty())
            return Queue<Type>{};

        Queue<Node<Type>*> queue;
        queue.enqueue(root_);
        Queue<Type> result;

        while (!queue.isEmpty()) {
            Node<Type>* current = queue.front();
            queue.dequeue();
            result.enqueue(current->data);

            if (current->left != nullptr)
                queue.enqueue(current->left);
            if (current->right != nullptr)
                queue.enqueue(current->right);
        }
        return result;
    }


  public:
    /// Default constructor
    BinaryTree() noexcept : root_() {}

    /// Constructor for braced-init-lists
    BinaryTree(std::initializer_list<Type> initial_data) : root_() {
        for (const auto& element : initial_data)
            this->insert(element);
    }

    /**
     * Constructor that creates a binary tree from an array using level-order
     * insertion.
     * @param array Pointer to the array of elements.
     * @param size Number of elements in the array.
     */
    BinaryTree(const Type* array, const size_t size) : root_() {
        if (array == nullptr && size != 0)
            throw std::invalid_argument(
                "Array pointer cannot be null when size is non-zero");
        for (size_t i = 0; i < size; i++)
            this->insert(array[i]);
    }

    /// Copy constructor
    BinaryTree(const BinaryTree& other) : root_(), size_(other.size_) {
        recursiveCopy(other);
    }

    /// Move constructor
    BinaryTree(BinaryTree&& other) noexcept
        : root_(other.root_), size_(other.size_) {
        other.root_ = nullptr;
        other.size_ = 0;
    }


    /// Copy assignment operator
    BinaryTree& operator=(const BinaryTree& other) {
        if (this == &other)
            return *this;

        Node<Type>* new_root = recursiveCopyNode(other.root_);
        clear();
        root_ = new_root;
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


    /// Checks if the binary tree is empty.
    [[nodiscard]]
    bool isEmpty() const noexcept {
        assert(((size_ == 0) && (root_ == nullptr)) ||
               ((size_ > 0) && (root_ != nullptr)));
        return size_ == 0;
    }

    /// Returns the number of elements in the binary tree.
    [[nodiscard]]
    size_t size() const noexcept {
        return size_;
    }

    /// Returns the root node of the binary tree.
    [[nodiscard]]
    const Node<Type>* getRoot() const noexcept {
        return root_;
    }

    /// Returns the height of the binary tree.
    [[nodiscard]]
    size_t getHeight() const noexcept {
        return recursiveHeight(root_);
    }


    /**
     * Checks if the binary tree is a complete binary tree.
     *
     * A tree is complete if all levels are completely filled except possibly
     * the last, and all nodes at the last level are as far left as possible.
     *
     * @return true if the tree is complete, false otherwise.
     */
    [[nodiscard]]
    bool isCompleteTree() const {
        if (this->isEmpty())
            return true;

        Queue<Node<Type>*> queue;
        queue.enqueue(this->root_);
        bool foundMissingChild = false;

        while (!queue.isEmpty()) {
            Node<Type>* current = queue.front();
            queue.dequeue();

            if (current->left != nullptr) {
                if (foundMissingChild) return false;
                queue.enqueue(current->left);
            } else {
                foundMissingChild = true;
            }

            if (current->right != nullptr) {
                if (foundMissingChild) return false;
                queue.enqueue(current->right);
            } else {
                foundMissingChild = true;
            }
        }
        return true;
    }


    /**
     * @brief Insert as the right child of the rightmost chain.
     *
     * Descends via `right` pointers until it reaches the rightmost node that
     * has no right child, then attaches the new node there. If the tree is
     * empty, the new node becomes root.
     *
     * @tparam U  A type that can construct `Type` (perfect-forwarded).
     * @param element  The value to insert.
     */
    template <typename U>
    void insertRight(U&& element) {
        static_assert(std::is_constructible_v<Type, U&&>,
                      "Only types constructible into Type are allowed");

        if (this->isEmpty()) {
            root_ = new Node<Type>(std::forward<U>(element));
            size_++;
            return;
        }

        Node<Type>* current = root_;
        while (current->right != nullptr)
            current = current->right;

        auto* newNode = new Node<Type>(std::forward<U>(element));
        newNode->parent = current;
        current->right = newNode;
        size_++;
    }


    /**
     * @brief Insert as the left child of the leftmost chain.
     *
     * Descends via `left` pointers until it reaches the leftmost node that has
     * no left child, then attaches the new node there. If the tree is empty,
     * the new node becomes root.
     *
     * @tparam U  A type that can construct `Type` (perfect-forwarded).
     * @param element  The value to insert.
     */
    template <typename U>
    void insertLeft(U&& element) {
        static_assert(std::is_constructible_v<Type, U&&>,
                      "Only types constructible into Type are allowed");

        if (this->isEmpty()) {
            root_ = new Node<Type>(std::forward<U>(element));
            size_++;
            return;
        }

        Node<Type>* current = root_;
        while (current->left != nullptr)
            current = current->left;

        auto* newNode = new Node<Type>(std::forward<U>(element));
        newNode->parent = current;
        current->left = newNode;
        size_++;
    }


    /**
     * @brief Insert at the first available position in level-order (BFS).
     *
     * The tree is traversed breadth-first; the new node becomes the left child
     * of the first node missing a left child, otherwise the right child of the
     * first node missing a right child. If the tree is empty, the new node
     * becomes root.
     *
     * @tparam U  A type that can construct `Type` (perfect-forwarded).
     * @param element  The value to insert.
     */
    template <typename U>
    void insert(U&& element) {
        static_assert(std::is_constructible_v<Type, U&&>,
                      "Element must be constructible into Type");

        if (this->isEmpty()) {
            root_ = new Node<Type>(std::forward<U>(element));
            size_++;
            return;
        }

        Queue<Node<Type>*> queue;
        queue.enqueue(root_);

        while (!queue.isEmpty()) {
            Node<Type>* current = queue.front();
            queue.dequeue();

            // Check if left child is available
            if (current->left == nullptr) {
                auto* newNode = new Node<Type>(std::forward<U>(element));
                newNode->parent = current;
                current->left = newNode;
                size_++;
                return;
            }

            // Check if right child is available
            if (current->right == nullptr) {
                auto* newNode = new Node<Type>(std::forward<U>(element));
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
    [[nodiscard]]
    virtual bool contains(const Type& value) const {
        return recursiveContainsNode(root_, value);
    }


    /**
     * Finds a node with a specific value using depth-first search (preorder).
     *
     * @param value The value to search for in the binary tree.
     * @return A const pointer to the first matching node found, or nullptr if
     * none.
     */
    [[nodiscard]]
    const Node<Type>* findNode(const Type& value) const {
        return recursiveFindNode(root_, value);
    }


    /**
     * Finds a node with a specific value in the binary tree.
     *
     * @param value The value to search for in the binary tree.
     * @return A pointer to the node containing the value, or nullptr if not
     * found.
     */
    [[nodiscard]]
    Node<Type>* findNode(const Type& value) {
        return const_cast<Node<Type>*>(
            static_cast<const BinaryTree*>(this)->findNode(value));
    }


    /**
     * Finds a node with a specific value using level-order traversal.
     *
     * @param value The value to search for in the binary tree.
     * @return A const pointer to the node containing the value, or nullptr if
     * not found.
     */
    [[nodiscard]]
    const Node<Type>* findNodeLevelOrder(const Type& value) const {
        if (isEmpty())
            return nullptr;

        Queue<Node<Type>*> queue;
        queue.enqueue(root_);

        while (!queue.isEmpty()) {
            Node<Type>* current = queue.front();
            queue.dequeue();

            if (current->data == value)
                return current;

            if (current->left != nullptr)
                queue.enqueue(current->left);

            if (current->right != nullptr)
                queue.enqueue(current->right);
        }

        return nullptr;
    }


    /**
     * Finds a node with a specific value using level-order traversal.
     *
     * @param value The value to search for in the binary tree.
     * @return A pointer to the node containing the value, or nullptr if not
     * found.
     */
    [[nodiscard]]
    Node<Type>* findNodeLevelOrder(const Type& value) {
        return const_cast<Node<Type>*>(
            static_cast<const BinaryTree*>(this)->findNodeLevelOrder(value));
    }


    /**
     * Clears the binary tree by deallocating all nodes and resetting to empty.
     *
     * Exception safety: No-throw.
     */
    void clear() noexcept {
        delete root_;
        root_ = nullptr;
        size_ = 0;
    }


    /// Destructor
    virtual ~BinaryTree() noexcept { clear(); }
};

} // namespace containers

#endif // BINARYTREE_HPP
