#ifndef QUEUE_HPP
#define QUEUE_HPP


#include <limits>
#include <stdexcept>
#include <type_traits>


#include "DynamicArray.hpp"


/**
 * @class Queue
 * @brief A FIFO (first-in, first-out) container built on a growable, contiguous
 buffer.
 *
 * This template implements a queue with amortized O(1) enqueue/dequeue while
 preserving
 * insertion order. Internally it views a `DynamicArray<Type>` as a ring buffer:
 elements
 * live in one contiguous allocation; the logical front is tracked by
 `front_idx_`, and
 * the physical index of the i-th logical element is `(front_idx_ + i) %
 capacity()`.
 *
 * @tparam Type Element type stored by the queue.

 * @par Performance
 * - `enqueue` / `emplaceBack`: amortized O(1); O(n) when growth occurs.
 * - `dequeue`: O(1) amortized; an occasional shrink is O(n).
 * - `front` / `back`: O(1).
 *
 * @par Exception safety
 * - Rebuild paths (grow/shrink) use allocate+move+commit and provide the strong
 *   guarantee (the queue is unchanged on failure).
 * - In-place overwrite during enqueue uses assignment; if it throws, the queue
 *   remains valid (basic guarantee).
 * - Accessors (`front`, `back`, `dequeue`) throw `std::out_of_range` on empty.
 *
 * @par Invalidation
 * - Any reallocation (growth or shrink) invalidates all references, pointers,
 and
 *   iterators. In-place overwrite does not invalidate references to that slot,
 *   but the value changes.
 *
 * @par Type requirements
 * - `Type` must be MoveConstructible or CopyConstructible (for relocation).
 * - MoveAssignable or CopyAssignable enables the fast in-place overwrite path.
 *
 * @par Moved-from state
 * - A moved-from `Queue` is valid and empty (`size()==0`, `front_idx_==0`) and
 owns
 *   a fresh default-initialized `DynamicArray`.
 *
 * @par Thread-safety
 * - Not thread-safe; external synchronization is required for concurrent use.
 */
template <typename Type>
class Queue {

    DynamicArray<Type> array_;

    std::size_t front_idx_;
    std::size_t size_;
    std::size_t shrink_check_counter_ = 0;

    static constexpr std::size_t SHRINK_CHECK_INTERVAL = 16;
    static constexpr std::size_t MIN_SHRINK_CAPACITY = 10;
    static constexpr std::size_t SHRINK_THRESHOLD_DIVISOR = 4;
    static constexpr std::size_t GROWTH_FACTOR = 2;

    static constexpr std::size_t HARD_MAX_ELEMENTS =
        std::numeric_limits<std::size_t>::max() / sizeof(Type);


    /**
     * @brief Compute the physical index in the ring buffer for a logical
     * position.
     *
     * Translates a 0-based logical offset from the current front into an index
     * within the backing `DynamicArray`, wrapping via modulo by the current
     * capacity. This is the core mapping that turns the contiguous storage into
     * a circular queue.
     *
     * @param logical_index Offset from the front in [0, size()).
     * @return Index into the underlying storage in [0, capacity()).
     *
     * Complexity: O(1).
     *
     * Exception safety: No-throw.
     *
     * @note
     * Assumes `array_.capacity() > 0`, which is guaranteed by the underlying
     * `DynamicArray`. Does not perform bounds checks against `size()`; callers
     * must ensure `logical_index < size_`.
     */
    [[nodiscard]]
    std::size_t
    getCircularIndex(const std::size_t logical_index) const noexcept {
        return (front_idx_ + logical_index) % array_.capacity();
    }


