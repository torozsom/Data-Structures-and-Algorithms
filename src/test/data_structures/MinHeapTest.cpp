
#include "Tests.h"


void testMinHeap() {
    MinHeap<int> heap;

    // Insert elements into the heap
    std::cout << "Inserting elements into the MinHeap: 10, 5, 15, 3, 8, 4\n";
    heap.insert(10);
    heap.insert(5);
    heap.insert(15);
    heap.insert(3);
    heap.insert(8);
    heap.insert(4);

    // Check the root value
    std::cout << "Current root of the MinHeap: " << heap.peekRoot()
              << " (Expected: 3)\n";
    std::cout << "Current size of the MinHeap: " << heap.size()
              << " (Expected: 6)\n";

    // Extract the root and check the new root
    std::cout << "\nExtracting the root...\n";
    int extractedRoot = heap.extractRoot();
    std::cout << "Extracted root value: " << extractedRoot
              << " (Expected: 3)\n";
    std::cout << "New root after extraction: " << heap.peekRoot()
              << " (Expected: 4)\n";
    std::cout << "New size of the MinHeap: " << heap.size()
              << " (Expected: 5)\n";


    std::cout << "\nExtracting all elements from the MinHeap:\n";
    while (!heap.isEmpty())
        std::cout << "Extracted value: " << heap.extractRoot() << "\n";

    std::cout << "Heap is empty: " << (heap.isEmpty() ? "Yes" : "No")
              << " (Expected: Yes)\n";
}
