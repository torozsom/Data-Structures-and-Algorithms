#ifndef HEAP_HPP
#define HEAP_HPP


#include "BinaryTree.hpp"
#include <limits>


/**
 * @class Heap: Abstract class
 *
 * A generic heap data structure that stores elements of a given type.
 * The heap is a binary tree structure that satisfies the heap property,
 * which depends on the type of heap (min-heap or max-heap).
 *
 * In a min-heap, the parent node is less than or equal to its children, while
 * in a max-heap, the parent node is greater than or equal to its children. The
 * heap structure is commonly used to implement priority queues and sorting
 * algorithms. This class provides common functionality for both min-heaps and
 * max-heaps.
 *
 * @tparam Type The type of elements stored in the heap.
 */
template <typename Type>
class Heap : public BinaryTree<Type> {

  protected:
    virtual void heapifyUp(Node<Type>* node) = 0;
    virtual void heapifyDown(Node<Type>* node) = 0;


    /**
     * @brief Finds a node in the complete binary tree (heap) using bit
     * manipulation.
     *
     * This method determines the path to the node by examining the binary
     * representation of the node's index. After finding the most significant
     * bit (MSB), it uses the remaining bits to navigate the tree: 0 for left
     * child and 1 for right child.
     *
     * The implementation uses efficient bit manipulation operations:
     * - MSB detection through right-shifting
     * - Bitwise AND for path determination
     *
     * @param path The index of the node to find.
     * @return Pointer to the node at index n, or nullptr if not found.
     *
     * @complexity Time: O(log n) where n is the number of nodes
     * @complexity Space: O(1) - uses only a constant amount of extra space
     */
    Node<Type>* findNodeByPath(const std::size_t path) const {
        if (this->isEmpty())
            return nullptr;

        std::size_t msb = static_cast<size_t>(1)
                          << (std::numeric_limits<size_t>::digits - 1);
        while (msb > 0 && !(path & msb))
            msb >>= 1;

        Node<Type>* current = this->root_;
        msb >>= 1;

        while (msb && current) {
            if (path & msb)
                current = current->right;
            else
                current = current->left;
            msb >>= 1;
        }

        return current;
    }


    /**
     * Finds the last node in the heap based on the current size.
     *
     * This method calculates the path to the last node using the size of the
     * heap. It uses a binary representation of the size to determine the
     * correct path in the complete binary tree structure of the heap.
     *
     * @return Pointer to the last node in the heap, or nullptr if the heap is
     * empty.
     */
    const Node<Type>* findLastNode() const {
        return findNodeByPath(this->size());
    }


    /**
     * Swaps the data stored in two nodes of the binary tree.
     *
     * This method exchanges the `data` field of the two provided nodes. If
     * either node is null, the method does nothing.
     *
     * @param node1 Pointer to the first node involved in the swap operation.
     * @param node2 Pointer to the second node involved in the swap operation.
     */
    void swapData(Node<Type>* node1, Node<Type>* node2) {
        if (node1 == nullptr || node2 == nullptr)
            return;

        Type temp = node1->data;
        node1->data = node2->data;
        node2->data = temp;
    }


  public:
    /// Default constructor
    Heap() : BinaryTree<Type>() {}

    /// Copy constructor
    Heap(const Heap& other) : BinaryTree<Type>(other) {}

    /// Move constructor
    Heap(Heap&& other) noexcept : BinaryTree<Type>(std::move(other)) {}

    /// Copy assignment operator
    Heap& operator=(const Heap& other) {
        BinaryTree<Type>::operator=(other);
        return *this;
    }

    /// Move assignment operator
    Heap& operator=(Heap&& other) noexcept {
        BinaryTree<Type>::operator=(std::move(other));
        return *this;
    }

    void insertLeft(const Type& element) = delete;
    void insertRight(const Type& element) = delete;

    [[nodiscard]]
    virtual bool isValidHeap() const = 0;


    /**
     * Removes and returns the root element of the heap, maintaining the heap
     * property.
     *
     * This method retrieves the value of the root node, then replaces the root
     * with the value of the last node in level-order. After removing the last
     * node, the heap property is restored by a downward adjustment from the
     * root. If the heap is empty, an exception is thrown.
     *
     * @return The value of the root element that was removed from the heap.
     * @throws std::out_of_range If the heap is empty.
     */
    Type extractRoot() {
        if (this->isEmpty())
            throw std::out_of_range("Heap is empty. Cannot extract root.");

        Type rootValue = this->root_->data;
        const Node<Type>* lastNode = this->findLastNode();

        if (lastNode == this->root_) {
            delete this->root_;
            this->root_ = nullptr;
            this->size_ = 0;
        } else {
            this->root_->data = lastNode->data;

            if (lastNode->parent->left == lastNode)
                lastNode->parent->left = nullptr;
            else
                lastNode->parent->right = nullptr;

            delete lastNode;
            --this->size_;
            heapifyDown(this->root_);
        }

        return rootValue;
    }


    /**
     * Retrieves and returns the value stored at the root of the heap.
     *
     * This method provides access to the root element of the heap without
     * removing it. If the heap is empty, an exception is thrown to indicate the
     * error.
     *
     * @return The value stored at the root of the heap.
     * @throws std::out_of_range If the heap is empty.
     */
    Type peekRoot() const {
        if (this->isEmpty())
            throw std::out_of_range("Heap is empty");
        return this->root_->data;
    }


    /// Destructor
    ~Heap() override = default;
};

#endif // HEAP_HPP