    /**
     * @brief Compute the physical index where the next element would be
     * enqueued.
     *
     * Returns the ring-buffer slot immediately after the current back element,
     * computed as `(front_idx_ + size_) % array_.capacity()`. This is the
     * position used by `enqueue()`/`emplaceBack()` to place the next value when
     * there is remaining capacity.
     *
     * @return Index into the underlying storage in [0, capacity()) for the next
     * enqueue.
     *
     * Complexity: O(1).
     *
     * Exception safety: No-throw.
     *
     * @note
     * This is *not* the index of the current back element. That index (when
     * `size_ > 0`) is `(front_idx_ + size_ - 1) % array_.capacity()`. Assumes
     * `array_.capacity() > 0` (guaranteed by `DynamicArray`).
     */
    [[nodiscard]]
    std::size_t getBackIndex() const noexcept {
        return (front_idx_ + size_) % array_.capacity();
    }


    /**
     * @brief Periodically shrink the underlying buffer when the queue becomes
     * sparse.
     *
     * Increments an internal counter on each call (typically from `dequeue()`),
     * and every `SHRINK_CHECK_INTERVAL` calls evaluates whether the queue is
     * sufficiently under-utilized to justify a shrink. If `size_ <=
     * capacity()/SHRINK_THRESHOLD_DIVISOR` and `capacity() >
     * MIN_SHRINK_CAPACITY`, it builds a smaller buffer, moves elements in
     * logical order, and commits the new storage, resetting `front_idx_` to 0.
     *
     * @par Complexity
     * Amortized O(1) per dequeue; when a shrink is triggered, the operation
     * performs O(size()) move constructions plus one allocation.
     *
     * @par Exception Safety
     * Strong guarantee. The shrink is executed using an allocate+move+commit
     * pattern: if allocation or any element move construction throws, all
     * partially constructed objects in the new buffer are destroyed, the new
     * buffer is discarded, and the original queue remains unchanged.
     *
     * @par Effects
     * - Reduces capacity to `max(size_, capacity()/GROWTH_FACTOR)`, but never
     * below `MIN_SHRINK_CAPACITY`.
     * - Preserves FIFO order of elements.
     * - Resets `front_idx_` to 0 on successful shrink.
     * - Resets the periodic counter after each check (regardless of whether a
     * shrink occurred).
     *
     * @par Rationale
     * - The periodic check avoids paying the cost of a shrink heuristic on
     * every dequeue.
     * - The divisor threshold prevents oscillation (shrink/expand thrashing)
     * under bursty loads.
     * - Targeting at least `size_` ensures no immediate reallocation is needed
     * after shrinking.
     */
    void autoManageCapacity() {
        shrink_check_counter_++;
        if (shrink_check_counter_ >= SHRINK_CHECK_INTERVAL) {
            shrink_check_counter_ = 0;

            if (size_ <= array_.capacity() / SHRINK_THRESHOLD_DIVISOR &&
                array_.capacity() > MIN_SHRINK_CAPACITY) {

                DynamicArray<Type> new_array;
                const std::size_t halved = array_.capacity() / GROWTH_FACTOR;
                const std::size_t target = size_ > halved ? size_ : halved;

                const std::size_t clamped =
                    target < MIN_SHRINK_CAPACITY ? MIN_SHRINK_CAPACITY : target;

                new_array.reserve(clamped);

                for (std::size_t i = 0; i < size_; ++i) {
                    auto& src = array_[getCircularIndex(i)];
                    if constexpr (std::is_nothrow_move_constructible_v<Type> ||
                                  !std::is_copy_constructible_v<Type>)
                        new_array.emplaceLast(std::move(src));
                    else
                        new_array.emplaceLast(src);
                }

                array_ = std::move(new_array);
                front_idx_ = 0;
            }
        }
    }


