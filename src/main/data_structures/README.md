# Data Structures Implementation

This repository contains a collection of fundamental data structures implemented in C++17. These implementations are designed to be efficient, reliable, and educational, providing a solid foundation for algorithm development and computer science education.

## Overview

The following data structures are implemented in this repository:

| Data Structure | File | Time Complexity | Space Complexity |
|---------------|------|-----------------|-------------------|
| Dynamic Array | [`DynamicArray.hpp`](DynamicArray.hpp) | Access: O(1)<br>Insert/Remove: O(n) | O(n) |
| Linked List | [`LinkedList.hpp`](LinkedList.hpp) | Access: O(n)<br>Insert/Remove at ends: O(1)<br>Insert/Remove in middle: O(n) | O(n) |
| Stack | [`Stack.hpp`](Stack.hpp) | Push/Pop/Peek: O(1) | O(n) |
| Binary Tree | [`BinaryTree.hpp`](BinaryTree.hpp) | Access/Search/Insert/Delete: O(n) | O(n) |
| Binary Search Tree | [`BinarySearchTree.hpp`](BinarySearchTree.hpp) | Access/Search/Insert/Delete: O(h)* | O(n) |
| Min Heap | [`MinHeap.hpp`](MinHeap.hpp) | Insert: O(log n)<br>Extract: O(log n)<br>Peek: O(1) | O(n) |
| Max Heap | [`MaxHeap.hpp`](MaxHeap.hpp) | Insert: O(log n)<br>Extract: O(log n)<br>Peek: O(1) | O(n) |

*where h is the height of the tree (worst case O(n), balanced case O(log n))

## Detailed Descriptions

### Dynamic Array

A resizable array implementation that grows and shrinks automatically. The internal capacity doubles when the array is full and halves when it's 25% full, maintaining amortized O(1) insertion at the end.

**Key Features:**
- Automatic resizing with capacity management
- O(1) random access by index
- Efficient addition and removal at the end
- Copy and move semantics fully supported

**Implementation Details:**
- Uses a pointer to a dynamically allocated array of type `Type*`
- Keeps track of size and capacity separately
- Implements copy-on-write semantics for efficiency

### Linked List

A doubly-linked list implementation providing efficient insertion and deletion operations at both ends of the list.

**Key Features:**
- O(1) insertion and deletion at both ends
- Bidirectional traversal capability
- O(1) access to the first and last elements

**Implementation Details:**
- Each node contains data, next pointer, and previous pointer
- Maintains head and tail pointers for quick access to both ends
- Implements optimized iteration by starting from the closest end when accessing by index

### Stack

A Last-In-First-Out (LIFO) data structure implemented using a dynamic array.

**Key Features:**
- O(1) push and pop operations
- O(1) access to the top element
- Memory efficient implementation

**Implementation Details:**
- Built on top of the `DynamicArray` class
- Provides clear LIFO semantics with push, pop, and peek operations
- Automatically handles memory management via the underlying array

### Binary Tree

A binary tree implementation where each node can have at most two children.

**Key Features:**
- Support for in-order, pre-order, and post-order traversals
- Recursive and iterative operations
- Height calculation and size tracking

**Implementation Details:**
- Each node contains data and pointers to left child, right child, and parent
- Maintains a root pointer and size counter
- Provides comprehensive tree manipulation and traversal methods

### Binary Search Tree

A binary tree that maintains the BST property: for each node, all elements in the left subtree are less than the node's element, and all elements in the right subtree are greater.

**Key Features:**
- O(log n) search, insert, and delete operations for balanced trees
- Automatic ordering of elements
- Efficient range queries

**Implementation Details:**
- Extends the `BinaryTree` class
- Implements recursive insertion and deletion algorithms
- Handles special cases like removing nodes with 0, 1, or 2 children

### Min Heap & Max Heap

Binary heaps that maintain the heap property: in a min heap, each parent is less than or equal to its children; in a max heap, each parent is greater than or equal to its children.

**Key Features:**
- O(1) access to the minimum/maximum element
- O(log n) insertion and extraction
- Efficient priority queue implementation

**Implementation Details:**
- Both extend the abstract `Heap` class
- Implement the heap property through heapifyUp and heapifyDown operations
- Use a binary representation of indices for efficient navigation
- Complete binary tree structure for optimal space efficiency

## Implementation Efficiency

All data structures are implemented with careful attention to efficiency:

1. **Memory Management**: Proper resource acquisition and release with RAII principles
2. **Exception Safety**: Strong exception guarantees where possible
3. **Move Semantics**: Efficient transfer of resources
4. **Template Design**: Type-agnostic implementations for maximum flexibility

## Usage Examples

Each data structure can be instantiated and used as follows:

```cpp
// Dynamic Array example
DynamicArray<int> array;
array.addLast(10);
array.addFirst(5);
int value = array[0]; // 5

// Linked List example
LinkedList<string> list;
list.addLast("hello");
list.addLast("world");
list.printForward(); // "hello world"

// Stack example
Stack<double> stack;
stack.push(3.14);
stack.push(2.71);
double top = stack.pop(); // 2.71

// Binary Search Tree example
BinarySearchTree<int> bst;
bst.insert(50);
bst.insert(30);
bst.insert(70);
bst.printInOrder(); // 30 50 70

// Min Heap example
MinHeap<int> minHeap;
minHeap.insert(5);
minHeap.insert(3);
minHeap.insert(7);
int min = minHeap.peekRoot(); // 3
```

## Testing

Comprehensive test cases for each data structure are provided in the `src/test/data_structures` directory.

## Future Improvements

- Add balancing to the Binary Search Tree (AVL or Red-Black tree)
- Implement additional data structures like Queue, Trie, and Graph
- Add iterators for container-like access patterns
- Improve time complexity analysis with benchmarking