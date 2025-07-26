
#include "integration/Tests.h"


enum class DataStructure {
    DYNAMIC_ARRAY = 1,
    LINKED_LIST,
    STACK,
    BINARY_TREE,
    BINARY_SEARCH_TREE,
    MIN_HEAP,
    MAX_HEAP,
    ALL,
    EXIT
};


void printMenu() {
    std::cout << "\nWhat would you like to test?\n"
                 "1. Dynamic Array \n"
                 "2. Linked List \n"
                 "3. Stack \n"
                 "4. Binary Tree \n"
                 "5. Binary Search Tree \n"
                 "6. Minimum Heap \n"
                 "7. Maximum Heap \n"
                 "8. All \n"
                 "9. Exit \n";
}


int main() {
    int num;

    do {
        printMenu();
        std::cout << "\nEnter a number:  ";
        std::cin >> num;
        switch (static_cast<DataStructure>(num)) {
        case DataStructure::DYNAMIC_ARRAY:
            testDynamicArray();
            break;

        case DataStructure::LINKED_LIST:
            testLinkedList();
            break;

        case DataStructure::STACK:
            testStack();
            break;

        case DataStructure::BINARY_TREE:
            testBinaryTree();
            break;

        case DataStructure::BINARY_SEARCH_TREE:
            testBinarySearchTree();
            break;

        case DataStructure::MIN_HEAP:
            testMinHeap();
            break;

        case DataStructure::MAX_HEAP:
            testMaxHeap();
            break;

        case DataStructure::ALL:
            testDynamicArray();
            testLinkedList();
            testStack();
            testBinaryTree();
            testBinarySearchTree();
            testMinHeap();
            testMaxHeap();
            break;

        case DataStructure::EXIT:
            break;

        default:
            std::cout << "Invalid input" << std::endl;
            break;
        }
    } while (num != 9);

    return 0;
}