    /**
     * @brief Grow the buffer and append a new element in one
     * strongly-exception-safe step.
     *
     * Computes a larger capacity using `new_cap = min(capacity()*GROWTH_FACTOR,
     * HARD_MAX_ELEMENTS)`, allocates a fresh `DynamicArray<Type>` with that
     * capacity, moves current elements into the new buffer in logical (FIFO)
     * order, then constructs the new element at the back. On success, commits
     * the new storage (`array_ = std::move(new_array)`), resets `front_idx_` to
     * 0, and increments `size_`.
     *
     * @tparam Args Argument types forwarded to `Type`'s constructor for the
     * appended element.
     * @param args  Constructor arguments forwarded to `Type` for the new
     * element.
     *
     * @par Complexity
     * - O(size()) move constructions plus one allocation; O(1) index
     * arithmetic.
     *
     * @par Exception Safety
     * - Strong guarantee. Uses allocate+move+commit: if allocation or any
     * element construction (including the new element) throws, all partially
     * constructed objects in the new buffer are destroyed, the temporary buffer
     * is discarded, and the original queue remains unchanged.
     *
     * @par Effects
     * - Capacity increases to `new_cap` (subject to `DynamicArray`'s internal
     * maximum).
     * - All existing elements are preserved and appear at indices `[0, size_)`
     * in the new buffer.
     * - The new element is appended at logical back; `front_idx_` becomes 0;
     * `size_` increments by 1.
     *
     * @par Notes
     * - Although `HARD_MAX_ELEMENTS` guards against size_t overflow, the
     * underlying `DynamicArray` may still throw `std::bad_alloc` if `new_cap`
     * exceeds its element-based `MAX_CAPACITY`.
     * - Preserves element order and stability (relative order of existing
     * elements).
     */
    template <typename... Args>
    void reallocateAndPush(Args&&... args) {
        const std::size_t cap = array_.capacity();

        if (cap >= HARD_MAX_ELEMENTS)
            throw std::length_error("Queue capacity exceeded");

        const std::size_t new_cap = (cap > HARD_MAX_ELEMENTS / GROWTH_FACTOR)
                                        ? HARD_MAX_ELEMENTS
                                        : cap * GROWTH_FACTOR;

        DynamicArray<Type> new_array;
        new_array.reserve(new_cap);

        for (std::size_t i = 0; i < size_; ++i) {
            auto& src = array_[getCircularIndex(i)];
            if constexpr (std::is_nothrow_move_constructible_v<Type> ||
                          !std::is_copy_constructible_v<Type>)
                new_array.emplaceLast(std::move(src));
            else
                new_array.emplaceLast(src);
        }

        new_array.emplaceLast(std::forward<Args>(args)...);
        array_ = std::move(new_array);
        front_idx_ = 0;
        ++size_;
    }


    /**
     * @brief Push a new element at the logical back, reusing storage when
     * possible.
     *
     * Chooses the most efficient path to append:
     *  - If the queue is full (`size_ == array_.capacity()`), grow and append
     * via `reallocateAndPush(args...)` (allocate + move + commit).
     *  - Else, compute the physical back slot `back_idx = getBackIndex()`.
     *     - If `back_idx >= array_.size()`, the slot lies in the unconstructed
     * tail, so construct in place via `array_.emplaceLast(args...)`.
     *     - Otherwise, the slot already holds a (logically unused) live object;
     * for assignable `Type`, construct a temporary from `args...` and assign
     *       `array_[back_idx] = std::move(tmp)` to overwrite it without
     * reallocation. For non-assignable `Type`, fall back to
     * `reallocateAndPush(args...)`.
     *
     * @tparam Args Argument types forwarded to `Type`'s constructor.
     * @param args  Constructor arguments forwarded to create/assign the new
     * value.
     *
     * @par Complexity
     * - Amortized O(1).
     * - O(n) only when a growth reallocation occurs (moves `size_` elements).
     * - O(1) when appending into the unconstructed tail or overwriting an
     * existing slot.
     *
     * @par Exception Safety
     * - Growth path (`reallocateAndPush`): strong guarantee via
     * allocate+move+commit (the queue is unchanged on failure).
     * - Tail construction path (`emplaceLast`): strong guarantee (if
     * construction throws, neither `array_` nor `size_` changes).
     * - Overwrite path (assignable `Type`): strong guarantee for the queue. A
     * temporary is constructed first; if that throws, nothing changes. The
     * assignment occurs before incrementing `size_`; if assignment throws, the
     * queue’s logical state remains unchanged (the overwritten slot was not
     * part of the logical queue).
     *
     * @par Invalidation
     * - Only the growth path invalidates references/iterators. The
     * overwrite/tail paths keep the underlying buffer and indices stable.
     */
    template <typename... Args>
    void pushBack(Args&&... args) {
        if (size_ == array_.capacity()) {
            reallocateAndPush(std::forward<Args>(args)...);
            return;
        }

        const std::size_t back_idx = getBackIndex();
        if (back_idx >= array_.size()) {
            array_.emplaceLast(std::forward<Args>(args)...);
            ++size_;
            return;
        }

        if constexpr (std::is_move_assignable_v<Type> ||
                      std::is_copy_assignable_v<Type>) {
            Type tmp(std::forward<Args>(args)...);
            array_[back_idx] = std::move(tmp);
            ++size_;
        } else {
            reallocateAndPush(std::forward<Args>(args)...);
        }
    }


