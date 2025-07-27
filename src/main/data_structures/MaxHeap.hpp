#ifndef MAXHEAP_HPP
#define MAXHEAP_HPP


#include "Heap.hpp"


/**
 * @class MaxHeap
 *
 * A specialized heap data structure that maintains the max-heap property,
 * where each parent node is greater than or equal to its child nodes.
 * This class provides methods for inserting elements while ensuring the
 * max-heap property is preserved.
 *
 * @tparam Type The type of elements stored in the heap.
 */
template <typename Type>
class MaxHeap final : public Heap<Type> {

  private:
    /**
     * Restores the max-heap property by moving the given node upwards.
     * Starting from the given node, this method swaps the node's data with its
     * parent if its value is greater than the parent's value, continuing until
     * the heap property is restored or the root is reached.
     *
     * @param node A pointer to the node that needs adjustment upwards.
     */
    void heapifyUp(Node<Type>* node) override {
        while (node && node->parent && node->data > node->parent->data) {
            this->swapData(node, node->parent);
            node = node->parent;
        }
    }


    /**
     * Restores the max-heap property by moving the given node downwards.
     * Starting from the given node, this method swaps its data with its largest
     * child if the largest child's value is greater than the current node's
     * value, continuing recursively until the max-heap property is properly
     * restored.
     *
     * @param node A pointer to the node that needs adjustment downwards.
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
    bool isValidMaxHeap(Node<Type>* node) const {
        if (node == nullptr)
            return true;

        if (node->left && node->left->data > node->data)
            return false;
        if (node->right && node->right->data > node->data)
            return false;

        return isValidMaxHeap(node->left) && isValidMaxHeap(node->right);
    }


  public:
    MaxHeap() : Heap<Type>() {}

    MaxHeap(const Type* array, const std::size_t size) : Heap<Type>() {
        for (std::size_t i = 0; i < size; ++i) {
            this->insert(array[i]);
        }
    }

    MaxHeap(const MaxHeap& other) : Heap<Type>(other) {}

    MaxHeap(MaxHeap&& other) noexcept : Heap<Type>(std::move(other)) {}

    MaxHeap& operator=(const MaxHeap& other) {
        Heap<Type>::operator=(other);
        return *this;
    }

    MaxHeap& operator=(MaxHeap&& other) noexcept {
        Heap<Type>::operator=(std::move(other));
        return *this;
    }


    /**
     * Inserts a new element into the heap while maintaining the max-heap
     * property. The new element is placed in a determined location based on a
     * binary path, and then heapified upwards to maintain the heap property.
     *
     * @param element The element to be inserted into the heap.
     */
    void insert(const Type& element) override {
        Node<Type>* newNode = new Node<Type>(element);

        if (this->isEmpty()) {
            this->root_ = newNode;
            ++this->size_;
            return;
        }

        std::size_t path = this->size() + 1;
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
    bool isValidHeap() const override { return isValidMaxHeap(this->root_); }


    ~MaxHeap() override = default;
};

#endif // MAXHEAP_HPP