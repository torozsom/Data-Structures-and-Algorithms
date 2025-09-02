#ifndef HEAP_HPP
#define HEAP_HPP


#include <limits>
#include <stdexcept>
#include <utility>

#include "BinaryTree.hpp"


namespace containers {


/**
 * @class Heap
 * @brief Abstract base for pointer-based binary heaps over `Type`.
 *
 * Supplies the shape management and shared mechanics for a heap stored as a
 * *complete* binary tree of linked nodes (with `parent/left/right`). Ordering
 * (min/max) is delegated to derived classes via `heapifyUp` / `heapifyDown`.
 *
 * @tparam Type Element type. Must be MoveConstructible or CopyConstructible.
 *
 * @section shape Shape & storage
 * - The tree is always complete: levels are filled left-to-right.
 * - Child/parent placement is computed by bit-walking a 1-based level-order
 *   index (see `findNodeByPath()`).
 * - Nodes store `Type` by value; swaps move payloads, not pointers.
 *
 * @section base What the base provides
 * - `extractRoot()` — remove & return the root, then restore the heap via
 *   `heapifyDown()`; **O(log n)**.
 * - `peekRoot()` — read (or move from, rvalue overload) the root without
 * removal; **O(1)**.
 * - Helpers: `findNodeByPath()`, `findLastNode()`, `swapData()`.
 * - Deleted `insertLeft/insertRight` to protect the complete-tree invariant.
 *
 * @section derived What derived classes must provide
 * - `heapifyUp(Node*)` and `heapifyDown(Node*)` to enforce their ordering.
 * - `isValidHeap() const` for validation/testing.
 * - An `insert(U&&)` that:
 *    1) creates a new node at index `size()+1` using `findNodeByPath(path>>1)`,
 *    2) links `parent/left/right`, increments `size_`,
 *    3) calls `heapifyUp(newNode)`.
 *
 * @par Duplicate policy
 * Duplicates are allowed; ordering semantics are defined by the derived type.
 *
 * @par Complexity
 * - `extractRoot()`: O(log n) time, O(1) extra space.
 * - `peekRoot()`: O(1) time/space.
 * - `findNodeByPath()`: O(log n) time, O(1) space.
 *
 * @par Exception safety
 * Operations propagate exceptions from `Type` move/assign. The base offers the
 * **basic** guarantee: the tree remains structurally valid; element values may
 * be partially moved if a `Type` operation throws.
 */
template <typename Type>
class Heap : public BinaryTree<Type> {

  protected:
    virtual void heapifyUp(Node<Type>* node) = 0;
    virtual void heapifyDown(Node<Type>* node) = 0;


    /**
     * @brief Find a node by its 1-based array index in the complete tree.
     *
     * Interprets `index` in binary, skips the most-significant bit (the root),
     * then walks remaining bits high→low: bit 0 = go left, bit 1 = go right.
     *
     * @param idx 1-based index (1 = root).
     * @return Pointer to the node at that index, or nullptr if the path is
     * invalid.
     *
     * @complexity Time: O(log n); Space: O(1).
     */
    Node<Type>* findNodeByPath(const size_t idx) const noexcept {
        if (this->isEmpty())
            return nullptr;

        if (idx == 0)
            return nullptr;

        if (idx == 1)
            return this->root_;

        size_t msb = static_cast<size_t>(1)
                     << (std::numeric_limits<size_t>::digits - 1);
        while (msb > 0 && !(idx & msb))
            msb >>= 1;

        Node<Type>* current = this->root_;
        msb >>= 1;

        while (msb && current) {
            if (idx & msb)
                current = current->right;
            else
                current = current->left;
            msb >>= 1;
        }

        return current;
    }


    /**
     * @brief Locate the node representing the last element in level-order.
     *
     * Computes the 1-based index equal to `size()` and delegates to
     * `findNodeByPath` to follow the corresponding left/right edges. Used by
     * `extractRoot()` when splicing out the final node.
     *
     * @return Pointer to the last node, or nullptr if the heap is empty.
     *
     * @complexity Time: O(log n); Space: O(1).
     */
    Node<Type>* findLastNode() const noexcept {
        return findNodeByPath(this->size());
    }


    /**
     * @brief Swap the payloads of two nodes using move operations.
     *
     * No effect if either pointer is null or both pointers are equal.
     * May propagate exceptions from `Type` move construction/assignment.
     *
     * @complexity O(1).
     */
    void swapData(Node<Type>* node1, Node<Type>* node2) noexcept {
        if (!node1 || !node2 || node1 == node2)
            return;

        Type temp = std::move(node1->data);
        node1->data = std::move(node2->data);
        node2->data = std::move(temp);
    }


  public:
    /// Default constructor
    Heap() noexcept : BinaryTree<Type>() {}

    /// Copy constructor
    Heap(const Heap& other) : BinaryTree<Type>(other) {}

    /// Move constructor
    Heap(Heap&& other) noexcept : BinaryTree<Type>(std::move(other)) {}

    /// Copy assignment operator
    Heap& operator=(const Heap& other) {
        BinaryTree<Type>::operator=(other);
        return *this;
    }

    /// Move assignment operator
    Heap& operator=(Heap&& other) noexcept {
        BinaryTree<Type>::operator=(std::move(other));
        return *this;
    }


    template <typename U> void insertLeft(U&&) = delete;
    template <typename U> void insertRight(U&&) = delete;


    [[nodiscard]]
    virtual bool isValidHeap() const = 0;


    /**
     * @brief Remove and return the root while maintaining the heap property.
     *
     * Swaps the root with the last node (by 1-based level-order index),
     * unlinks and deletes the last node, then heapifies down from the root.
     *
     * @return The former root value (moved).
     * @throws std::out_of_range if the heap is empty.
     *
     * @complexity Time: O(log n); Space: O(1).
     */
    Type extractRoot() {
        if (this->isEmpty())
            throw std::out_of_range("Heap is empty");
        Type out = std::move(this->root_->data);

        Node<Type>* last = findLastNode();
        if (last == this->root_) {
            delete this->root_;
            this->root_ = nullptr;
            this->size_ = 0;
            return out;
        }

        this->swapData(this->root_, last);

        if (last->parent->left == last)
            last->parent->left = nullptr;
        else
            last->parent->right = nullptr;

        delete last;
        --this->size_;
        heapifyDown(this->root_);
        return out;
    }


    /**
     * @brief Move out the root value from an rvalue heap without removing it.
     * @return The root value (moved).
     * @throws std::out_of_range if the heap is empty.
     * @complexity O(1).
     */
    [[nodiscard]]
    Type peekRoot() && {
        if (this->isEmpty())
            throw std::out_of_range("Heap is empty");
        return std::move(this->root_->data);
    }


    /**
     * @brief Access the root value without removing it.
     * @return Const reference to the root value.
     * @throws std::out_of_range if the heap is empty.
     * @complexity O(1).
     */
    [[nodiscard]]
    const Type& peekRoot() const& {
        if (this->isEmpty())
            throw std::out_of_range("Heap is empty");
        return this->root_->data;
    }


    /// Destructor
    ~Heap() override = default;
};

} // namespace containers

#endif // HEAP_HPP
