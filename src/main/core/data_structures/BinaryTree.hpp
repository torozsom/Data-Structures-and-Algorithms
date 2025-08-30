#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP


#include <cassert>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <initializer_list>

#include "Queue.hpp"


namespace containers {


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
        : data(std::forward<U>(data)), parent(), left(), right() {}
};


/**
 * @class BinaryTree
 * @brief A generic binary tree storing elements of type `Type`.
 *
 * Each node has up to two children (`left`, `right`). Insertion via `insert()`
 * follows level-order (breadth-first) rules, which keeps the tree as compact
 * as possible without being a heap or a search tree.
 *
 * @tparam Type Element type stored by the tree.
 *
 * @par Semantics
 * - Equality/ordering between nodes is not defined by the tree; search APIs
 *   (`findNode*`, `containsNode`) rely on `operator==` for `Type`.
 *
 * @par Height definition
 * - `getHeight()` returns 0 for an empty tree; otherwise it is
 *   `1 + max(height(left), height(right))`.
 *
 * @par Complexity (selected)
 * - `insert` (level-order): O(n) in the worst case (queue traversal).
 * - `findNode` (DFS) / `findNodeLevelOrder` (BFS): O(n).
 * - `isCompleteTree`: O(n).
 * - `clear`: O(n).
 *
 * @par Exception safety
 * - Most public operations propagate exceptions from `Type` (copy/move/compare)
 *   and allocations. See `recursiveCopyNode` for copy semantics.
 */
template <typename Type>
class BinaryTree {

  protected:
    Node<Type>* root_ = nullptr;
    std::size_t size_ = 0;


    /**
     * Recursively copies a node and its subtree.
     *
     * @param otherNode Pointer to the subtree root to copy (may be nullptr).
     * @param parent    Parent to set on the newly created node (may be
     * nullptr).
     * @return Pointer to the new subtree root (or nullptr if `otherNode` is
     * nullptr).
     *
     * Complexity: O(k) where k is the number of nodes in the copied subtree.
     *
     * @par Exception safety
     * This implementation propagates exceptions from allocations and `Type`
     * construction. If an exception is thrown mid-copy, already-allocated nodes
     * in the partially built subtree are not reclaimed here (leak risk).
     * For a strong guarantee, switch to an RAII/commit pattern (build the
     * subtree with smart pointers and only release ownership on success).
     */
    Node<Type>* recursiveCopyNode(const Node<Type>* otherNode,
                                  Node<Type>* parent = nullptr) {
        if (otherNode == nullptr)
            return nullptr;

        auto* newNode = new Node<Type>(otherNode->data);
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
     * @param node Pointer to the current node being visited.
     * @return Height using the convention: empty subtree => 0; leaf => 1.
     *
     * Complexity: O(n) over the size of the subtree.
     * Exception safety: No-throw (assuming `node` pointers are valid).
     */
    std::size_t recursiveHeight(Node<Type>* node) const {
        if (node == nullptr)
            return 0;

        const std::size_t leftHeight = recursiveHeight(node->left);
        const std::size_t rightHeight = recursiveHeight(node->right);
        return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
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
     * Complexity: O(n) time, O(n) additional space for the queue and result.
     * Exception safety: Propagates exceptions from `Type` move/copy and
     * allocations.
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
            Node<Type>* current = queue.dequeue();
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
    BinaryTree(const Type* array, const std::size_t size) : root_() {
        if (array == nullptr && size != 0)
            throw std::invalid_argument(
                "Array pointer cannot be null when size is non-zero");
        for (std::size_t i = 0; i < size; i++)
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


    /// Checks if the binary tree is empty.
    [[nodiscard]]
    bool isEmpty() const noexcept {
        assert(((size_ == 0) && (root_ == nullptr)) ||
               ((size_ > 0) && (root_ != nullptr)));
        return size_ == 0;
    }

    /// Returns the number of elements in the binary tree.
    [[nodiscard]]
    std::size_t size() const noexcept {
        return size_;
    }

    /// Returns the root node of the binary tree.
    [[nodiscard]]
    const Node<Type>* getRoot() const noexcept {
        return root_;
    }

    /// Returns the height of the binary tree.
    [[nodiscard]]
    std::size_t getHeight() const noexcept {
        return recursiveHeight(root_);
    }


    /**
     * Checks if the binary tree is a complete binary tree.
     *
     * A tree is complete if all levels are completely filled except possibly
     * the last, and all nodes at the last level are as far left as possible.
     *
     * @return true if the tree is complete, false otherwise.
     *
     * Complexity: O(n) time, O(n) auxiliary space for the queue.
     * Exception safety: Propagates allocation exceptions (from the queue).
     */
    [[nodiscard]]
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
     * @brief Insert as the right child of the rightmost chain.
     *
     * Descends via `right` pointers until it reaches the rightmost node that
     * has no right child, then attaches the new node there. If the tree is
     * empty, the new node becomes root.
     *
     * @tparam U  A type that can construct `Type` (perfect-forwarded).
     * @param element  The value to insert.
     *
     * @par Notes
     * - This operation can skew the tree and does **not** preserve
     * completeness.
     * - Duplicates are allowed; not a BST insert.
     * - Existing node addresses remain stable.
     *
     * @par Complexity
     * - Time: O(h), where h is the tree height.
     * - Space: O(1) auxiliary.
     *
     * @par Exception safety
     * - Strong: if allocation or `Type` construction throws, no changes occur.
     *
     * @par Postconditions
     * - `size()` increased by 1; `parent` of the new node points to its parent.
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
     *
     * @par Notes
     * - This operation can skew the tree and does **not** preserve
     * completeness.
     * - Duplicates are allowed; not a BST insert.
     * - Existing node addresses remain stable.
     *
     * @par Complexity
     * - Time: O(h), where h is the tree height.
     * - Space: O(1) auxiliary.
     *
     * @par Exception safety
     * - Strong: if allocation or `Type` construction throws, no changes occur.
     *
     * @par Postconditions
     * - `size()` increased by 1; `parent` of the new node points to its parent.
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
     *
     * @par Behavior
     * - Preserves a “compact” shape; repeated calls (without removals) keep the
     *   tree complete at the moment of insertion. This is **not** a BST insert.
     * - Duplicates are allowed (equality has no structural effect).
     * - Addresses of existing nodes remain stable.
     *
     * @par Complexity
     * - Time: O(n) in the worst case (must scan a full level to find a hole).
     * - Space: O(w) auxiliary for the queue, where w is the maximum level
     * width.
     *
     * @par Exception safety
     * - Strong: if allocation or `Type` construction throws, the tree is
     * unchanged. Possible throws: `std::bad_alloc`, exceptions from `Type`’s
     * constructor.
     *
     * @par Postconditions
     * - `size()` increased by 1; parent/child pointers set; `parent` of the new
     *   node points to its parent (or nullptr if it became root).
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
            Node<Type>* current = queue.dequeue();

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
     *
     * Complexity: O(n).
     * Exception safety: Propagates exceptions from `operator==` on `Type`.
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
     * Complexity: O(n) deleting every node exactly once.
     * Exception safety: No-throw.
     */
    void clear() noexcept {
        recursiveClear(root_);
        root_ = nullptr;
        size_ = 0;
    }


    /// Destructor
    virtual ~BinaryTree() noexcept { clear(); }
};

} // namespace data_structs

#endif // BINARYTREE_HPP