  public:
    /// Default constructor
    Queue() : array_(), front_idx_(0), size_(0) {}

    /// Constructor with initial capacity
    explicit Queue(std::size_t initial_capacity)
        : array_(), front_idx_(0), size_(0) {
        array_.reserve(initial_capacity);
    }

    /**
     * Constructor with initial data and size.
     *
     * This constructor initializes the queue with a given array of initial
     * data and its size. It reserves enough space in the underlying dynamic
     * array to hold the initial elements.
     *
     * @param initial_data Pointer to the initial data array.
     * @param initial_size The number of elements in the initial data array.
     */
    Queue(const Type* initial_data, const std::size_t initial_size)
        : array_(initial_data, initial_size), front_idx_(0),
          size_(initial_size) {}

    /// Copy constructor
    Queue(const Queue& other) : array_(), front_idx_(0), size_(0) {
        array_.reserve(other.size_);
        for (std::size_t i = 0; i < other.size_; ++i) {
            std::size_t src_idx = other.getCircularIndex(i);
            array_.addLast(other.array_[src_idx]);
        }
        size_ = other.size_;
    }

    /// Move constructor
    Queue(Queue&& other) noexcept
        : array_(std::move(other.array_)), front_idx_(other.front_idx_),
          size_(other.size_) {
        other.array_ = DynamicArray<Type>{};
        other.front_idx_ = 0;
        other.size_ = 0;
    }

    /// Copy assignment operator
    Queue& operator=(const Queue& other) {
        if (this == &other)
            return *this;

        DynamicArray<Type> new_array;
        new_array.reserve(other.size_);

        for (std::size_t i = 0; i < other.size_; ++i) {
            const std::size_t src_idx = other.getCircularIndex(i);
            new_array.addLast(other.array_[src_idx]);
        }

        array_ = std::move(new_array);
        front_idx_ = 0;
        size_ = other.size_;
        return *this;
    }

    /// Move assignment operator
    Queue& operator=(Queue&& other) noexcept {
        if (this == &other)
            return *this;

        array_ = std::move(other.array_);
        front_idx_ = other.front_idx_;
        size_ = other.size_;

        other.array_ = DynamicArray<Type>{};
        other.front_idx_ = 0;
        other.size_ = 0;

        return *this;
    }


    /// Returns the number of elements in the queue.
    [[nodiscard]]
    std::size_t size() const noexcept {
        return size_;
    }

    /// Returns the capacity of the underlying data storage.
    [[nodiscard]]
    std::size_t capacity() const noexcept {
        return array_.capacity();
    }

    /// Checks if the queue is empty.
    [[nodiscard]]
    bool isEmpty() const noexcept {
        return size_ == 0;
    }


