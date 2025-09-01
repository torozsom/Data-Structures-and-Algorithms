#ifndef BINARYSEARCHTREE_HPP
#define BINARYSEARCHTREE_HPP


#include <initializer_list>
#include <type_traits>
#include <utility>

#include "BinaryTree.hpp"


namespace containers {


/**
 * @class BinarySearchTree
 * @brief Unbalanced binary search tree storing elements of type `Type`.
 *
 * Each node has up to two children. The BST invariant is strict ordering:
 * for every node N, all values in N->left are < N->data and all values in
 * N->right are > N->data. Equal values are ignored (no duplicate insert).
 *
 * @tparam Type Element type.
 *
 * @par Type requirements
 * - `Type` must be MoveConstructible or CopyConstructible (node storage).
 * - A strict weak ordering via `operator<` must be defined for `Type`
 *   (search/validation).  (Tip: you can implement comparisons using only `<`.)
 *
 * @par Behavior & guarantees
 * - Not self-balancing; shape depends on insertion order.
 * - Parent pointers are maintained in nodes.
 * - Duplicate policy: duplicates are not inserted.
 *
 * @par Complexity (typical operations)
 * - Average (random-ish data): O(log n) height ⇒ insert/find/remove ~ O(log n).
 * - Worst case (degenerate/unbalanced): height O(n) ⇒ operations ~ O(n).
 * - Space: O(n) nodes; recursion depth up to tree height.
 *
 * @par Exception safety
 * - Insert/remove propagate exceptions from allocations and `Type`
 *   construction/move/assign. Insert offers a strong guarantee (tree
 *   unchanged on failure).
 */
template <typename Type>
class BinarySearchTree : public BinaryTree<Type> {

