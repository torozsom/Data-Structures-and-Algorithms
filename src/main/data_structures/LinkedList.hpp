

#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP


#include <iostream>



/**
 * @class LinkedList
 *
 * A generic doubly linked list class that provides functionalities for adding,
 * removing, and accessing elements. The internal storage is implemented using
 * a doubly linked list structure.
 */
template<typename Type>
class LinkedList {

private:
    struct Node {
        Type data;
        Node* next;
        Node* prev;
        explicit Node(const Type& data) : data(data), next(nullptr), prev(nullptr) {}
    };

    Node* head_;
    Node* tail_;
    unsigned int size_;


public:
    /// Default constructor
    LinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}


    LinkedList(const Type* array, const unsigned int size) : head_(nullptr), tail_(nullptr), size_(0) {
        for (unsigned int i = 0; i < size; ++i)
            addLast(array[i]);
    }


    /// Copy constructor
    LinkedList(const LinkedList& other) : head_(nullptr), tail_(nullptr), size_(other.size_) {
        Node* current = other.head_;
        while (current != nullptr) {
            addLast(current->data);
            current = current->next;
        }
    }


    /// Move constructor
    LinkedList(LinkedList&& other) noexcept
        : head_(other.head_), tail_(other.tail_), size_(other.size_) {
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }


    Node* getHead() const { return head_; }

    Node* getTail() const { return tail_; }

    unsigned int getSize() const { return size_; }

    bool isEmpty() const { return size_ == 0; }


    /// Copy assignment operator
    LinkedList& operator=(const LinkedList& other) {
        if (this == &other)
            return *this;

        clear();

        Node* current = other.head_;
        while (current != nullptr) {
            add(current->data);
            current = current->next;
        }

        return *this;
    }


    /// Move assignment operator
    LinkedList& operator=(LinkedList&& other) noexcept {
        if (this == &other)
            return *this;

        clear();

        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;

        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;

        return *this;
    }


    /**
     * Adds a new element to the end of the linked list.
     *
     * @param element The element of type Type to be added to the linked list.
     */
    void addLast(const Type& element) {
        Node* new_node = new Node(element);

        if (head_ == nullptr) {
            head_ = new_node;
            tail_ = new_node;
        } else {
            tail_->next = new_node;
            new_node->prev = tail_;
            tail_ = new_node;
        }

        ++size_;
    }


    /**
     * Adds a new element to the beginning of the linked list.
     *
     * @param element The element of type Type to be added to the linked list.
     */
    void addFirst(const Type& element) {
        Node* new_node = new Node(element);

        if (head_ == nullptr) {
            head_ = new_node;
            tail_ = new_node;
        } else {
            new_node->next = head_;
            head_->prev = new_node;
            head_ = new_node;
        }

        ++size_;
    }


    /**
     * Inserts an element at the specified index in the linked list.
     * If the index is 0, the element is added to the beginning of the list.
     * If the index is equal to the current size, the element is appended to the end.
     * For other valid indices, the element is inserted at the desired position.
     *
     * @param idx The index where the element should be inserted.
     * @param element The element of type Type to be inserted into the list.
     * @throws std::out_of_range If the specified index is greater than the current size.
     */
    void insert(const unsigned int idx, const Type& element) {
        if (idx > size_)
            throw std::out_of_range("Index out of range");

        if (idx == 0) {
            addFirst(element);
        } else if (idx == size_) {
            addLast(element);
        } else {
            Node* current = head_;
            for (unsigned int i = 0; i < idx; ++i)
                current = current->next;

            Node* new_node = new Node(element);
            new_node->next = current;
            new_node->prev = current->prev;

            current->prev->next = new_node;
            current->prev = new_node;

            ++size_;
        }
    }


    /**
     * Removes the first element from the linked list. If the list is empty,
     * the method returns immediately without performing any operation.
     *
     * The memory allocated for the removed node is released, and the size
     * of the linked list is decremented. If the list becomes empty as a
     * result of this operation, both the head and tail pointers are set
     * to nullptr.
     */
    void removeFirst() {
        if (head_ == nullptr)
            return;

        Node* temp = head_;
        head_ = head_->next;

        if (head_ != nullptr)
            head_->prev = nullptr;
        else
            tail_ = nullptr;

        delete temp;
        --size_;
    }


    /**
     * Removes the last node from the linked list.
     * If the list is empty, the method returns without performing any action.
     * If the list has only one node, the head and tail pointers are reset to nullptr.
     * Decrements the size of the linked list by 1.
     */
    void removeLast() {
        if (tail_ == nullptr)
            return;

        Node* temp = tail_;
        tail_ = tail_->prev;

        if (tail_ != nullptr)
            tail_->next = nullptr;
        else
            head_ = nullptr;

        delete temp;
        --size_;
    }


    /**
     * Removes the element at the specified index in the linked list.
     * If the index is 0, the first element is removed.
     * If the index is equal to the size of the list minus 1, the last element is removed.
     * For other valid indices, the element at the specified position is removed.
     *
     * @param idx The index of the element to remove.
     * @throws std::out_of_range If the specified index is out of range (greater than or equal to the size of the list).
     */
    void removeAt(const unsigned int idx) {
        if (idx >= size_)
            throw std::out_of_range("Index out of range");

        if (idx == 0) {
            removeFirst();
        } else if (idx == size_ - 1) {
            removeLast();

        } else if (idx < size_ / 2) {
            Node* current = head_;
            for (unsigned int i = 0; i < idx; ++i)
                current = current->next;

            current->prev->next = current->next;
            current->next->prev = current->prev;
            delete current;
            --size_;

        } else if (idx >= size_ / 2) {
            Node* current = tail_;
            for (unsigned int i = size_ - 1; i > idx; --i)
                current = current->prev;

            current->prev->next = current->next;
            current->next->prev = current->prev;
            delete current;
            --size_;
        }
    }


    /**
     * Removes the first occurrence of the specified element from the linked list.
     * If the element is found, it is removed from the list, and the size of the
     * linked list is decremented. The method returns immediately after removing
     * the element. If the element is not found, no changes are made to the list.
     *
     * @param element The element of type Type to be removed from the linked list.
     */
    void remove(const Type& element) {
        Node* current = head_;

        while (current != nullptr) {
            if (current->data == element) {
                if (current == head_) {
                    removeFirst();
                } else if (current == tail_) {
                    removeLast();
                } else {
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                    delete current;
                    --size_;
                }
                return;
            }
            current = current->next;
        }
    }


    /**
     * Retrieves the data at the specified index.
     *
     * @param idx The zero-based index of the element to retrieve.
     * @return A reference to the data at the specified index.
     * @throws std::out_of_range If the provided index is outside the bounds of the list.
     */
    Type& get(const unsigned int idx) const {
        if (idx >= size_)
            throw std::out_of_range("Index out of range");

        Node* current;
        if (idx < size_ / 2) {
            current = head_;
            for (unsigned int i = 0; i < idx; ++i)
                current = current->next;
        } else {
            current = tail_;
            for (unsigned int i = size_ - 1; i > idx; --i)
                current = current->prev;
        }

        return current->data;
    }


    /**
     * Removes all elements from the linked list and releases their allocated memory.
     * After this operation, the linked list will be empty, with head and tail pointers
     * set to nullptr and the size set to zero.
     */
    void clear() {
        while (!isEmpty())
            removeFirst();
    }


    /**
     * Prints the elements of the linked list in forward order, starting from the
     * first node (head) to the last node (tail). The elements are separated by
     * a space and terminated with a newline character. If the list is empty,
     * no output is produced.
     */
    void printForward() const {
        Node* current = head_;
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }


    /**
     * Prints the elements of the linked list in reverse order, from the tail to the head.
     * Each element is output to the standard output stream, separated by spaces.
     * If the list is empty, no output is produced.
     */
    void printBackward() const {
        Node* current = tail_;
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->prev;
        }
        std::cout << std::endl;
    }


    /// Destructor
    ~LinkedList() { clear(); }

};


#endif //LINKEDLIST_HPP
