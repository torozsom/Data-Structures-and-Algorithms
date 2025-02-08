
#ifndef MINHEAP_HPP
#define MINHEAP_HPP

#include "Heap.hpp"
#include "Stack.hpp"


template<typename Type>
class MinHeap : public Heap<Type> {

    using HeapNode = HeapNode<Type>;

protected:
    /**
     * Restores the heap property by moving the given node upwards in the heap.
     * Starting from the given node, this method swaps its data with its parent's
     * data if its value is less than the parent's value, continuing until the
     * heap property is restored or the root is reached.
     *
     * @param node A pointer to the node that needs to be adjusted upwards.
     */
    void heapifyUp(HeapNode* node) override {
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
    void heapifyDown(HeapNode* node) override {
        while (node) {
            HeapNode* smallest = node;

            if (node->left && node->left->data < smallest->data)
                smallest = static_cast<HeapNode*>(node->left);

            if (node->right && node->right->data < smallest->data)
                smallest = static_cast<HeapNode*>(node->right);

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
     * Inserts a new value into the MinHeap while maintaining the min-heap property.
     * The new value is added as the last node in the binary tree, and the heapifyUp
     * function is called to restore the min-heap structure.
     *
     * @param element The value to be inserted into the heap.
     */
    void insert(const Type& element) override {
        auto* newNode = new HeapNode(element);

        if (this->root_ == nullptr)
            this->root_ = newNode;
        else {
            Stack<HeapNode*> stack;
            stack.push(static_cast<HeapNode*>(this->root_));

            while (!stack.isEmpty()) {
                HeapNode* current = stack.pop();

                if (!current->left) {
                    current->left = newNode;
                    newNode->parent = current;
                    break;
                } else {
                    stack.push(static_cast<HeapNode*>(current->left));
                }

                if (!current->right) {
                    current->right = newNode;
                    newNode->parent = current;
                    break;
                } else {
                    stack.push(static_cast<HeapNode*>(current->right));
                }
            }
        }

        ++this->size_;
        this->heapifyUp(newNode);
    }


    /**
     * Removes the specified element from the heap while maintaining the min-heap property.
     * The element is searched for: if found, it is removed, and the structure is corrected
     * using heapifyDown.
     *
     * @param element The value to be removed from the heap.
     */
    void remove(const Type& element) override {
        if (this->root_ == nullptr)
            throw std::out_of_range("Heap is empty");

        Stack<HeapNode*> stack;
        stack.push(static_cast<HeapNode*>(this->root_));
        HeapNode* target = nullptr;

        while (!stack.isEmpty()) {
            HeapNode* current = stack.pop();

            if (current->data == element) {
                target = current;
                break;
            }

            if (current->right)
                stack.push(static_cast<HeapNode*>(current->right));

            if (current->left)
                stack.push(static_cast<HeapNode*>(current->left));
        }

        if (target == nullptr)
            throw std::invalid_argument("Element not found in heap");

        HeapNode* lastNode = this->findLastNode();

        if (target != lastNode) {
            target->data = lastNode->data;

            if (lastNode->parent) {
                if (lastNode->parent->left == lastNode)
                    lastNode->parent->left = nullptr;
                else if (lastNode->parent->right == lastNode)
                    lastNode->parent->right = nullptr;
            } else {
                this->root_ = nullptr;
            }

            delete lastNode;
            --this->size_;
            this->heapifyDown(target);

        } else {
            if (lastNode->parent) {
                if (lastNode->parent->left == lastNode)
                    lastNode->parent->left = nullptr;
                else if (lastNode->parent->right == lastNode)
                    lastNode->parent->right = nullptr;
            } else {
                this->root_ = nullptr;
            }

            delete lastNode;
            --this->size_;
        }
    }


    Type extractRoot() override {
        if (this->root_ == nullptr)
            throw std::out_of_range("Heap is empty");

        Type value = this->root_->data;
        remove(value);
        return value;
    }


    void clear() override { Heap<Type>::clear(); }


    ~MinHeap() override { MinHeap::clear(); }
};

#endif // MINHEAP_HPP