    /**
     * Recursively inserts an element into a binary search tree.
     *
     * Traverses from `node` to find the proper spot: values less than go left,
     * greater go right. Equal values are ignored (no duplicate insert). If
     * `node` is null, creates a new node and links its parent.
     *
     * @tparam U  Value type constructible into `Type`.
     * @param node    Reference to the current child pointer being examined.
     * @param element Value to insert.
     * @param parent  Parent of the current node (used when creating a child).
     *
     * @complexity Time: O(h) where h is the tree height (avg ~ O(log n), worst
     * O(n)); Space: O(h) recursion depth.
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
     * Recursively removes an element from a BST.
     *
     * Cases:
     * - Leaf: delete node.
     * - One child: splice child up, fix parent pointer.
     * - Two children: replace with in-order successor (min of right subtree),
     *   then erase that successor from the right subtree.
     *
     * @param node    Reference to the current child pointer being examined.
     * @param element Value to remove.
     *
     * @complexity Time: O(h) (avg ~ O(log n), worst O(n)); Space: O(h)
     * recursion.
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
    static Node<Type>* findMinNode(Node<Type>* node) {
        if (node == nullptr)
            return nullptr;

        while (node->left != nullptr)
            node = node->left;
        return node;
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
     * @param node A const pointer to the root of the subtree in which to find
     * the minimum node.
     * @return A const pointer to the node containing the minimum value in the
     * subtree, or nullptr if the subtree is empty.
     */
    static const Node<Type>* findMinNode(const Node<Type>* node) {
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
    static Node<Type>* findMaxNode(Node<Type>* node) {
        if (node == nullptr)
            return nullptr;

        while (node->right != nullptr)
            node = node->right;
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
     * @param node A const pointer to the root of the subtree in which to find
     * the maximum node.
     * @return A const pointer to the node containing the maximum value in the
     * subtree, or nullptr if the subtree is empty.
     */
    static const Node<Type>* findMaxNode(const Node<Type>* node) {
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

        if ((minVal && !(*minVal < node->data)) ||
            (maxVal && !(node->data < *maxVal)))
            return false;

        return isValidBSTHelper(node->left, minVal, &node->data) &&
               isValidBSTHelper(node->right, &node->data, maxVal);
    }


  public:
    /// Default constructor
    BinarySearchTree() noexcept : BinaryTree<Type>() {}

    /// Constructor for braced-init-lists
    BinarySearchTree(std::initializer_list<Type> initial_data)
        : BinaryTree<Type>() {
        for (const Type& element : initial_data)
            this->insert(element);
    }

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
    BinarySearchTree(const Type* array, const size_t size)
        : BinaryTree<Type>() {
        for (size_t i = 0; i < size; ++i)
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


    /**
     * @brief Verify the BST invariant (strict ordering) over the whole tree.
     * @return true if every node N satisfies (all left < N.data < all right).
     *
     * @par Complexity
     * - Time: O(n), Space: O(h) recursion depth.
     *
     * @par Notes
     * - Uses only `operator<` on `Type` (strict ordering).
     */
    [[nodiscard]]
    bool isValidBST() const {
        return isValidBSTHelper(this->root_);
    }


    /**
     * @brief Insert a value while maintaining the BST invariant.
     *
     * Recursively descends from the root: values less than the current node go
     * left, greater values go right. Equal values are ignored (no duplicate
     * insert). If the tree is empty, the new node becomes the root.
     *
     * @tparam U  A type that can construct `Type` (perfect-forwarded).
     * @param element  The value to insert.
     *
     * @par Duplicate policy
     * - Strict BST: duplicates are **not inserted**.
     *
     * @par Complexity
     * - Time: O(h) where h is the tree height (average ≈ O(log n), worst O(n)).
     * - Space: O(h) recursion depth.
     *
     * @par Exception safety
     * - Strong: if allocation or `Type` construction/assignment throws, the
     * tree is unchanged.
     */
    template <typename U>
    void insert(U&& element) {
        recursiveInsert(this->root_, std::forward<U>(element));
    }


    template <typename U> void insertLeft(U&&) = delete;
    template <typename U> void insertRight(U&&) = delete;


    /**
     * @brief Remove a value if present, maintaining the BST invariant.
     *
     * Standard cases:
     * - Leaf: delete the node.
     * - One child: splice the child up and relink its parent.
     * - Two children: replace node's value with the minimum from the right
     * subtree (in-order successor), then remove that successor node.
     *
     * @param element  The value to erase (if present).
     *
     * @par Complexity
     * - Time: O(h) (average ≈ O(log n), worst O(n)).
     * - Space: O(h) recursion depth.
     *
     * @par Exception safety
     * - Basic/strong depending on `Type` move-assign; if assigning the
     * successor value throws, no node is removed.
     */
    void remove(const Type& element) { recursiveRemove(this->root_, element); }


    /**
     * @brief Check whether a value exists in the tree.
     * @param element The value to search for.
     * @return true if found, false otherwise.
     *
     * @par Complexity
     * - Time: O(h) (average ≈ O(log n), worst O(n)).
     * - Space: O(1).
     */
    bool contains(const Type& element) const override {
        Node<Type>* current = this->root_;
        while (current) {
            if (element < current->data)
                current = current->left;
            else if (current->data < element)
                current = current->right;
            else
                return true;
        }
        return false;
    }


    /**
     * @brief Return the smallest value in the tree.
     * @return The minimum value.
     * @throws std::runtime_error if the tree is empty.
     *
     * @par Complexity
     * - Time: O(h), Space: O(1).
     */
    [[nodiscard]]
    Type findMinimum() const {
        if (this->isEmpty())
            throw std::runtime_error("Tree is empty");
        return findMinNode(this->root_)->data;
    }


    /**
     * @brief Return the greatest value in the tree.
     * @return The maximum value.
     * @throws std::runtime_error if the tree is empty.
     *
     * @par Complexity
     * - Time: O(h), Space: O(1).
     */
    [[nodiscard]]
    Type findMaximum() const {
        if (this->isEmpty())
            throw std::runtime_error("Tree is empty");
        return findMaxNode(this->root_)->data;
    }


    ~BinarySearchTree() override = default;
};

} // namespace data_structs

#endif // BINARYSEARCHTREE_HPP
