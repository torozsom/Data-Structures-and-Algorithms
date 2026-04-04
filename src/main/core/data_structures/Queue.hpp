#ifndef QUEUE_HPP
#define QUEUE_HPP


#include <limits>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <bit>


#include "DynamicArray.hpp"


namespace containers {

using std::size_t;


/**
 * @class Queue
 *
 * @brief A dynamic circular queue implementation using a `DynamicArray` as the
 * underlying storage.
 *
 * This `Queue` class provides efficient enqueue and dequeue operations with
 * amortized O(1) complexity. It maintains a circular buffer within the
 * `DynamicArray`, tracking the front index and size to manage the logical order
 * of elements. The queue automatically grows its capacity when needed and
 * periodically checks for under-utilization to shrink the buffer, optimizing
 * memory usage while maintaining performance.
 *
 * @tparam Type The type of elements stored in the queue.
 */
template <typename Type>
class Queue {

    DynamicArray<Type> array_;

    size_t front_idx_;
    size_t size_;
    size_t shrink_check_counter_ = 0;

    static constexpr size_t SHRINK_CHECK_INTERVAL = 16;
    static constexpr size_t MIN_SHRINK_CAPACITY = 16;
    static constexpr size_t SHRINK_THRESHOLD_DIVISOR = 4;
    static constexpr size_t GROWTH_FACTOR = 2;

    static constexpr size_t HARD_MAX_ELEMENTS =
        std::numeric_limits<size_t>::max() / sizeof(Type);


    /**
     * @brief Compute the physical index in the underlying array for a given
     * logical index.
     *
     * Given a logical index `i` (where `0 <= i < size_`), this function
     * calculates the corresponding physical index in `array_` by adding `i`
     * to `front_idx_` and applying modulo with the capacity. The modulo is
     * optimized to a bitwise AND since capacity is always a power of two.
     *
     * @param logical_index The logical index of the element (0-based).
     * @return The physical index in the underlying array where the element at
     *         `logical_index` resides.
     */
    [[nodiscard]]
    size_t getCircularIndex(const size_t logical_index) const noexcept {
        return (front_idx_ + logical_index) & (array_.capacity() - 1);
    }


