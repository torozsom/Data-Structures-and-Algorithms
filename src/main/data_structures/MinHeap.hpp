#ifndef MINHEAP_HPP
#define MINHEAP_HPP


#include "Heap.hpp"


/**
 * @class MinHeap
 *
 * A specialized heap data structure that maintains the min-heap property,
 * where each parent node is less than or equal to its child nodes.
 * This class provides methods for inserting elements while ensuring the
 * min-heap property is preserved.
 *
 * @tparam Type The type of elements stored in the heap.
 */
template <typename Type>
class MinHeap final : public Heap<Type> {

  private:
    /**
     * Restores the heap property by moving the given node upwards in the heap.
     * Starting from the given node, this method swaps its data with its
     * parent's data if its value is less than the parent's value, continuing
     * until the heap property is restored or the root is reached.
     *
     * @param node A pointer to the node that needs to be adjusted upwards.
     */
    void heapifyUp(Node<Type>* node) override {
        while (node && node->parent && node->data < node->parent->data) {
            this->swapData(node, node->parent);
            node = node->parent;
        }
    }


    /**
     * Restores the heap property by moving the given node downwards in the
     * heap. Starting from the given node, this method swaps its data with the
     * smallest child's data if the smallest child's value is less than the
     * current node's value, continuing recursively until the heap property is
     * restored or the node has no children that violate the property.
     *
     * @param node A pointer to the node that needs to be adjusted downwards.
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
    bool isValidMinHeap(Node<Type>* node) const {
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
    MinHeap(const Type* array, const std::size_t size) : Heap<Type>() {
        for (std::size_t i = 0; i < size; ++i)
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
     * @tparam U The type of the element to be inserted, which must be
     * constructible into Type.
     * @param element The element to be inserted into the heap.
     */
    template <typename U>
    void insert(U&& element) {
        static_assert(std::is_constructible_v<Type, U&&>,
                      "Element must be constructible into Type");

        Node<Type>* newNode = new Node<Type>(std::forward<U>(element));

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
        return isValidMinHeap(this->root_);
    }


    ~MinHeap() override = default;
};

#endif // MINHEAP_HPP
