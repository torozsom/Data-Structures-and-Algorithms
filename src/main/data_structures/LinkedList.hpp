#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP


#include <cassert>
#include <iostream>
#include <type_traits>
#include <utility>


/**
 * @class LinkedList
 *
 * A generic doubly linked list class that provides functionalities for adding,
 * removing, and accessing elements. The internal storage is implemented using
 * a doubly linked list structure.
 */
template <typename Type>
class LinkedList {

    struct Node {
        Type data;
        Node* next;
        Node* prev;

        template <typename U>
        explicit Node(U&& data)
            : data(std::forward<U>(data)), next(nullptr), prev(nullptr) {
            static_assert(std::is_constructible_v<Type, U&&>,
                          "Only types constructible into Type are allowed");
        }
    };

    Node* head_;
    Node* tail_;
    std::size_t size_;


    /**
     * Retrieves the node at the specified index.
     *
     * @param idx The zero-based index of the node to retrieve.
     * @return A pointer to the node at the specified index.
     * @throws std::out_of_range If the provided index is outside the bounds of
     * the list.
     */
    Node* getNodeAt(const std::size_t idx) const {
        if (idx >= size_)
            throw std::out_of_range("Index out of range");

        Node* current;
        if (idx < size_ / 2) {
            current = head_;
            for (std::size_t i = 0; i < idx; ++i)
                current = current->next;
        } else {
            current = tail_;
            for (std::size_t i = size_ - 1; i > idx; --i)
                current = current->prev;
        }

        return current;
    }


  public:
    /// Default constructor
    LinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

    /**
     * Constructor that initializes the linked list with an array of elements.
     * The size of the linked list will be set to the number of elements in the
     * provided array.
     *
     * @param array Pointer to the array of elements to initialize the linked
     * list.
     * @param size The number of elements in the array.
     *
     * @throws std::invalid_argument If the provided array is null and size is
     * greater than zero.
     */
    LinkedList(const Type* array, const std::size_t size)
        : head_(nullptr), tail_(nullptr), size_(0) {

        if (size > 0 && array == nullptr)
            throw std::invalid_argument(
                "Initial data cannot be null if size is positive.");

        try {
            for (std::size_t i = 0; i < size; ++i)
                addLast(array[i]);
        } catch (...) {
            clear();
            throw;
        }
    }

    /// Copy constructor
    LinkedList(const LinkedList& other)
        : head_(nullptr), tail_(nullptr), size_(0) {
        Node* current = other.head_;

        try {
            while (current != nullptr) {
                addLast(current->data);
                current = current->next;
            }
        } catch (...) {
            clear();
            throw;
        }
    }

    /// Move constructor
    LinkedList(LinkedList&& other) noexcept
        : head_(other.head_), tail_(other.tail_), size_(other.size_) {
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }

    /// Copy assignment operator
    LinkedList& operator=(const LinkedList& other) {
        if (this == &other)
            return *this;

        clear();
        Node* current = other.head_;

        try {
            while (current != nullptr) {
                addLast(current->data);
                current = current->next;
            }
        } catch (...) {
            clear();
            throw;
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


    /// Returns the current size of the linked list.
    [[nodiscard]]
    std::size_t size() const noexcept {
        return size_;
    }

    /// Checks if the linked list is empty.
    [[nodiscard]]
    bool isEmpty() const noexcept {
        assert(size_ == 0 == (head_ == nullptr));
        return size_ == 0;
    }


    /**
     * Adds a new element to the beginning of the linked list.
     * If the list is empty, the new element becomes both the head and tail.
     *
     * @tparam U The type of the element to be added. Must be the same as Type
     * or convertible to it.
     * @param element The element to be added at the beginning of the linked
     * list.
     *
     * @throws std::bad_alloc If memory allocation fails.
     * @throws std::invalid_argument If the element is not constructible into
     * Type.
     */
    template <typename U>
    void addFirst(U&& element) {
        static_assert(std::is_constructible_v<Type, U&&>,
                      "Element must be constructible into Type");
        Node* new_node = new Node(std::forward<U>(element));

        if (isEmpty()) {
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
     * Adds a new element to the end of the linked list.
     * If the list is empty, the new element becomes both the head and tail.
     *
     * @tparam U The type of the element to be added. Must be the same as Type
     * or convertible to it.
     * @param element The element to be added at the end of the linked list.
     *
     * @throws std::bad_alloc If memory allocation fails.
     * @throws std::invalid_argument If the element is not constructible into
     * Type.
     */
    template <typename U>
    void addLast(U&& element) {
        static_assert(std::is_constructible_v<Type, U&&>,
                      "Element must be constructible into Type");
        Node* new_node = new Node(std::forward<U>(element));

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
     * Inserts a new element at the specified index in the linked list.
     * If the index is 0, the element is added at the beginning.
     * If the index is equal to the size of the list, the element is added at
     * the end.
     * For other valid indices, the element is inserted at the specified
     * position.
     *
     * @tparam U The type of the element to be added. Must be the same as Type
     * or convertible to it.
     * @param element The element to be inserted into the linked list.
     * @param idx The index at which to insert the new element.
     *
     * @throws std::out_of_range If the specified index is out of range (greater
     * than size).
     */
    template <typename U>
    void insert(U&& element, const std::size_t idx) {
        static_assert(std::is_constructible_v<Type, U&&>,
                      "Element must be constructible into Type");

        if (idx > size_)
            throw std::out_of_range("Index out of range");

        Node* new_node = nullptr;

        try {
            if (idx == 0) {
                addFirst(std::forward<U>(element));
            } else if (idx == size_) {
                addLast(std::forward<U>(element));
            } else {
                Node* current = getNodeAt(idx);

                new_node = new Node(std::forward<U>(element));
                new_node->next = current;
                new_node->prev = current->prev;

                current->prev->next = new_node;
                current->prev = new_node;
                ++size_;
            }
        } catch (...) {
            delete new_node;
            throw;
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

        const Node* temp = head_;
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
     * If the list has only one node, the head and tail pointers are reset to
     * nullptr. Decrements the size of the linked list by 1.
     */
    void removeLast() {
        if (tail_ == nullptr)
            return;

        const Node* temp = tail_;
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
     * If the index is equal to the size of the list minus 1, the last element
     * is removed. For other valid indices, the element at the specified
     * position is removed.
     *
     * @param idx The index of the element to remove.
     * @throws std::out_of_range If the specified index is out of range (greater
     * than or equal to the size of the list).
     */
    void removeAt(const std::size_t idx) {
        if (idx >= size_)
            throw std::out_of_range("Index out of range");

        if (idx == 0) {
            removeFirst();
        } else if (idx == size_ - 1) {
            removeLast();

        } else if (idx < size_ / 2) {
            Node* current = head_;
            for (std::size_t i = 0; i < idx; ++i)
                current = current->next;

            current->prev->next = current->next;
            current->next->prev = current->prev;
            delete current;
            --size_;

        } else if (idx >= size_ / 2) {
            Node* current = tail_;
            for (std::size_t i = size_ - 1; i > idx; --i)
                current = current->prev;

            current->prev->next = current->next;
            current->next->prev = current->prev;
            delete current;
            --size_;
        }
    }


    /**
     * Removes the first occurrence of the specified element from the linked
     * list. If the element is found, it is removed and the size of the list is
     * decremented. If the element is not found, no action is taken.
     *
     * @param element The element of type Type to be removed from the linked
     * list.
     */
    void remove(const Type& element) {
        Node* current = head_;

        while (current != nullptr) {
            if (current->data == element) {
                const Node* to_delete = current;

                if (current == head_) {
                    head_ = current->next;
                    if (head_)
                        head_->prev = nullptr;
                    else
                        tail_ = nullptr; // list is now empty
                } else if (current == tail_) {
                    tail_ = current->prev;
                    if (tail_)
                        tail_->next = nullptr;
                    else
                        head_ = nullptr; // list is now empty
                } else {
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                }

                delete to_delete;
                --size_;
                return;
            }

            current = current->next;
        }
    }


    /**
     * Removes all occurrences of the specified element from the linked list.
     * If the element is found, it is removed and the size of the list is
     * decremented. The method returns the number of elements removed.
     *
     * @param element The element of type Type to be removed from the linked
     * list.
     * @return The number of elements removed from the linked list.
     */
    std::size_t removeAll(const Type& element) {
        if (isEmpty())
            return 0;

        Node* current = head_;
        std::size_t removed_count = 0;

        while (current != nullptr) {
            Node* next_node = current->next;
            if (current->data == element) {
                if (current == head_) {
                    head_ = current->next;
                    if (head_)
                        head_->prev = nullptr;
                    else
                        tail_ = nullptr;
                } else if (current == tail_) {
                    tail_ = current->prev;
                    if (tail_)
                        tail_->next = nullptr;
                    else
                        head_ = nullptr;
                } else {
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                }

                delete current;
                --size_;
                ++removed_count;
            }
            current = next_node;
        }

        return removed_count;
    }


    /**
     * Retrieves the data at the specified index.
     *
     * @param idx The zero-based index of the element to retrieve.
     * @return A reference to the data at the specified index.
     * @throws std::out_of_range If the provided index is outside the bounds of
     * the list.
     */
    Type& get(const std::size_t idx) {
        if (idx >= size_)
            throw std::out_of_range("Index out of range");

        Node* current = getNodeAt(idx);
        return current->data;
    }


    /**
     * Retrieves the data at the specified index.
     *
     * @param idx The zero-based index of the element to retrieve.
     * @return A const reference to the data at the specified index.
     * @throws std::out_of_range If the provided index is outside the bounds of
     * the list.
     */
    const Type& get(const std::size_t idx) const {
        if (idx >= size_)
            throw std::out_of_range("Index out of range");

        Node* current = getNodeAt(idx);
        return current->data;
    }


    /**
     * Retrieves the data at the specified index.
     *
     * @param idx The zero-based index of the element to retrieve.
     * @return A reference to the data at the specified index.
     * @throws std::out_of_range If the provided index is outside the bounds of
     * the list.
     */
    Type& operator[](const std::size_t idx) { return get(idx); }


    /**
     * Retrieves the data at the specified index.
     *
     * @param idx The zero-based index of the element to retrieve.
     * @return A const reference to the data at the specified index.
     * @throws std::out_of_range If the provided index is outside the bounds of
     * the list.
     */
    const Type& operator[](const std::size_t idx) const { return get(idx); }


    /**
     * Clears the linked list by deleting all nodes and resetting the head,
     * tail, and size.
     *
     * This method deallocates all memory used by the linked list nodes and
     * sets the size to zero. After calling this method, the linked list will be
     * empty.
     */
    void clear() {
        Node* current = head_;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head_ = tail_ = nullptr;
        size_ = 0;
    }

    /// Destructor
    ~LinkedList() { clear(); }


    /**
     * @class iterator
     *
     * A bidirectional iterator for the LinkedList class.
     * This iterator allows traversal of the linked list in both directions
     * (forward and backward) and provides access to the data stored in the
     * nodes.
     */
    class iterator {

        friend class const_iterator;

        Node* current_;

      public:
        /// Default constructor
        iterator() : current_(nullptr) {}

        /// Constructor that initializes the iterator to a specific node
        explicit iterator(Node* node) : current_(node) {}


        /// Dereference operator to access the data of the current node
        Type& operator*() { return current_->data; }

        /// Arrow operator to access the address of the data in the current node
        Type* operator->() { return &current_->data; }

        /// Pre-increment operator to move the iterator to the next node
        iterator& operator++() {
            current_ = current_->next;
            return *this;
        }

        /// Post-increment operator to move the iterator to the next node
        iterator operator++(int) {
            iterator temp = *this;
            current_ = current_->next;
            return temp;
        }

        /// Pre-decrement operator to move the iterator to the previous node
        iterator& operator--() {
            current_ = current_->prev;
            return *this;
        }

        /// Post-decrement operator to move the iterator to the previous node
        iterator operator--(int) {
            iterator temp = *this;
            current_ = current_->prev;
            return temp;
        }

        /// Equality operator to compare two iterators
        bool operator==(const iterator& other) const {
            return current_ == other.current_;
        }

        /// Inequality operator to compare two iterators
        bool operator!=(const iterator& other) const {
            return current_ != other.current_;
        }
    };


    /**
     * @class const_iterator
     *
     * A constant bidirectional iterator for the LinkedList class.
     * This iterator allows traversal of the linked list in both directions
     * (forward and backward) and provides access to the data stored in the
     * nodes without allowing modification of the data.
     */
    class const_iterator {

        const Node* current_;

      public:
        /// Default constructor
        const_iterator() : current_(nullptr) {}

        /// Constructor that initializes the iterator to a specific node
        explicit const_iterator(const Node* node) : current_(node) {}

        /// Constructor that allows conversion from a non-const iterator
        explicit const_iterator(const iterator& iter)
            : current_(iter.current_) {}

        /// Dereference operator to access the data of the current node
        const Type& operator*() const { return current_->data; }

        /// Arrow operator to access the address of the data in the current node
        const Type* operator->() const { return &current_->data; }

        /// Pre-increment operator to move the iterator to the next node
        const_iterator& operator++() {
            current_ = current_->next;
            return *this;
        }

        /// Post-increment operator to move the iterator to the next node
        const_iterator operator++(int) {
            const_iterator temp = *this;
            current_ = current_->next;
            return temp;
        }

        /// Pre-decrement operator to move the iterator to the previous node
        const_iterator& operator--() {
            current_ = current_->prev;
            return *this;
        }

        /// Post-decrement operator to move the iterator to the previous node
        const_iterator operator--(int) {
            const_iterator temp = *this;
            current_ = current_->prev;
            return temp;
        }

        /// Equality operator to compare two iterators
        bool operator==(const const_iterator& other) const {
            return current_ == other.current_;
        }

        /// Inequality operator to compare two iterators
        bool operator!=(const const_iterator& other) const {
            return current_ != other.current_;
        }
    };


    // Regular iterators
    iterator begin() { return iterator(head_); }
    iterator end() { return iterator(nullptr); }

    // Const iterators
    const_iterator begin() const { return const_iterator(head_); }
    const_iterator end() const { return const_iterator(nullptr); }

    // Explicit const access
    const_iterator cbegin() const { return const_iterator(head_); }
    const_iterator cend() const { return const_iterator(nullptr); }


    /// Type aliases for the LinkedList class

    using value_type = Type;
    using size_type = std::size_t;
    using reference = Type&;
    using const_reference = const Type&;
    using pointer = Type*;
    using const_pointer = const Type*;
};


#endif // LINKEDLIST_HPP
