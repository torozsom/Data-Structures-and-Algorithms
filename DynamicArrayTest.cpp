//
// Created by toron on 2024. 12. 26..
//

#include "Tests.h"

/**
 * @brief Tests the functionality of the DynamicArray class with various operations.
 *
 * This method performs a series of tests on a DynamicArray object to ensure it behaves as expected
 * under different scenarios. It tests the following operations:
 * - Checking if the array is empty and retrieving its size and capacity.
 * - Adding elements to the array.
 * - Inserting elements at specific index positions.
 * - Accessing elements using their index.
 * - Removing elements from specific positions in the array.
 * - Attempting to remove elements or access elements at invalid indices to test exception handling.
 * - Clearing all elements in the array.
 * - Testing edge cases such as accessing an empty array or removing invalid indices.
 *
 * The results of the operations and any exceptions encountered are output to the console.
 */
void testDynamicArray() {
    // Create a DynamicArray of integers
    DynamicArray<int> array;

    std::cout << "\n\n==== Testing Dynamic Array ====" << std::endl;

    // Test isEmpty and getSize on an empty array
    std::cout << "Is the array empty? " << (array.isEmpty() ? "Yes" : "No") << std::endl;
    std::cout << "Size of the array: " << array.getSize() << std::endl;
    std::cout << "Capacity of the array: " << array.getCapacity() << std::endl;

    // Adding elements
    std::cout << "\n-- Adding Elements --" << std::endl;
    for (int i = 1; i <= 5; ++i)
        array.addLast(i * 10);

    std::cout << "Array after adding elements: ";
    for (unsigned int i = 0; i < array.getSize(); ++i)
        std::cout << array.get(i) << " ";

    std::cout << "\nCurrent size: " << array.getSize() << ", Capacity: " << array.getCapacity() << std::endl;

    // Inserting elements
    std::cout << "\n-- Inserting Elements --" << std::endl;
    array.insert(2, 15);
    array.insert(0, 5);
    array.insert(array.getSize(), 60);

    std::cout << "Array after inserting elements: ";
    for (unsigned int i = 0; i < array.getSize(); ++i)
        std::cout << array.get(i) << " ";

    std::cout << "\nCurrent size: " << array.getSize() << ", Capacity: " << array.getCapacity() << std::endl;

    // Accessing elements
    std::cout << "\n-- Accessing Elements --" << std::endl;
    try {
        std::cout << "Element at index 0: " << array.get(0) << std::endl;
        std::cout << "Element at index 3: " << array.get(3) << std::endl;
        std::cout << "Element at index 7: " << array.get(7) << std::endl;
    } catch (const std::out_of_range& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    // Removing elements
    std::cout << "\n-- Removing Elements --" << std::endl;
    array.remove(0);
    array.remove(2);
    array.remove(array.getSize() - 1);

    std::cout << "Array after removing elements: ";
    for (unsigned int i = 0; i < array.getSize(); ++i)
        std::cout << array.get(i) << " ";

    std::cout << "\nCurrent size: " << array.getSize() << ", Capacity: " << array.getCapacity() << std::endl;

    // Edge Case: Removing invalid index
    std::cout << "\n-- Removing Invalid Index --" << std::endl;
    try {
        array.remove(10); // Invalid index
    } catch (const std::out_of_range& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    // Clearing the array
    std::cout << "\n-- Clearing the Array --" << std::endl;
    array.clear();
    std::cout << "Array after clearing: ";
    for (unsigned int i = 0; i < array.getSize(); ++i)
        std::cout << array.get(i) << " ";

    std::cout << "\nIs the array empty? " << (array.isEmpty() ? "Yes" : "No") << std::endl;
    std::cout << "Current size: " << array.getSize() << ", Capacity: " << array.getCapacity() << std::endl;

    // Edge Case: Accessing an empty array
    std::cout << "\n-- Accessing an Empty Array --" << std::endl;
    try {
        array.get(0);
    } catch (const std::out_of_range& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    std::cout << "\n==== Dynamic Array Testing Complete ====" << std::endl;
}