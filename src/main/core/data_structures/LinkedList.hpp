#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP


#include <cassert>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <initializer_list>


namespace ds {


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
     * @brief Retrieve the node pointer at a given index.
     *
     * @param idx Zero-based index in [0, size()).
     * @return A pointer to the node at the specified index.
     * @throws std::out_of_range If idx >= size().
     *
     * @par Complexity
     * O(min(idx, size()-idx)) due to bidirectional traversal.
     *
     * @par Exception Safety
     * Strong: throws only before modifying any state.
     */
    Node* getNodeAt(const std::size_t idx) {
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


    /**
     * @brief Retrieve the node pointer at a given index (const overload).
     *
     * @param idx Zero-based index in [0, size()).
     * @return A const pointer to the node at the specified index.
     * @throws std::out_of_range If idx >= size().
     *
     * @par Complexity
     * O(min(idx, size()-idx)).
     *
     * @par Exception Safety
     * Strong: throws only before modifying any state.
     */
    const Node* getNodeAt(const std::size_t idx) const {
        if (idx >= size_)
            throw std::out_of_range("Index out of range");

        const Node* current;
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
    LinkedList() noexcept : head_(nullptr), tail_(nullptr), size_(0) {}

    /// Constructor for braced-init-lists
    LinkedList(std::initializer_list<Type> initial_data)
        : head_(nullptr), tail_(nullptr), size_(0) {
        try {
            for (const Type& element : initial_data)
                addLast(element);
        } catch (...) {
            clear();
            throw;
        }
    }

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

        Node* new_head = nullptr;
        Node* new_tail = nullptr;
        std::size_t new_size = 0;

        try {
            for (const Node* cur = other.head_; cur != nullptr;
                 cur = cur->next) {
                Node* n = new Node(cur->data);
                if (!new_head)
                    new_head = new_tail = n;
                else {
                    new_tail->next = n;
                    n->prev = new_tail;
                    new_tail = n;
                }
                ++new_size;
            }
        } catch (...) {
            for (Node* p = new_head; p != nullptr;) {
                Node* next = p->next;
                delete p;
                p = next;
            }
            throw;
        }

        clear();
        head_ = new_head;
        tail_ = new_tail;
        size_ = new_size;
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
        assert(((size_ == 0) && (head_ == nullptr) && (tail_ == nullptr)) ||
               ((size_ > 0) && (head_ != nullptr) && (tail_ != nullptr)));
        return size_ == 0;
    }


    /**
     * @brief Add a new element to the beginning of the list.
     *
     * If the list is empty, the new element becomes both head and tail.
     *
     * @tparam U A type that can construct `Type` (enforced at compile time).
     * @param element Value to insert at the front (perfect-forwarded).
     *
     * @par Complexity
     * O(1).
     *
     * @par Exception Safety
     * Strong: if allocation or element construction throws, the list is
     * unchanged.
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
     * @brief Add a new element to the end of the list.
     *
     * If the list is empty, the new element becomes both head and tail.
     *
     * @tparam U A type that can construct `Type` (enforced at compile time).
     * @param element Value to insert at the back (perfect-forwarded).
     *
     * @par Complexity
     * O(1).
     *
     * @par Exception Safety
     * Strong: if allocation or element construction throws, the list is
     * unchanged.
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
     * @brief Insert a new element at index `idx`.
     *
     * If `idx == 0`, inserts at the front; if `idx == size()`, inserts at the
     * back.
     *
     * @tparam U A type that can construct `Type` (enforced at compile time).
     * @param element Value to insert (perfect-forwarded).
     * @param idx Insertion position in [0, size()].
     * @throws std::out_of_range If idx > size().
     *
     * @par Complexity
     * O(min(idx, size()-idx)) to locate the position; link/unlink is O(1).
     *
     * @par Exception Safety
     * Strong: allocation/element construction may throw; on failure the list is
     * unchanged.
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
     * @brief Remove the first element if present.
     *
     * If the list is empty, no action is taken.
     *
     * @par Complexity
     * O(1).
     *
     * @par Exception Safety
     * No-throw.
     */
    void removeFirst() noexcept {
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
     * @brief Remove the last element if present.
     *
     * If the list is empty, no action is taken.
     *
     * @par Complexity
     * O(1).
     *
     * @par Exception Safety
     * No-throw.
     */
    void removeLast() noexcept {
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
     * @brief Remove the element at index `idx`.
     *
     * @param idx Zero-based index in [0, size()).
     * @throws std::out_of_range If idx >= size().
     *
     * @par Complexity
     * O(min(idx, size()-idx)) to locate the node; unlink is O(1).
     *
     * @par Exception Safety
     * Strong: throws only before any modification (on out_of_range).
     */
    void removeAt(const std::size_t idx) {
        if (idx >= size_)
            throw std::out_of_range("Index out of range");

        if (idx == 0) {
            removeFirst();
        } else if (idx == size_ - 1) {
            removeLast();

        } else {
            Node* cur = getNodeAt(idx);
            cur->prev->next = cur->next;
            cur->next->prev = cur->prev;
            delete cur;
            --size_;
        }
    }


    /**
     * @brief Remove the first occurrence of `element`, if any.
     *
     * @param element Value to remove (compared via `operator==`).
     *
     * @par Complexity
     * O(n) for linear search; unlink is O(1).
     *
     * @par Exception Safety
     * Basic-to-strong: if comparison (`operator==`) throws, no modification
     * occurs up to that point.
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
     * @brief Remove all occurrences of `element`.
     *
     * @param element Value to remove (compared via `operator==`).
     * @return Number of removed elements.
     *
     * @par Complexity
     * O(n) for a full traversal; each unlink is O(1).
     *
     * @par Exception Safety
     * Basic: if comparison (`operator==`) throws mid-traversal, some removals
     * may already have occurred; the list remains valid.
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
     * @brief Access the element at index `idx`.
     *
     * @param idx Zero-based index in [0, size()).
     * @return Reference to the element at `idx`.
     * @throws std::out_of_range If idx >= size().
     *
     * @par Complexity
     * O(min(idx, size()-idx)).
     */
    Type& get(const std::size_t idx) {
        if (idx >= size_)
            throw std::out_of_range("Index out of range");

        Node* current = getNodeAt(idx);
        return current->data;
    }


    /**
     * @brief Access the element at index `idx` (const overload).
     *
     * @param idx Zero-based index in [0, size()).
     * @return Const reference to the element at `idx`.
     * @throws std::out_of_range If idx >= size().
     *
     * @par Complexity
     * O(min(idx, size()-idx)).
     */
    const Type& get(const std::size_t idx) const {
        if (idx >= size_)
            throw std::out_of_range("Index out of range");

        const Node* current = getNodeAt(idx);
        return current->data;
    }


    /**
     * @brief Element access with bounds checking.
     *
     * @param idx Zero-based index in [0, size()).
     * @return Reference to the element at `idx`.
     * @throws std::out_of_range If idx >= size().
     *
     * @par Complexity
     * O(min(idx, size()-idx)).
     *
     * @note Unlike `std::vector::operator[]`, this overload performs a bounds
     * check and may throw.
     */
    Type& operator[](const std::size_t idx) { return get(idx); }


    /**
     * @brief Element access with bounds checking (const overload).
     *
     * @param idx Zero-based index in [0, size()).
     * @return Const reference to the element at `idx`.
     * @throws std::out_of_range If idx >= size().
     *
     * @par Complexity
     * O(min(idx, size()-idx)).
     *
     * @note Unlike `std::vector::operator[]`, this overload performs a bounds
     * check and may throw.
     */
    const Type& operator[](const std::size_t idx) const { return get(idx); }


    /**
     * @brief Access the first element.
     * @return Reference to the first element.
     * @throws std::out_of_range If the list is empty.
     *
     * @par Complexity
     * O(1).
     */
    Type& front() {
        if (isEmpty())
            throw std::out_of_range("LinkedList is empty");
        return head_->data;
    }


    /**
     * @brief Access the first element (const overload).
     * @return Const reference to the first element.
     * @throws std::out_of_range If the list is empty.
     *
     * @par Complexity
     * O(1).
     */
    const Type& front() const {
        if (isEmpty())
            throw std::out_of_range("LinkedList is empty");
        return head_->data;
    }


    /**
     * @brief Access the last element.
     * @return Reference to the last element.
     * @throws std::out_of_range If the list is empty.
     *
     * @par Complexity
     * O(1).
     */
    Type& back() {
        if (isEmpty())
            throw std::out_of_range("LinkedList is empty");
        return tail_->data;
    }


    /**
     * @brief Access the last element (const overload).
     * @return Const reference to the last element.
     * @throws std::out_of_range If the list is empty.
     *
     * @par Complexity
     * O(1).
     */
    const Type& back() const {
        if (isEmpty())
            throw std::out_of_range("LinkedList is empty");
        return tail_->data;
    }


    /**
     * @brief Remove all elements and reset the list to empty.
     *
     * @par Complexity
     * O(n) — deletes each node.
     *
     * @par Exception Safety
     * No-throw.
     */
    void clear() noexcept {
        Node* current = head_;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head_ = tail_ = nullptr;
        size_ = 0;
    }


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

        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = Type*;
        using reference = Type&;
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

        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = const Type*;
        using reference = const Type&;
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


    /**
     * @brief Erase the node at `pos` and return an iterator to the next node.
     *
     * If `pos` is end(), returns end().
     *
     * @param pos Iterator pointing to the node to erase.
     * @return Iterator to the node following the erased one (or end()).
     *
     * @par Complexity
     * O(1).
     *
     * @par Exception Safety
     * No-throw (pointer relinks and delete).
     *
     * @par Invalidation
     * Invalidates only the erased iterator; all other iterators remain valid.
     */
    iterator erase(iterator pos) {
        Node* cur = pos.current_;
        if (!cur)
            return iterator(nullptr);

        Node* next = cur->next;

        if (cur == head_) {
            head_ = cur->next;
            if (head_)
                head_->prev = nullptr;
            else
                tail_ = nullptr;
        } else if (cur == tail_) {
            tail_ = cur->prev;
            if (tail_)
                tail_->next = nullptr;
            else
                head_ = nullptr;
        } else {
            cur->prev->next = cur->next;
            cur->next->prev = cur->prev;
        }

        delete cur;
        --size_;
        return iterator(next);
    }


    /// Destructor
    ~LinkedList() noexcept { clear(); }
};

} // namespace data_structs

#endif // LINKEDLIST_HPP
