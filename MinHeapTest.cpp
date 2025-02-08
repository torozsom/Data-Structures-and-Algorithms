#include <iostream>
#include "MinHeap.hpp"

void testMinHeap() {
    MinHeap<int> heap;

    // Test Insert
    std::cout << "Inserting elements: 10, 5, 15, 3, 8\n";
    heap.insert(10);
    heap.insert(5);
    heap.insert(15);
    heap.insert(3);
    heap.insert(8);

    std::cout << "Min element (root): " << heap.peekRoot() << " (Expected: 3)\n";
    std::cout << "Heap size: " << heap.size() << " (Expected: 5)\n";

    // Test Extract Root
    std::cout << "Extracting root: " << heap.extractRoot() << " (Expected: 3)\n";
    std::cout << "New root: " << heap.peekRoot() << " (Expected: 5)\n";
    std::cout << "Heap size: " << heap.size() << " (Expected: 4)\n";

    // Test Remove
    std::cout << "Removing element 5\n";
    heap.remove(5);
    std::cout << "New root: " << heap.peekRoot() << " (Should not be 5)\n";
    std::cout << "Heap size: " << heap.size() << " (Expected: 3)\n";

    // Edge Case: Extract from empty heap
    std::cout << "Extracting all elements...\n";
    while (!heap.isEmpty()) {
        std::cout << "Extracted: " << heap.extractRoot() << "\n";
    }

    std::cout << "Heap is now empty: " << (heap.isEmpty() ? "Yes" : "No") << " (Expected: Yes)\n";

    // Clear Heap Test
    heap.insert(20);
    heap.insert(30);
    heap.clear();
    std::cout << "Heap cleared. Size: " << heap.size() << " (Expected: 0)\n";
}


