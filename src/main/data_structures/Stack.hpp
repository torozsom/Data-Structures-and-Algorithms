#ifndef STACK_HPP
#define STACK_HPP


#include <stdexcept>
#include <type_traits>
#include <utility>

#include "DynamicArray.hpp"


namespace data_structs {


/**
 * @class Stack
 * @brief A LIFO (last-in, first-out) container backed by a resizable contiguous
 * buffer.
 *
 * This template implements a classic stack interface on top of
 * DynamicArray<Type>. Elements live in a single contiguous allocation for
 * cache-friendly access, and all operations act on the logical “top” at the end
 * of the buffer. Capacity management (growth/shrink and alignment) is delegated
 * to the underlying dynamic array.
 *
 * @tparam Type Element type stored by the stack.
 *
 * @par Core semantics
 * - push/emplace: add a new element to the top.
 * - top/top() const: return a reference to the current top element.
 * - pop: remove the top element and return it by value (moved).
 * - clear: destroy all elements; size() becomes 0.
 * - reserve: ensure capacity without changing size (helps avoid repeated
 * growth).
 * - shrinkToFit: reduce capacity toward the current size to release memory.
 *
 * @par Performance
 * - push/emplace: amortized O(1); O(n) only when a growth reallocation occurs.
 * - pop: O(1); may be O(n) on occasions when the underlying buffer shrinks.
 * - top/isEmpty/size: O(1).
 *
 * @par Exception safety
 * - Growth/shrink paths in the underlying DynamicArray use
 * allocate+construct+commit and provide the strong guarantee (the stack is
 * unchanged on failure).
 * - Constructing a pushed/emplaced value may propagate exceptions from Type.
 * - pop first moves the top element into the return object; if that move/copy
 * throws, the stack state is unchanged (strong for the removal step).
 *
 * @par Invalidation
 * - Any reallocation (growth or shrink) invalidates references, pointers, and
 * iterators to stored elements. Operations that do not reallocate leave
 * existing references valid.
 *
 * @par Type requirements
 * - Type must be MoveConstructible or CopyConstructible. If Type is nothrow
 * move-constructible, relocations prefer move over copy when growing/shrinking.
 *
 * @par Moved-from state
 * - A moved-from Stack remains valid and behaves as empty (size() == 0).
 *
 * @par Thread-safety
 * - Not thread-safe; external synchronization is required for concurrent
 * access.
 *
 * @par Notes
 * - Use reserve() to avoid repeated growth during predictable bursts of
 * push/emplace.
 * - Use shrinkToFit() to release memory after large temporary spikes.
 */

template <typename Type>
class Stack {

    DynamicArray<Type> array_;

  public:
    /// Default constructor
    Stack() : array_() {}

    /// Constructor with initial capacity
    explicit Stack(std::size_t capacity) : array_(capacity) {}

    /// Constructor for braced-init-lists
    Stack(std::initializer_list<Type> initial_data) : array_(initial_data) {}

    /**
     * Constructor with initial data and size.
     *
     * This constructor initializes the stack with a given array of initial
     * data and its size. It reserves enough space in the underlying dynamic
     * array to hold the initial elements.
     *
     * @param initial_data Pointer to the initial data array.
     * @param initial_size The number of elements in the initial data array.
     */
    Stack(const Type* initial_data, const std::size_t initial_size)
        : array_(initial_data, initial_size) {}

    /// Copy constructor
    Stack(const Stack& other) : array_(other.array_) {}

    /// Move constructor
    Stack(Stack&& other) noexcept : array_(std::move(other.array_)) {}

    /// Copy assignment operator
    Stack& operator=(const Stack& other) {
        if (this == &other)
            return *this;
        array_ = other.array_;
        return *this;
    }

    /// Move assignment operator
    Stack& operator=(Stack&& other) noexcept {
        if (this == &other)
            return *this;
        array_ = std::move(other.array_);
        return *this;
    }


    /// Checks if the stack is empty.
    [[nodiscard]]
    bool isEmpty() const noexcept {
        return array_.isEmpty();
    }

    /// Returns the number of elements in the stack.
    [[nodiscard]]
    std::size_t size() const noexcept {
        return array_.size();
    }

    /// Clears the stack, removing all elements.
    void clear() { array_.clear(); }

    /// Shrinks the underlying array to fit the current size.
    void shrinkToFit() { array_.shrinkToFit(); }


    /**
     * Reserve space for at least `capacity` elements (size unchanged).
     *
     * @par Complexity
     * O(n) move/copy if reallocation occurs;
     * otherwise O(1).
     *
     * @par Exception Safety
     * Strong (allocate+construct+commit).
     */
    void reserve(const std::size_t capacity) { array_.reserve(capacity); }


    /**
     * Push a new element on top of the stack.
     *
     * @tparam U Value type that can construct `Type`.
     * @param element The value to push (perfect-forwarded).
     *
     * @par Complexity
     * Amortized O(1); O(n) when an internal growth reallocation occurs.
     *
     * @par Exception Safety
     * Strong on growth (allocate+construct+commit); otherwise propagates from
     * `Type` construction.
     *
     * @par Invalidation
     * Growth reallocation invalidates references/pointers/iterators to
     * elements.
     */
    template <typename U>
    void push(U&& element) {
        static_assert(std::is_constructible_v<Type, U&&>,
                      "U must be constructible into Type");
        array_.addLast(std::forward<U>(element));
    }


    /**
     * Pop and return the top element by value.
     *
     * @return The removed element (moved).
     * @throws std::out_of_range if the stack is empty.
     *
     * @par Complexity
     * O(1); may become O(n) if the underlying array shrinks.
     *
     * @par Invalidation
     * A shrink (triggered by the underlying array) invalidates
     * references/pointers/iterators.
     */
    Type pop() {
        if (array_.isEmpty())
            throw std::out_of_range("Stack is empty");
        return array_.removeLast();
    }


    /**
     * Access the top element by reference.
     *
     * @return Reference to the top element.
     * @throws std::out_of_range if the stack is empty.
     *
     * Complexity: O(1).
     */
    Type& top() {
        if (array_.isEmpty())
            throw std::out_of_range("Stack is empty");
        return array_.getLast();
    }


    /**
     * Const overload of top().
     *
     * @return Const reference to the top element.
     * @throws std::out_of_range if the stack is empty.
     *
     * Complexity: O(1).
     */
    const Type& top() const {
        if (array_.isEmpty())
            throw std::out_of_range("Stack is empty");
        return array_.getLast();
    }


    /**
     * Emplace-construct a new element on top of the stack.
     *
     * @tparam Args Constructor argument types for `Type`.
     * @param args  Arguments to forward to `Type`'s constructor.
     *
     * @par Complexity
     * Amortized O(1); O(n) if growth occurs.
     *
     * @par Exception Safety
     * Strong on growth; otherwise propagates from `Type` construction.
     *
     * @par Invalidation
     * Growth reallocation invalidates references/pointers/iterators.
     */
    template <typename... Args>
    void emplace(Args&&... args) {
        array_.emplaceLast(std::forward<Args>(args)...);
    }


    ~Stack() = default;
};

} // namespace data_structs

#endif // STACK_HPP
