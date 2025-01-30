
#ifndef MINHEAP_HPP
#define MINHEAP_HPP

#include "Heap.hpp"

template<typename Type>
class MinHeap : public Heap<Type> {

protected:
    /**
     * Ensures the heap property is maintained by moving a given node upwards in the heap.
     *
     * This function starts with a specified node and compares its value with its parent's value.
     * If the node's value is less than its parent's value (violating the min-heap property),
     * their values are swapped. This continues until the node is in the correct position
     * or it becomes the root of the heap.
     *
     * @param node Pointer to the node that needs to be adjusted upwards in the heap.
     */
    void heapifyUp(Node<Type>* node) override {
        if (node->parent == nullptr)
            return;

        while (node && node->parent && node->data < node->parent->data) {
            swapData(node->data, node->parent->data);
            node = node->parent;
        }
    }


    /**
     * Ensures the heap property is maintained by moving a given node downwards in the heap.
     *
     * This function starts with a specified node and compares its value with its children's values.
     * If any child node's value is smaller than the current node's value (violating the min-heap property),
     * their values are swapped. The process continues recursively until the current node is
     * in the correct position or there are no children that violate the heap property.
     *
     * @param node Pointer to the node that needs to be adjusted downwards in the heap.
     */
    void heapifyDown(Node<Type>* node) override {
        if (node == nullptr)
            return;

       while (node) {
           Node<Type>* smallest = node;

           if (node->left && node->left->data < smallest->data)
               smallest = node->left;
           if (node->right && node->right->data < smallest->data)
               smallest = node->right;
           if (smallest == node)
               break;

           swapData(node, smallest);
           node = smallest;
       }
    }



public:
    MinHeap() = default;

    MinHeap(const MinHeap& other) = default;

    MinHeap(MinHeap&& other) noexcept = default;

    MinHeap& operator=(const MinHeap& other) = default;

    MinHeap& operator=(MinHeap&& other) noexcept = default;



    void insert(const Type& element) override {
        Node<Type>* newNode = new Node<Type>(element);

        if (this->root == nullptr) {
            this->root = newNode;
            return;
        }

        Node<Type>* lastNode = this->findLastNode();

        if (lastNode->left == nullptr)
            lastNode->left = newNode;
        else
            lastNode->right = newNode;

        newNode->parent = lastNode;
        heapifyUp(newNode);
        ++this->size_;
    }

};


#endif //MINHEAP_HPP