    /**
     * @brief Enqueue (append) a new element at the logical back of the queue.
     *
     * Perfect-forwards the argument into the queue. If there is spare capacity,
     * the element is placed at the ring-buffer slot returned by
     * `getBackIndex()`. Depending on `Type`:
     *  - If the slot lies in the unconstructed tail of the underlying
     * `DynamicArray`, the element is constructed in-place.
     *  - If the slot already holds a live (but logically unused) object and
     * `Type` is assignable, a temporary is constructed from `element` and
     * assigned into that slot (no reallocation).
     *  - Otherwise, the queue grows its capacity (by `GROWTH_FACTOR`) and
     * appends into a fresh buffer in logical order (allocate + move + commit).
     *
     * @tparam U  A type that can construct `Type` (via perfect forwarding).
     * @param element  The value to enqueue.
     *
     * @par Complexity
     * - Amortized O(1). O(n) only when a growth reallocation occurs (moves
     * `size()` elements).
     *
     * @par Exception Safety
     * - Growth path: **strong guarantee** via allocate+move+commit (the queue
     * is unchanged on failure).
     * - Tail construction path: strong (if construction throws, neither storage
     * nor `size_` changes).
     * - Overwrite path (assignable `Type`): strong for the queue; a temporary
     * is constructed first, so if construction or assignment throws, the
     * logical state does not change.
     * - May throw `std::bad_alloc` during growth.
     *
     * @par Invalidation
     * - Only the growth path invalidates references/pointers/iterators;
     * in-place paths do not.
     */
    template <typename U>
    void enqueue(U&& element) {
        static_assert(std::is_constructible_v<Type, U&&>,
                      "Element must be constructible into Type");
        pushBack(std::forward<U>(element));
    }


    /**
     * @brief Dequeue (remove) the front element and return it by value.
     *
     * Moves the current front element out of the queue and advances
     * `front_idx_` modulo the current capacity, then decrements `size_`. The
     * removed slot’s object is left in a valid moved-from state (its destructor
     * will run later when the buffer is rebuilt/shrunk, overwritten by
     * assignment, or the queue is cleared/destroyed). Periodically, a shrink
     * check may run and rebuild the buffer into a smaller capacity.
     *
     * @return The removed front element (moved).
     * @throws std::out_of_range if the queue is empty.
     *
     * @par Complexity
     * - O(1) for the dequeue itself; an occasional shrink is O(n).
     *
     * @par Exception Safety
     * - Moving the front element into the return value occurs first. If that
     * move construction throws, no state is changed (strong for the removal
     * step).
     * - The subsequent periodic shrink (if attempted) uses allocate+move+commit
     * (strong for the shrink itself). If it throws (e.g., `std::bad_alloc` or
     * element move/copy throws), the element has already been removed; the
     * exception is propagated afterward.
     *
     * @par Postconditions
     * - `size()` decreased by 1; `front_idx_` advanced modulo `capacity()`.
     * - FIFO order of remaining elements is preserved.
     *
     * @par Notes
     * - Deferring destruction of the popped slot enables fast in-place
     * overwrites on later enqueues for assignable `Type`. For resource-heavy
     * types, memory may be released earlier if a shrink or rebuild happens soon
     * after.
     */
    Type dequeue() {
        if (isEmpty())
            throw std::out_of_range("Queue is empty");

        Type element = std::move(array_[front_idx_]);
        front_idx_ = (front_idx_ + 1) % array_.capacity();
        --size_;
        autoManageCapacity();

        return element;
    }


    /**
     * @brief Returns the front element of the queue without removing it.
     *
     * @complexity O(1) for accessing the front element.
     *
     * @return A reference to the front element of the queue.
     * @throws std::out_of_range if the queue is empty.
     */
    Type& front() {
        if (isEmpty())
            throw std::out_of_range("Queue is empty");

        return array_[front_idx_];
    }


