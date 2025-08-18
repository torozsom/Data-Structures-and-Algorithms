#ifndef MAXHEAP_HPP
#define MAXHEAP_HPP


#include <type_traits>
#include <utility>

#include "Heap.hpp"


/**
 * @class MaxHeap
 * @brief Pointer-based max-heap (binary tree) storing elements of type `Type`.
 *
 * Maintains the max-heap property: for every node `N`, `N->data` is **>=** each
 * child’s value (ties allowed). The physical structure is a complete binary
 * tree built with linked nodes (parent/left/right), not an array. New nodes are
 * placed by their 1-based level-order index; restoration uses upward/downward
 * heapify.
 *
 * @tparam Type Element type.
 *
 * @par Type requirements
 * - `Type` must be MoveConstructible or CopyConstructible (node storage/moves).
 * - A strict weak ordering must be available; this implementation uses
 *   `operator>` comparisons internally.
 *
 * @par Duplicate policy
 * - Duplicates are **allowed** (equality does not trigger reordering).
 *
 * @par Operations (time / space)
 * - `insert(U&&)`: O(log n) / O(1) — insert at the end, then heapify up.
 * - `extractRoot()` (inherited): O(log n) / O(1) — swap with last, remove last,
 * heapify down.
 * - `peekRoot()` (inherited): O(1) / O(1) — access max without removal.
 * - `isValidHeap()`: O(n) / O(h) recursion depth.
 * - `size()` / `isEmpty()`: O(1).
 *
 * @par Storage model
 * - Linked nodes with `parent` pointers; placement found via bit-walking the
 *   level-order index (1-based).
 *
 * @par Exception safety
 * - Operations propagate exceptions from allocations and `Type` move/compare.
 *   Provides the **basic** guarantee (structure remains valid; values may be
 *   partially moved if `Type` move/assign throws).
 */
template <typename Type>
class MaxHeap final : public Heap<Type> {

    /**
     * Restores the max-heap property by moving the given node upwards.
     * Iteratively swaps with the parent while node->data > parent->data.
     */
    void heapifyUp(Node<Type>* node) override {
        while (node && node->parent && node->data > node->parent->data) {
            this->swapData(node, node->parent);
            node = node->parent;
        }
    }


    /**
     * Restores the max-heap property by moving the given node downwards.
     * Iteratively swaps with the larger child while that child’s value >
     * node->data.
     */
    void heapifyDown(Node<Type>* node) override {
        while (node) {
            Node<Type>* largest = node;

            if (node->left && node->left->data > largest->data)
                largest = node->left;

            if (node->right && node->right->data > largest->data)
                largest = node->right;

            if (largest == node)
                break;

            this->swapData(node, largest);
            node = largest;
        }
    }


    /// Checks if the heap maintains the max-heap property.
    static bool isValidMaxHeap(const Node<Type>* node) {
        if (node == nullptr)
            return true;

        if (node->left && node->left->data > node->data)
            return false;
        if (node->right && node->right->data > node->data)
            return false;

        return isValidMaxHeap(node->left) && isValidMaxHeap(node->right);
    }


  public:
    /// Default constructor
    MaxHeap() : Heap<Type>() {}

    /**
     * Constructs a MaxHeap from an array of elements. Each element is inserted
     * into the heap, maintaining the max-heap property.
     *
     * @param array Pointer to the array of elements to be inserted into the
     * heap.
     * @param size The number of elements in the array.
     */
    MaxHeap(const Type* array, const std::size_t size) : Heap<Type>() {
        for (std::size_t i = 0; i < size; ++i)
            insert(array[i]);
    }

    /// Copy constructor
    MaxHeap(const MaxHeap& other) : Heap<Type>(other) {}

    /// Move constructor
    MaxHeap(MaxHeap&& other) noexcept : Heap<Type>(std::move(other)) {}

    /// Copy assignment operator
    MaxHeap& operator=(const MaxHeap& other) {
        Heap<Type>::operator=(other);
        return *this;
    }

    /// Move assignment operator
    MaxHeap& operator=(MaxHeap&& other) noexcept {
        Heap<Type>::operator=(std::move(other));
        return *this;
    }


    /**
     * Inserts an element into the max-heap. The element is added as a new node
     * at the end of the heap, and then the heap property is restored by moving
     * the new node upwards if necessary.
     *
     * @par Complexity
     * Time: O(log n), Space: O(1).
     *
     * @par Exceptions
     * Basic guarantee: structure remains valid;
     * values may be partially moved if Type move throws.
     *
     * @tparam U The type of the element to be inserted, which must be
     * constructible into Type.
     * @param element The element to be inserted into the max-heap.
     */
    template <typename U>
    void insert(U&& element) {
        static_assert(std::is_constructible_v<Type, U&&>,
                      "Element must be constructible into Type");

        auto* newNode = new Node<Type>(std::forward<U>(element));

        if (this->isEmpty()) {
            this->root_ = newNode;
            ++this->size_;
            return;
        }

        const std::size_t path = this->size() + 1;
        Node<Type>* parent = this->findNodeByPath(path >> 1);
        newNode->parent = parent;

        if (path & 1)
            parent->right = newNode;
        else
            parent->left = newNode;

        ++this->size_;
        heapifyUp(newNode);
    }


    /// Checks if the heap maintains the correct heap property.
    [[nodiscard]]
    bool isValidHeap() const override {
        return isValidMaxHeap(this->root_);
    }


    ~MaxHeap() override = default;
};

#endif // MAXHEAP_HPP
