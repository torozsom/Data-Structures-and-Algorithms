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
    Stack() : array_() {}

    Stack(const Stack& other) : array_(other.array_) {}

    Stack(Stack&& other) noexcept : array_(std::move(other.array_)) {}

    Stack& operator=(const Stack& other) {
        if (this == &other)
            return *this;
        array_ = other.array_;
        return *this;
    }

    Stack& operator=(Stack&& other) noexcept {
        if (this == &other)
            return *this;
        array_ = std::move(other.array_);
        return *this;
    }


    bool isEmpty() const noexcept { return array_.isEmpty(); }

    std::size_t size() const noexcept { return array_.size(); }

    void clear() { array_.clear(); }


    /**
     * Adds an element to the top of the stack.
     *
     * @param element The element to be added to the stack.
     */
    void push(const Type& element) { array_.addLast(element); }


    /**
     * Removes and returns the top element of the stack.
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
     * @return A constant reference to the top element of the stack.
     * @throws std::out_of_range If the stack is empty.
     */
    const Type& top() const {
        if (array_.isEmpty())
            throw std::out_of_range("Stack is empty");

        return array_.getLast();
    }


    /// Destructor
    ~Stack() = default;
};


#endif // STACK_HPP
