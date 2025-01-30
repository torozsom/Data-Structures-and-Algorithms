

#ifndef STACK_HPP
#define STACK_HPP

#include <iostream>
#include "DynamicArray.hpp"

/**
 * A data structure that follows the Last In, First Out (LIFO) principle.
 *
 * The Stack class provides methods to add, access, and remove elements
 * from the stack. Elements can only be added to or removed from the top
 * of the stack.
 */
template<typename Type>
class Stack {

private:
    DynamicArray<Type> array_;

public:
    Stack() = default;

    Stack(const Stack& other) = default;

    Stack(Stack&& other) noexcept = default;

    Stack& operator=(const Stack& other) = default;

    Stack& operator=(Stack&& other) noexcept = default;



    bool isEmpty() const { return array_.isEmpty(); }

    unsigned int getSize() const { return array_.getSize(); }

    void clear() { array_.clear(); }


    /**
     * Adds an element to the top of the stack.
     *
     * @param element The element to be added to the stack.
     */
    void push(const Type& element) {
        array_.addLast(element);
    }


    /**
     * Removes and returns the top element of the stack.
     *
     * @return A reference to the top element of the stack.
     * @throws std::out_of_range If the stack is empty.
     */
    Type pop() {
        if (array_.isEmpty())
            throw std::out_of_range("Stack is empty");

        Type element = array_[array_.getSize() - 1];
        array_.remove(array_.getSize() - 1);
        return element;
    }


    /**
     * Retrieves the top element of the stack without removing it.
     *
     * @return A reference to the top element of the stack.
     * @throws std::out_of_range If the stack is empty.
     */
    Type& peek() {
        if (array_.isEmpty())
            throw std::out_of_range("Stack is empty");

        return array_[array_.getSize() - 1];
    }


    /**
     * Retrieves the top element of the stack without removing it.
     *
     * @return A constant reference to the top element of the stack.
     * @throws std::out_of_range If the stack is empty.
     */
    const Type& peek() const {
        if (array_.isEmpty())
            throw std::out_of_range("Stack is empty");

        return array_[array_.getSize() - 1];
    }


    /**
     * Prints the elements of the stack from top to bottom.
     * Outputs the representation to the standard output stream.
     */
    void print() const {
        std::cout << "Stack (top to bottom): ";
        for (unsigned int i = array_.getSize(); i > 0; --i)
            std::cout << array_[i - 1] << " ";
        std::cout << std::endl;
    }


    /// Destructor
    ~Stack() = default;
};


#endif //STACK_HPP