    /**
     * @brief Returns the front element of the queue without removing it.
     *
     * @complexity O(1) for accessing the front element.
     *
     * @return A const reference to the front element of the queue.
     * @throws std::out_of_range if the queue is empty.

     */
    const Type& front() const {
        if (isEmpty())
            throw std::out_of_range("Queue is empty");

        return array_[front_idx_];
    }


    /**
     * @brief Returns the back element of the queue without removing it.
     *
     * @complexity O(1) for accessing the back element.
     *
     * @return A reference to the back element of the queue.
     * @throws std::out_of_range if the queue is empty.

     */
    Type& back() {
        if (isEmpty())
            throw std::out_of_range("Queue is empty");

        std::size_t back_idx = (front_idx_ + size_ - 1) % array_.capacity();
        return array_[back_idx];
    }


    /**
     * @brief Returns the back element of the queue without removing it.
     *
     * @complexity O(1) for accessing the back element.
     *
     * @return A const reference to the back element of the queue.
     * @throws std::out_of_range if the queue is empty.
     */
    const Type& back() const {
        if (isEmpty())
            throw std::out_of_range("Queue is empty");

        std::size_t back_idx = (front_idx_ + size_ - 1) % array_.capacity();
        return array_[back_idx];
    }


    /**
     * @brief Enqueue by constructing the element in place at the logical back.
     *
     * Perfect-forwards `args...` to `Type`'s constructor. Behavior mirrors
     * `enqueue`: constructs in the uninitialized tail when available,
     * overwrites an existing back slot via assignment for assignable `Type`, or
     * grows the buffer and appends into a fresh allocation (allocate + move +
     * commit).
     *
     * @tparam Args  Argument types forwarded to `Type`'s constructor.
     * @param args   Constructor arguments for the new element.
     *
     * @par Complexity
     * - Amortized O(1); O(n) only when a growth reallocation occurs.
     *
     * @par Exception Safety
     * - Growth path: **strong guarantee** (the queue is unchanged on failure).
     * - Tail construction path: strong (if construction throws, nothing
     * changes).
     * - Overwrite path (assignable `Type`): strong for the queue; a temporary
     * is constructed first, so if construction/assignment throws, the logical
     * state is unchanged.
     * - May throw `std::bad_alloc` during growth.
     *
     * @par Invalidation
     * - Only the growth path invalidates references/pointers/iterators.
     */
    template <typename... Args>
    void emplaceBack(Args&&... args) {
        pushBack(std::forward<Args>(args)...);
    }


    /// Clears the queue, removing all elements.
    void clear() {
        array_ = DynamicArray<Type>{};
        front_idx_ = 0;
        size_ = 0;
        shrink_check_counter_ = 0;
    }

    /// Destructor
    ~Queue() = default;


    /**
     * @class iterator
     *
     * An iterator class for traversing the elements of the queue.
     * It provides methods to access and manipulate the elements in the queue
     * using standard iterator operations such as incrementing, decrementing,
     * dereferencing, and comparing iterators.
     */
    class iterator {

        friend class const_iterator;

        Queue& queue_;
        std::size_t index_;

      public:
        /// Constructor for iterator
        explicit iterator(Queue& queue, const std::size_t index)
            : queue_(queue), index_(index) {}

        /// Dereference operator to access the element at the current index
        Type& operator*() {
            return queue_.array_[queue_.getCircularIndex(index_)];
        }

        /// Arrow operator to access the address of the element at the current
        /// index
        Type* operator->() {
            return &queue_.array_[queue_.getCircularIndex(index_)];
        }

        /// Pre-increment operator to move the iterator to the next element
        iterator& operator++() {
            if (index_ >= queue_.size_)
                throw std::out_of_range(
                    "Iterator cannot be incremented past the end");
            index_++;
            return *this;
        }

