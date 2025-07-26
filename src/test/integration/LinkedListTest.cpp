
#include "Tests.h"

/**
 * @brief Tests various functionalities of a doubly linked list implementation.
 *
 * This method evaluates the correctness of a LinkedList class with the
 * following operations:
 * - Checking if the list is empty and getting its size.
 * - Adding elements to the beginning and end of the list.
 * - Printing the list contents in both forward and backward order.
 * - Accessing elements at specific indices.
 * - Inserting elements at specific indices.
 * - Removing elements from the beginning, end, or a specific index.
 * - Removing elements by value.
 * - Handling edge cases such as invalid indices and operating on an empty list.
 * - Clearing the list and verifying its state afterward.
 *
 * The function uses try-catch blocks to handle exceptions caused by
 * out-of-range access or removal.
 */
void testLinkedList() {
    // Create a LinkedList of integers
    LinkedList<int> list;

    std::cout << "\n\n==== Testing Doubly Linked List ====" << std::endl;

    // Test isEmpty and getSize on an empty list
    std::cout << "Is the list empty? " << (list.isEmpty() ? "Yes" : "No")
              << std::endl;
    std::cout << "Size of the list: " << list.getSize() << std::endl;

    // Add elements to the list
    std::cout << "\n-- Adding Elements --" << std::endl;
    list.addFirst(10);
    list.addLast(20);
    list.addLast(30);
    list.addFirst(5);
    std::cout << "List after adding elements (Forward): ";
    list.printForward();
    std::cout << "List after adding elements (Backward): ";
    list.printBackward();

    // Test getSize
    std::cout << "Size after additions: " << list.getSize() << std::endl;

    // Test get
    std::cout << "\n-- Accessing Elements --" << std::endl;
    try {
        std::cout << "Element at index 0: " << list.get(0) << std::endl;
        std::cout << "Element at index 2: " << list.get(2) << std::endl;
        std::cout << "Element at index 3: " << list.get(3) << std::endl;
    } catch (const std::out_of_range& e) {
        std::cout << e.what() << std::endl;
    }

    // Test insert
    std::cout << "\n-- Inserting Elements --" << std::endl;
    list.insert(2, 15); // Insert 15 at index 2
    std::cout << "List after inserting 15 at index 2 (Forward): ";
    list.printForward();

    // Test removeFirst
    std::cout << "\n-- Removing First Element --" << std::endl;
    list.removeFirst();
    std::cout << "List after removing first element (Forward): ";
    list.printForward();

    // Test removeLast
    std::cout << "\n-- Removing Last Element --" << std::endl;
    list.removeLast();
    std::cout << "List after removing last element (Forward): ";
    list.printForward();

    // Test removeAt
    std::cout << "\n-- Removing Element at Index 1 --" << std::endl;
    list.removeAt(1);
    std::cout << "List after removing element at index 1 (Forward): ";
    list.printForward();

    // Test remove by value
    std::cout << "\n-- Removing Element by Value (20) --" << std::endl;
    list.remove(20);
    std::cout << "List after removing element with value 20 (Forward): ";
    list.printForward();

    // Test edge cases for removeAt
    try {
        list.removeAt(5); // Invalid index
    } catch (const std::out_of_range& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    // Test clear
    std::cout << "\n-- Clearing the List --" << std::endl;
    list.clear();
    std::cout << "List after clearing (Forward): ";
    list.printForward();
    std::cout << "Is the list empty? " << (list.isEmpty() ? "Yes" : "No")
              << std::endl;

    // Test edge cases for get on an empty list
    try {
        list.get(0);
    } catch (const std::out_of_range& e) {
        std::cout << "Error (get on empty list): " << e.what() << std::endl;
    }

    std::cout << "\n==== Doubly Linked List Testing Complete ====" << std::endl;
}