

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
template<typename Type>
class MinHeap final : public Heap<Type> {

protected:
    /**
     * Restores the heap property by moving the given node upwards in the heap.
     * Starting from the given node, this method swaps its data with its parent's
     * data if its value is less than the parent's value, continuing until the
     * heap property is restored or the root is reached.
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
     * Restores the heap property by moving the given node downwards in the heap.
     * Starting from the given node, this method swaps its data with the smallest
     * child's data if the smallest child's value is less than the current node's
     * value, continuing recursively until the heap property is restored or the node
     * has no children that violate the property.
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



public:
    MinHeap() = default;

    MinHeap(const MinHeap& other) = default;

    MinHeap(MinHeap&& other) noexcept = default;

    MinHeap& operator=(const MinHeap& other) = default;

    MinHeap& operator=(MinHeap&& other) noexcept = default;


    /**
     * Inserts a new element into the heap while maintaining the heap property.
     * This method locates the correct position for the new element by following
     * a binary path derived from a level-order traversal. After insertion, the
     * heap property is restored by repositioning the new element upwards in the
     * heap if necessary.
     *
     * @param element The element to be inserted into the heap.
     */
    void insert(const Type& element) override {
        Node<Type>* newNode = new Node(element);

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


    ~MinHeap() override = default;

};

#endif // MINHEAP_HPP