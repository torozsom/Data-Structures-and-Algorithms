
#include "Tests.h"


void testMaxHeap() {
    MaxHeap<int> heap;

    // Insert elements into the heap
    std::cout << "Inserting elements into the MaxHeap: 10, 20, 15, 5, 8, 25\n";
    heap.insert(10);
    heap.insert(20);
    heap.insert(15);
    heap.insert(5);
    heap.insert(8);
    heap.insert(25);

    // Check the root value
    std::cout << "Current root of the MaxHeap: " << heap.peekRoot()
              << " (Expected: 25)\n";
    std::cout << "Current size of the MaxHeap: " << heap.size()
              << " (Expected: 6)\n";

    // Extract the root and check the new root
    std::cout << "\nExtracting the root...\n";
    const int extractedRoot = heap.extractRoot();
    std::cout << "Extracted root value: " << extractedRoot
              << " (Expected: 25)\n";
    std::cout << "New root after extraction: " << heap.peekRoot()
              << " (Expected: 20)\n";
    std::cout << "New size of the MaxHeap: " << heap.size()
              << " (Expected: 5)\n";

    std::cout << "\nExtracting all elements from the MaxHeap:\n";
    while (!heap.isEmpty())
        std::cout << "Extracted value: " << heap.extractRoot() << "\n";

    std::cout << "Heap is empty: " << (heap.isEmpty() ? "Yes" : "No")
              << " (Expected: Yes)\n";
}
