#ifndef MAXHEAP_HPP
#define MAXHEAP_HPP

#include "Heap.hpp"

template<typename Type>
class MaxHeap final : public Heap<Type> {

protected:
    /**
     * Restores the max-heap property by moving the given node upwards.
     * Starting from the given node, this method swaps the node's data with its parent
     * if its value is greater than the parent's value, continuing until
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
     * Starting from the given node, this method swaps its data with its largest child
     * if the largest child's value is greater than the current node's value, 
     * continuing recursively until the max-heap property is properly restored.
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

public:
    MaxHeap() = default;

    MaxHeap(const MaxHeap& other) = default;

    MaxHeap(MaxHeap&& other) noexcept = default;

    MaxHeap& operator=(const MaxHeap& other) = default;

    MaxHeap& operator=(MaxHeap&& other) noexcept = default;


    /**
     * Inserts a new element into the heap while maintaining the max-heap property.
     * The new element is placed in a determined location based on a binary path,
     * and then heapified upwards to maintain the heap property.
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

        unsigned int n = this->size() + 1;
        std::bitset<32> binary(n);
        std::string path = binary.to_string();
        unsigned int index = path.find('1');
        path = path.substr(index + 1);

        Node<Type>* current = this->root_;
        for (size_t i = 0; i < path.size() - 1; ++i) {
            if (path[i] == '0') {
                if (!current->left) {
                    current->left = newNode;
                    newNode->parent = current;
                    ++this->size_;
                    heapifyUp(newNode);
                    return;
                }
                current = current->left;
            } else {
                if (!current->right) {
                    current->right = newNode;
                    newNode->parent = current;
                    ++this->size_;
                    heapifyUp(newNode);
                    return;
                }
                current = current->right;
            }
        }

        if (path.back() == '0')
            current->left = newNode;
        else
            current->right = newNode;

        newNode->parent = current;
        ++this->size_;
        heapifyUp(newNode);
    }


    ~MaxHeap() override = default;

};

#endif // MAXHEAP_HPP