    /**
     * @brief Compute the physical index for the logical back of the queue.
     *
     * Calculates the index where the next enqueued element would be placed if
     * there is spare capacity. This is effectively `getCircularIndex(size_)`
     * but optimized to avoid an extra modulo operation by using bitwise AND,
     * leveraging the invariant that capacity is always a power of two.
     *
     * @return Index into the underlying storage for the logical back element.
     */
    [[nodiscard]]
    size_t getBackIndex() const noexcept {
        return (front_idx_ + size_) & (array_.capacity() - 1);
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
     * performs O(size()) move constructions plus one allocation..
     */
    void autoManageCapacity() {
        shrink_check_counter_++;
        if (shrink_check_counter_ >= SHRINK_CHECK_INTERVAL) {
            shrink_check_counter_ = 0;

            if (size_ <= array_.capacity() / SHRINK_THRESHOLD_DIVISOR &&
                array_.capacity() > MIN_SHRINK_CAPACITY) {

                DynamicArray<Type> new_array;
                const size_t halved = array_.capacity() / GROWTH_FACTOR;
                const size_t target = size_ > halved ? size_ : halved;

                const size_t clamped = target < MIN_SHRINK_CAPACITY
                    ? MIN_SHRINK_CAPACITY
                    : target;

                new_array.reserve(std::bit_ceil(clamped));

                for (size_t i = 0; i < size_; ++i) {
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
     * O(size()) move constructions plus one allocation; O(1) index
     * arithmetic.
     */
    template <typename... Args>
    void reallocateAndPush(Args&&... args) {
        const size_t cap = array_.capacity();

        if (cap >= HARD_MAX_ELEMENTS)
            throw std::length_error("Queue capacity exceeded");

        const size_t new_cap = (cap > HARD_MAX_ELEMENTS / GROWTH_FACTOR)
                                   ? HARD_MAX_ELEMENTS
                                   : cap * GROWTH_FACTOR;

        DynamicArray<Type> new_array;
        new_array.reserve(new_cap);

        for (size_t i = 0; i < size_; ++i) {
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
     * Amortized O(1):
     * - O(n) only when a growth reallocation occurs (moves `size_` elements).
     * - O(1) when appending into the unconstructed tail or overwriting an
     * existing slot.
     */
    template <typename... Args>
    void pushBack(Args&&... args) {
        if (size_ == array_.capacity()) {
            reallocateAndPush(std::forward<Args>(args)...);
            return;
        }

        const size_t back_idx = getBackIndex();
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
    Queue() : front_idx_(0), size_(0) {
        array_.reserve(16);
    }


    /**
     * @brief Constructor with initial capacity.
     *
     * Initializes the queue with a specified initial capacity. The actual
     * reserved capacity will be the next power of two greater than or equal to
     * `initial_capacity`, with a minimum of 8 to avoid too small buffers.
     *
     * @param initial_capacity The desired initial capacity for the queue.
     */
    explicit Queue(const size_t initial_capacity)
        : front_idx_(0), size_(0) {

        size_t power_of_two_cap = std::bit_ceil(initial_capacity);
        if (power_of_two_cap < 8) power_of_two_cap = 8;

        array_.reserve(power_of_two_cap);
    }


    /**
     * @brief Constructor with initializer list.
     *
     * Initializes the queue with elements from an initializer list. The reserved
     * capacity will be the next power of two greater than or equal to the
     * number of initial elements, with a minimum of 16 to accommodate small
     * lists without immediate growth.
     *
     * @param initial_data An initializer list containing the initial elements for
     * the queue.
     */
    Queue(std::initializer_list<Type> initial_data)
        : front_idx_(0), size_(initial_data.size()) {

        size_t cap = std::bit_ceil(initial_data.size());
        if (cap < 16) cap = 16;
        array_.reserve(cap);

        for (const auto& item : initial_data)
            array_.addLast(item);
    }


    /**
     * @brief Constructor with raw array and size.
     *
     * Initializes the queue with elements from a raw array. The reserved capacity
     * will be the next power of two greater than or equal to `initial_size`, with
     * a minimum of 16 to accommodate small arrays without immediate growth.
     *
     * @param initial_data Pointer to the first element of the initial data array.
     * @param initial_size The number of elements in the initial data array.
     *
     * @throws std::invalid_argument if `initial_data` is null while `initial_size` is greater than 0.
     */
    Queue(const Type* initial_data, const size_t initial_size)
        : front_idx_(0), size_(initial_size) {

        if (initial_size > 0 && initial_data == nullptr)
            throw std::invalid_argument("Initial data cannot be null");

        size_t cap = std::bit_ceil(initial_size);
        if (cap < 16) cap = 16;
        array_.reserve(cap);

        for (size_t i = 0; i < initial_size; ++i) {
            array_.addLast(initial_data[i]);
        }
    }


    /// Copy constructor
    Queue(const Queue& other) : array_(), front_idx_(0), size_(0) {
        size_t cap = std::bit_ceil(other.size_);
        if (cap < 16) cap = 16;
        array_.reserve(cap);
        for (size_t i = 0; i < other.size_; ++i) {
            size_t src_idx = other.getCircularIndex(i);
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
        size_t cap = std::bit_ceil(other.size_);
        if (cap < 16) cap = 16;
        new_array.reserve(cap);

        for (size_t i = 0; i < other.size_; ++i) {
            const size_t src_idx = other.getCircularIndex(i);
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
    size_t size() const noexcept {
        return size_;
    }

    /// Returns the capacity of the underlying data storage.
    [[nodiscard]]
    size_t capacity() const noexcept {
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
     * Amortized O(1).
     * - O(n) only when a growth reallocation occurs (moves
     * `size()` elements).
     */
    template <typename U>
    void enqueue(U&& element) {
        static_assert(std::is_constructible_v<Type, U&&>,
                      "Element must be constructible into Type");
        pushBack(std::forward<U>(element));
    }


    /**
     * @brief Dequeue (remove) the front element.
     *
     * To inspect the value, use front() before calling dequeue().
     *
     * @par Complexity
     * O(1) for index arithmetic and size decrement;
     * O(1) for move assignment of the front element to a discard variable
     * to ensure proper destruction if `Type` has a non-trivial destructor.
     * Amortized O(1) when considering periodic shrinking, which occurs every
     * `SHRINK_CHECK_INTERVAL` dequeues and involves O(size()) moves plus one allocation.
     */
    void dequeue() {
        if (isEmpty())
            throw std::out_of_range("Queue is empty");

        {
            Type discard = std::move(array_[front_idx_]);
        }

        front_idx_ = front_idx_ + 1 & array_.capacity() - 1;
        --size_;
        autoManageCapacity();
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

        size_t back_idx = (front_idx_ + size_ - 1) & (array_.capacity() - 1);
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

        size_t back_idx = (front_idx_ + size_ - 1) & (array_.capacity() - 1);
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


    /// Clears the queue, removing all elements while keeping capacity.
    void clear() noexcept {
        array_.clear();
        front_idx_ = 0;
        size_ = 0;
        shrink_check_counter_ = 0;
    }

    /// Destructor
    ~Queue() = default;
};

} // namespace containers

#endif // QUEUE_HPP
