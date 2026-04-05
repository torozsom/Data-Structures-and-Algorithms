#ifndef STACK_HPP
#define STACK_HPP


#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "DynamicArray.hpp"


namespace containers {

using std::size_t;


/**
 * @class Stack
 * @brief A LIFO (last-in, first-out) container backed by a resizable contiguous buffer.
 *
 * Implements a classic stack interface on top of DynamicArray<Type>.
 *
 * @tparam Type Element type stored by the stack.
 */

template <typename Type>
class Stack {

    DynamicArray<Type> array_;

  public:
    /// Default constructor
    Stack() : array_() {}

    /// Constructor with initial capacity
    explicit Stack(size_t capacity) : array_(capacity) {}

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
    Stack(const Type* initial_data, const size_t initial_size)
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
    size_t size() const noexcept {
        return array_.size();
    }

    /// Clears the stack, removing all elements.
    void clear() noexcept { array_.clear(); }

    /// Shrinks the underlying array to fit the current size.
    void shrinkToFit() { array_.shrinkToFit(); }


    /**
     * Reserve space for at least `capacity` elements (size unchanged).
     */
    void reserve(const size_t capacity) { array_.reserve(capacity); }


    /**
     * Push a new element on top of the stack.
     *
     * @tparam U Value type that can construct `Type`.
     * @param element The value to push (perfect-forwarded).
     */
    template <typename U>
    void push(U&& element) {
        static_assert(std::is_constructible_v<Type, U&&>,
                      "U must be constructible into Type");
        array_.addLast(std::forward<U>(element));
    }


    /**
     * Pop the top element from the stack.
     *
     * Removes the element at the top of the stack. To inspect the value,
     * use top() before calling pop().
     *
     * @throws std::out_of_range if the stack is empty.
     */
    void pop() {
        if (array_.isEmpty())
            throw std::out_of_range("Stack is empty");
        array_.popBack();
    }


    /**
     * Access the top element by reference.
     *
     * @return Reference to the top element.
     * @throws std::out_of_range if the stack is empty.
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
     */
    template <typename... Args>
    void emplace(Args&&... args) {
        array_.emplaceLast(std::forward<Args>(args)...);
    }


    ~Stack() = default;
};

} // namespace containers

#endif // STACK_HPP
