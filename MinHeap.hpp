
#ifndef MINHEAP_HPP
#define MINHEAP_HPP

#include "Heap.hpp"
#include "Stack.hpp"


template<typename Type>
class MinHeap final : public Heap<Type> {

    using Node = Node<Type>;

protected:
    /**
     * Restores the heap property by moving the given node upwards in the heap.
     * Starting from the given node, this method swaps its data with its parent's
     * data if its value is less than the parent's value, continuing until the
     * heap property is restored or the root is reached.
     *
     * @param node A pointer to the node that needs to be adjusted upwards.
     */
    void heapifyUp(Node* node) override {
        while (node && node->parent && node->data <= node->parent->data) {
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
    void heapifyDown(Node* node) override {
        while (node) {
            Node* smallest = node;

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
    Node* newNode = new Node(element);

    if (this->root_ == nullptr) {
        this->root_ = newNode;
        ++this->size_;
        return;
    }

    unsigned int n = this->size() + 1;
    std::bitset<32> binary(n);
    std::string path = binary.to_string();
    unsigned int index = path.find('1');
    path = path.substr(index + 1);

    Node* current = this->root_;
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


    /**
     * Removes and returns the root element of the heap, maintaining the heap property.
     *
     * This method retrieves the value of the root node, then replaces the root with
     * the value of the last node in level-order. After removing the last node, the
     * heap property is restored by a downward adjustment from the root.
     * If the heap is empty, an exception is thrown.
     *
     * @return The value of the root element that was removed from the heap.
     * @throws std::out_of_range If the heap is empty.
     */
    Type extractRoot() override {
        if (this->isEmpty())
            throw std::out_of_range("Heap is empty. Cannot extract root.");

        Type rootValue = this->root_->data;
        Node* lastNode = this->findLastNode();

        if (lastNode == this->root_) {
            delete this->root_;
            this->root_ = nullptr;
        } else {
            this->root_->data = lastNode->data;

            if (lastNode->parent->left == lastNode)
                lastNode->parent->left = nullptr;
            else
                lastNode->parent->right = nullptr;

            delete lastNode;
            heapifyDown(this->root_);
        }

        --this->size_;
        return rootValue;
    }


    void clear() override { Heap<Type>::clear(); }


    ~MinHeap() override { MinHeap::clear(); }

};

#endif // MINHEAP_HPP