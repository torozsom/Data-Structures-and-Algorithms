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

  private:
    DynamicArray<Type> array_;

  public:
    /// Default constructor
    Stack() : array_() {}

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
    bool isEmpty() const noexcept { return array_.isEmpty(); }

    /// Returns the number of elements in the stack.
    std::size_t size() const noexcept { return array_.size(); }

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
    void push(const Type& element) { array_.addLast(element); }


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
    template <typename... Args> void emplace(Args&&... args) {
        array_.emplaceLast(std::forward<Args>(args)...);
    }


    /// Destructor
    ~Stack() = default;
};


#endif // STACK_HPP
