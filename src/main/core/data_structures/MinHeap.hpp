#ifndef MINHEAP_HPP
#define MINHEAP_HPP


#include <initializer_list>
#include <type_traits>
#include <utility>

#include "Heap.hpp"


namespace containers {


/**
 * @class MinHeap
 * @brief Pointer-based binary **min-heap** over `Type`.
 *
 * Invariant: for every node `N`, `N->data` is **<=** each child's value.
 * (Duplicates are allowed; equality is permitted.)
 *
 * @tparam Type Element type.
 *
 * @section ordering Ordering & requirements
 * - Ordering uses `operator<` on `Type` (strict weak ordering assumed).
 * - Node payloads are moved during reheapification (`swapData`), not pointers.
 *
 * @section behavior Behavior
 * - `insert(U&&)`: append a new node at level-order index `size()+1`
 *   (computed via `findNodeByPath(path >> 1)`), link `parent/left/right`,
 *   increment `size_`, then `heapifyUp(newNode)`.
 * - `heapifyUp(Node*)`: bubble a node **up** while `node->data < parent->data`.
 * - `heapifyDown(Node*)`: push a node **down** by swapping with the smaller
 * child while that child's value is `< node->data`.
 * - `isValidHeap() const`: validates the min-heap property for
 * testing/debugging.
 *
 * @section complexity Complexity
 * - `insert`: O(log n) time, O(1) extra space.
 * - `heapifyUp` / `heapifyDown`: O(log n) time, O(1) space.
 * - `isValidHeap()`: O(n) time.
 *
 * @section exceptions Exception safety
 * - Propagates from allocations and `Type` move/assign.
 * - **Basic guarantee**: tree shape remains valid; values may be partially
 * moved if `Type` operations throw during reheapification.
 */
template <typename Type>
class MinHeap final : public Heap<Type> {

    /**
     * Restores the min-heap property by moving the given node upwards.
     * Iteratively swaps with the parent while node->data < parent->data.
     */
    void heapifyUp(Node<Type>* node) override {
        while (node && node->parent && node->data < node->parent->data) {
            this->swapData(node, node->parent);
            node = node->parent;
        }
    }


    /**
     * Restores the min-heap property by moving the given node downwards.
     * Iteratively swaps with the smaller child while that child’s value <
     * node->data.
     */
    void heapifyDown(Node<Type>* node) override {
        while (node) {
            Node<Type>* smallest = node;

            if (node->left && node->left->data < smallest->data)
                smallest = node->left;

            if (node->right && node->right->data < smallest->data)
                smallest = node->right;

            if (smallest == node)
                break;

            this->swapData(node, smallest);
            node = smallest;
        }
    }


    /// Checks if the heap maintains the min-heap property.
    static bool isValidMinHeap(const Node<Type>* node) {
        if (node == nullptr)
            return true;

        if (node->left && node->left->data < node->data)
            return false;
        if (node->right && node->right->data < node->data)
            return false;

        return isValidMinHeap(node->left) && isValidMinHeap(node->right);
    }


  public:
    /// Default constructor
    MinHeap() : Heap<Type>() {}


    /// Constructor for braced-init-lists
    MinHeap(std::initializer_list<Type> initial_data) : Heap<Type>() {
        for (const Type& element : initial_data)
            this->insert(element);
    }

    /**
     * Constructs a MinHeap from an array of elements.
     * This constructor inserts each element from the provided array into the
     * heap, ensuring that the min-heap property is maintained throughout the
     * insertion process.
     *
     * @param array Pointer to the array of elements to be inserted into the
     * heap.
     * @param size The number of elements in the array.
     */
    MinHeap(const Type* array, const size_t size) : Heap<Type>() {
        for (size_t i = 0; i < size; ++i)
            insert(array[i]);
    }

    /// Copy constructor
    MinHeap(const MinHeap& other) : Heap<Type>(other) {}

    /// Move constructor
    MinHeap(MinHeap&& other) noexcept : Heap<Type>(std::move(other)) {}

    /// Copy assignment operator
    MinHeap& operator=(const MinHeap& other) {
        Heap<Type>::operator=(other);
        return *this;
    }

    /// Move assignment operator
    MinHeap& operator=(MinHeap&& other) noexcept {
        Heap<Type>::operator=(std::move(other));
        return *this;
    }


    /**
     * Inserts a new element into the min-heap.
     * This method creates a new node with the provided element, finds the
     * correct position in the heap based on the current size, and restores the
     * min-heap property by moving the new node upwards as necessary.
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
     * @param element The element to be inserted into the heap.
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

        const size_t path = this->size() + 1;
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
        return isValidMinHeap(this->root_);
    }


    ~MinHeap() override = default;
};

} // namespace data_structs

#endif // MINHEAP_HPP