        /// Post-increment operator to move the iterator to the next element
        iterator operator++(int) {
            if (index_ >= queue_.size_)
                throw std::out_of_range(
                    "Iterator cannot be incremented past the end");
            iterator temp = *this;
            index_++;
            return temp;
        }

        /// Pre-decrement operator to move the iterator to the previous element
        iterator& operator--() {
            if (index_ == 0)
                throw std::out_of_range(
                    "Cannot decrement iterator past the start");
            index_--;
            return *this;
        }

        /// Post-decrement operator to move the iterator to the previous element
        iterator operator--(int) {
            if (index_ == 0)
                throw std::out_of_range(
                    "Cannot decrement iterator past the start");
            iterator temp = *this;
            index_--;
            return temp;
        }

        /// Equality operator for iterator comparison
        bool operator==(const iterator& other) const {
            return &queue_ == &other.queue_ && index_ == other.index_;
        }

        /// Inequality operator for iterator comparison
        bool operator!=(const iterator& other) const {
            return !(*this == other);
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
     * A constant iterator class for traversing the elements of the queue.
     * It provides methods to access the elements in the queue without allowing
     * modification of the elements. It supports standard iterator operations
     * such as incrementing, decrementing, dereferencing, and comparing
     * iterators.
     */
    class const_iterator {
        const Queue& queue_;
        std::size_t index_;

      public:
        /// Constructor for iterator
        explicit const_iterator(const Queue& queue, const std::size_t index)
            : queue_(queue), index_(index) {}

        /// Constructor for conversion from non-const iterator
        explicit const_iterator(const iterator& other)
            : queue_(other.queue_), index_(other.index_) {}

        /// Dereference operator to access the element at the current index
        const Type& operator*() const {
            return queue_.array_[queue_.getCircularIndex(index_)];
        }

        /// Arrow operator to access the address of the element at the current
        /// index
        const Type* operator->() const {
            return &queue_.array_[queue_.getCircularIndex(index_)];
        }

        /// Pre-increment operator to move the iterator to the next element
        const_iterator& operator++() {
            if (index_ >= queue_.size_)
                throw std::out_of_range(
                    "Iterator cannot be incremented past the end");
            index_++;
            return *this;
        }

        /// Post-increment operator to move the iterator to the next element
        const_iterator operator++(int) {
            if (index_ >= queue_.size_)
                throw std::out_of_range(
                    "Iterator cannot be incremented past the end");
            const_iterator temp = *this;
            index_++;
            return temp;
        }

        /// Pre-decrement operator to move the iterator to the previous element
        const_iterator& operator--() {
            if (index_ == 0)
                throw std::out_of_range(
                    "Cannot decrement iterator past the start");
            index_--;
            return *this;
        }

        /// Post-decrement operator to move the iterator to the previous element
        const_iterator operator--(int) {
            if (index_ == 0)
                throw std::out_of_range(
                    "Cannot decrement iterator past the start");
            const_iterator temp = *this;
            index_--;
            return temp;
        }

        /// Equality operator for iterator comparison
        bool operator==(const const_iterator& other) const {
            return &queue_ == &other.queue_ && index_ == other.index_;
        }

        /// Inequality operator for iterator comparison
        bool operator!=(const const_iterator& other) const {
            return !(*this == other);
        }

        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = const Type*;
        using reference = const Type&;
    };


    // --- Iterator support for range-based for loops ---

    iterator begin() { return iterator(*this, 0); }
    iterator end() { return iterator(*this, size_); }

    const_iterator begin() const { return const_iterator(*this, 0); }
    const_iterator end() const { return const_iterator(*this, size_); }

    // --- C++11 range-based for loop support ---

    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }


    using iterator = typename Queue::iterator;
    using const_iterator = typename Queue::const_iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }
    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(cend());
    }
    const_reverse_iterator crend() const {
        return const_reverse_iterator(cbegin());
    }
};

#endif // QUEUE_HPP
