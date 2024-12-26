//
// Created by toron on 2024. 12. 24..
//


#include "DebugTest.h"



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


/**
 * @brief Tests various functionalities of a doubly linked list implementation.
 *
 * This method evaluates the correctness of a LinkedList class with the following operations:
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
 * The function uses try-catch blocks to handle exceptions caused by out-of-range access or removal.
 */
void testLinkedList() {
     // Create a LinkedList of integers
    LinkedList<int> list;

    std::cout << "\n\n==== Testing Doubly Linked List ====" << std::endl;

    // Test isEmpty and getSize on an empty list
    std::cout << "Is the list empty? " << (list.isEmpty() ? "Yes" : "No") << std::endl;
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
    std::cout << "Is the list empty? " << (list.isEmpty() ? "Yes" : "No") << std::endl;

    // Test edge cases for get on an empty list
    try {
        list.get(0);
    } catch (const std::out_of_range& e) {
        std::cout << "Error (get on empty list): " << e.what() << std::endl;
    }

    std::cout << "\n==== Doubly Linked List Testing Complete ====" << std::endl;
}


/**
 * @brief Tests various functionalities of a stack implementation.
 *
 * This method evaluates the correctness of a generic stack implementation using the following operations:
 * - Checking if the stack is empty and retrieving its size.
 * - Pushing elements onto the stack.
 * - Printing the contents of the stack from top to bottom.
 * - Peeking at the top element without removing it from the stack.
 * - Popping elements from the stack and retrieving the updated state.
 * - Clearing the stack and verifying its empty state.
 *
 * The method includes scenarios to ensure correct behavior during typical stack operations
 * and edge cases such as empty stack conditions.
 */
void testStack() {
    Stack<int> stack;

    std::cout << "\n\n==== Testing Stack Implementation ====" << std::endl;

    // Test isEmpty and getSize
    std::cout << "Is stack empty? " << (stack.isEmpty() ? "Yes" : "No") << std::endl;
    std::cout << "Stack size: " << stack.getSize() << std::endl;

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
    std::cout << "Stack size: " << stack.getSize() << std::endl;
    std::cout << "Is stack empty? " << (stack.isEmpty() ? "Yes" : "No") << std::endl;

    // Clear stack
    stack.clear();
    std::cout << "\nStack after clearing:" << std::endl;
    stack.print();

    std::cout << "Is stack empty? " << (stack.isEmpty() ? "Yes" : "No") << std::endl;
}


/**
 * @brief Tests various functionalities of a binary tree implementation.
 *
 * This method evaluates the correctness of a BinaryTree class by performing the following actions:
 * - Verifying if the tree is initially empty by checking its height.
 * - Inserting a root node and verifying its properties through in-order traversal and height measurement.
 * - Adding nodes to the left subtree and testing the updated tree structure using in-order traversal and height analysis.
 * - Adding nodes to the right subtree and confirming the updated tree configuration through in-order traversal and height verification.
 * - Testing different traversal methods such as in-order, pre-order, and post-order to verify proper ordering of nodes in the tree.
 * - Clearing the tree to ensure all nodes are removed and confirming the state of the tree as empty.
 *
 * The function outputs intermediate results to assist in debugging and validation of the binary tree's behavior.
 */
void testBinaryTree() {
    BinaryTree<int> tree;

    std::cout << "\n\n==== Testing Binary Tree ====" << std::endl;

    // **Test Empty Tree**
    std::cout << "Is the tree empty initially? " << (tree.getHeight() == 0 ? "Yes" : "No") << std::endl;

    // **Insert Root Node**
    std::cout << "\n-- Inserting Root Node --" << std::endl;
    tree.insertLeft(10); // The first insert will become the root (if logic permits)
    std::cout << "In-Order Traversal after inserting root: ";
    tree.printInOrder();
    std::cout << "\nTree Height: " << tree.getHeight() << std::endl;

    // **Insert Left Nodes**
    std::cout << "\n-- Inserting Left Nodes --" << std::endl;
    tree.insertLeft(5);
    tree.insertLeft(2);
    std::cout << "In-Order Traversal after inserting left nodes: ";
    tree.printInOrder();
    std::cout << "\nTree Height: " << tree.getHeight() << std::endl;

    // **Insert Right Nodes**
    std::cout << "\n-- Inserting Right Nodes --" << std::endl;
    tree.insertRight(15);
    tree.insertRight(20);
    std::cout << "In-Order Traversal after inserting right nodes: ";
    tree.printInOrder();
    std::cout << "\nTree Height: " << tree.getHeight() << std::endl;

    // **Test Different Traversals**
    std::cout << "\n-- Traversals --" << std::endl;
    std::cout << "In-Order Traversal: ";
    tree.printInOrder();
    std::cout << "\nPre-Order Traversal: ";
    tree.printPreOrder();
    std::cout << "\nPost-Order Traversal: ";
    tree.printPostOrder();
    std::cout << "\nTree Height: " << tree.getHeight() << std::endl;

    // **Test Tree Clearing**
    std::cout << "\n-- Clearing the Tree --" << std::endl;
    tree.clear();
    std::cout << "Tree cleared.\n";
    std::cout << "Is the tree empty after clearing? " << (tree.getHeight() == 0 ? "Yes" : "No") << std::endl;
}



void testBinarySearchTree() {

    BinarySearchTree<int> bst;

    std::cout << "\n\n==== Testing Binary Search Tree ====" << std::endl;

    // **Insert Elements**
    bst.insert(50);
    bst.insert(30);
    bst.insert(70);
    bst.insert(20);
    bst.insert(40);
    bst.insert(60);
    bst.insert(80);

    // **Traversals**
    std::cout << "\nIn-Order Traversal (Should be sorted): ";
    bst.printInOrder();

    std::cout << "\nPre-Order Traversal: ";
    bst.printPreOrder();

    std::cout << "\nPost-Order Traversal: ";
    bst.printPostOrder();

    std::cout << "\nTree Height: " << bst.getHeight() << "\n";

    // **Remove Elements**
    std::cout << "\n-- Removing Leaf Node (20) --" << std::endl;
    bst.remove(20);
    bst.printInOrder();

    std::cout << "\n-- Removing Node with One Child (30) --" << std::endl;
    bst.remove(30);
    bst.printInOrder();

    std::cout << "\n-- Removing Node with Two Children (50) --" << std::endl;
    bst.remove(50);
    bst.printInOrder();

    std::cout << "\nTree Height after deletions: " << bst.getHeight() << "\n";

    // **Clear Tree**
    std::cout << "\n-- Clearing the Tree --" << std::endl;
    bst.clear();
    std::cout << "Tree cleared. Is empty? " << (bst.getHeight() == 0 ? "Yes" : "No") << "\n";
}
