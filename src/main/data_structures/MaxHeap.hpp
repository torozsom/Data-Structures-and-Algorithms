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
     * @tparam U The type of the element to be inserted, which must be
     * constructible into Type.
     * @param element The element to be inserted into the max-heap.
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
    [[nodiscard]]
    bool isValidHeap() const override {
        return isValidMaxHeap(this->root_);
    }


    ~MaxHeap() override = default;
};

#endif // MAXHEAP_HPP
