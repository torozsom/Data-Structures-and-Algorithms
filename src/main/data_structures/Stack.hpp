#ifndef STACK_HPP
#define STACK_HPP


#include "DynamicArray.hpp"
#include <iostream>


/**
 * A data structure that follows the Last In, First Out (LIFO) principle.
 *
 * The Stack class provides methods to add, access, and remove elements
 * from the stack. Elements can only be added to or removed from the top
 * of the stack.
 */
template <typename Type>
class Stack {

    DynamicArray<Type> array_;


  public:
    /// Default constructor
    Stack() : array_() {}

    /// Constructor with initial capacity
    explicit Stack(const std::size_t capacity) : array_() {
        array_.reserve(capacity);
    }

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


    /**
     * Adds an element to the top of the stack.
     *
     * @complexity O(1) on average, O(n) in the worst case when resizing is
     * needed.
     *
     * @param element The element to be added to the stack.
     */
    template <typename U>
    void push(U&& element) {
        array_.addLast(std::forward<U>(element));
    }


    /**
     * Removes and returns the top element of the stack.
     *
     * @complexity O(1) on average, O(n) in the worst case when resizing is
     * needed.
     *
     * @return A reference to the top element of the stack.
     * @throws std::out_of_range If the stack is empty.
     */
    Type pop() {
        if (array_.isEmpty())
            throw std::out_of_range("Stack is empty");

        return array_.removeLast();
    }


    /**
     * Retrieves the top element of the stack without removing it.
     *
     * @complexity O(1) for direct access.
     *
     * @return A reference to the top element of the stack.
     * @throws std::out_of_range If the stack is empty.
     */
    Type& top() {
        if (array_.isEmpty())
            throw std::out_of_range("Stack is empty");

        return array_.getLast();
    }


    /**
     * Retrieves the top element of the stack without removing it.
     *
     * @complexity O(1) for direct access.
     *
     * @return A constant reference to the top element of the stack.
     * @throws std::out_of_range If the stack is empty.
     */
    const Type& top() const {
        if (array_.isEmpty())
            throw std::out_of_range("Stack is empty");

        return array_.getLast();
    }


    /**
     * Emplaces a new element at the top of the stack, constructing it in place
     * using the provided arguments. If the stack is at full capacity, it
     * resizes to accommodate the new element.
     *
     * @complexity O(1) on average, O(n) in the worst case when resizing is
     * needed.
     *
     * @param args The arguments to be forwarded to the constructor of Type.
     */
    template <typename... Args>
    void emplace(Args&&... args) {
        array_.emplaceLast(std::forward<Args>(args)...);
    }


    /**
     * Reserves space for a specified number of elements in the stack.
     *
     * This method allows pre-allocation of memory to avoid frequent
     * reallocations when adding elements.
     *
     * @param capacity The number of elements to reserve space for.
     */
    void reserve(const std::size_t capacity) { array_.reserve(capacity); }


    /// Destructor
    ~Stack() = default;
};


#endif // STACK_HPP
