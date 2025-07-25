
#include "Tests.h"


/**
 * @brief Tests various functionalities of a stack implementation.
 *
 * This method evaluates the correctness of a generic stack implementation using
 * the following operations:
 * - Checking if the stack is empty and retrieving its size.
 * - Pushing elements onto the stack.
 * - Printing the contents of the stack from top to bottom.
 * - Peeking at the top element without removing it from the stack.
 * - Popping elements from the stack and retrieving the updated state.
 * - Clearing the stack and verifying its empty state.
 *
 * The method includes scenarios to ensure correct behavior during typical stack
 * operations and edge cases such as empty stack conditions.
 */
void testStack() {
    Stack<int> stack;

    std::cout << "\n\n==== Testing Stack Implementation ====" << std::endl;

    // Test isEmpty and getSize
    std::cout << "Is stack empty? " << (stack.isEmpty() ? "Yes" : "No")
              << std::endl;
    std::cout << "Stack size: " << stack.size() << std::endl;

    // Push elements
    stack.push(10);
    stack.push(20);
    stack.push(30);

    std::cout << "\nStack after pushing 10, 20, 30:" << std::endl;
    stack.print();

    // Peek at the top
    std::cout << "Top element: " << stack.peek() << std::endl;

    // Pop an element
    stack.pop();
    std::cout << "\nStack after one pop:" << std::endl;
    stack.print();

    // Check size and empty status
    std::cout << "Stack size: " << stack.size() << std::endl;
    std::cout << "Is stack empty? " << (stack.isEmpty() ? "Yes" : "No")
              << std::endl;

    // Clear stack
    stack.clear();
    std::cout << "\nStack after clearing:" << std::endl;
    stack.print();

    std::cout << "Is stack empty? " << (stack.isEmpty() ? "Yes" : "No")
              << std::endl;
}
