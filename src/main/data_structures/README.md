# 🚀 Advanced Data Structures Library

<div align="center">

![C++ Version](https://img.shields.io/badge/C++-17-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)

**A comprehensive, template-based implementation of fundamental data structures in modern C++17**

</div>

## 📋 Table of Contents

- [Overview](#-overview)
- [Data Structures](#-data-structures)
- [Implementation Philosophy](#-implementation-philosophy)
- [Performance Analysis](#-performance-analysis)
- [Advanced Features](#-advanced-features)
- [Usage Examples](#-usage-examples)
- [Testing](#-testing)
- [Future Roadmap](#-future-roadmap)

## 🔍 Overview

This library provides robust, efficient, and thoroughly tested implementations of essential data structures using modern
C++17 features. Each implementation is designed with a focus on:

- **Type Safety**: Template-based design for maximum flexibility
- **Exception Safety**: Strong exception guarantees throughout the codebase
- **Memory Efficiency**: Careful resource management with RAII principles
- **Performance**: Optimized algorithms with appropriate complexity guarantees
- **Maintainability**: Clean, well-documented code with comprehensive test coverage

## 📊 Data Structures

|     Data Structure     |                 Implementation                 |                         Operations                         |         Time Complexity          | Space Complexity |
|:----------------------:|:----------------------------------------------:|:----------------------------------------------------------:|:--------------------------------:|:----------------:|
|   **Dynamic Array**    |     [`DynamicArray.hpp`](DynamicArray.hpp)     | Access<br>Insert/Remove (end)<br>Insert/Remove (arbitrary) |  O(1)<br>Amortized O(1)<br>O(n)  |       O(n)       |
|    **Linked List**     |       [`LinkedList.hpp`](LinkedList.hpp)       |  Access<br>Insert/Remove (ends)<br>Insert/Remove (middle)  |       O(n)<br>O(1)<br>O(n)       |       O(n)       |
|       **Stack**        |            [`Stack.hpp`](Stack.hpp)            |                    Push<br>Pop<br>Peek                     |       O(1)<br>O(1)<br>O(1)       |       O(n)       |
|       **Queue**        |            [`Queue.hpp`](Queue.hpp)            |              Enqueue<br>Dequeue<br>Front/Back              |       O(1)<br>O(1)<br>O(1)       |       O(n)       |
|    **Binary Tree**     |       [`BinaryTree.hpp`](BinaryTree.hpp)       |               Insert<br>Search<br>Traversals               |       O(n)<br>O(n)<br>O(n)       |       O(n)       |
| **Binary Search Tree** | [`BinarySearchTree.hpp`](BinarySearchTree.hpp) |           Insert<br>Search<br>Delete<br>Min/Max            | O(h)*<br>O(h)*<br>O(h)*<br>O(h)* |       O(n)       |
|      **Min Heap**      |          [`MinHeap.hpp`](MinHeap.hpp)          |               Insert<br>Extract-Min<br>Peek                |   O(log n)<br>O(log n)<br>O(1)   |       O(n)       |
|      **Max Heap**      |          [`MaxHeap.hpp`](MaxHeap.hpp)          |               Insert<br>Extract-Max<br>Peek                |   O(log n)<br>O(log n)<br>O(1)   |       O(n)       |

*where h is the height of the tree (worst case O(n), balanced case O(log n))

## 🛠 Implementation Philosophy

### Dynamic Array

A versatile resizable array with dynamic memory management that automatically grows and shrinks to accommodate elements
efficiently.

**Key Features:**

- ✅ Automatic capacity management (doubles when full, halves when 25% utilized)
- ✅ Constant-time random access via operator[] and get() methods
- ✅ Efficient O(1) amortized insertion/removal at the end
- ✅ Comprehensive exception safety guarantees

**Distinctive Approach:**

- Uses strategic resizing to maintain O(1) amortized operations
- Implements optimized copy-on-write semantics
- Provides both move and copy operations for maximum flexibility

### Linked List

A bidirectional linked list implementation offering efficient manipulation at both ends.

**Key Features:**

- ✅ O(1) operations at both ends (addFirst, addLast, removeFirst, removeLast)
- ✅ Bidirectional traversal capabilities (printForward, printBackward)
- ✅ Smart index-based access that approaches from nearest end

**Distinctive Approach:**

- Optimizes element access by choosing the shortest traversal path
- Implements a doubly-linked structure for bidirectional navigation
- Carefully manages pointer relationships to prevent memory leaks

### Stack

A Last-In-First-Out (LIFO) data structure with comprehensive memory management.

**Key Features:**

- ✅ Constant-time push and pop operations
- ✅ Immediate access to the top element
- ✅ Built on top of the DynamicArray for memory efficiency

**Distinctive Approach:**

- Leverages the DynamicArray's amortized efficiency
- Provides clear semantic operations (push, pop, top)
- Maintains exception safety across all operations

### Queue

A First-In-First-Out (FIFO) data structure implemented with optimal efficiency.

**Key Features:**

- ✅ O(1) enqueue and dequeue operations
- ✅ Constant-time access to both front and back elements
- ✅ Built on the DynamicArray for reliable performance

**Distinctive Approach:**

- Uses a circular buffer concept for efficient memory utilization
- Implements full exception safety with clear error reporting
- Provides intuitive interface with front() and back() methods

### Binary Tree

A flexible binary tree implementation supporting various traversal methods and operations.

**Key Features:**

- ✅ Support for level-order, in-order, pre-order, and post-order traversals
- ✅ Comprehensive tree inspection capabilities (height, size, isEmpty)
- ✅ Efficient node insertion and removal mechanisms

**Distinctive Approach:**

- Implements both recursive and iterative traversal algorithms
- Maintains parent pointers for enhanced navigation
- Provides utility methods for tree structure validation

### Binary Search Tree

An ordered binary tree maintaining the BST property for efficient searching.

**Key Features:**

- ✅ O(log n) search, insert, and delete operations (balanced case)
- ✅ Automatic element ordering based on comparison
- ✅ Efficient range-based operations

**Distinctive Approach:**

- Handles all deletion cases elegantly (leaf, one child, two children)
- Maintains the BST invariant throughout all operations
- Implements efficient minimum/maximum value finding

### Min/Max Heaps

Binary heap implementations optimized for priority queue operations.

**Key Features:**

- ✅ O(1) access to minimum/maximum element
- ✅ O(log n) insertion and extraction operations
- ✅ Complete binary tree structure for space efficiency

**Distinctive Approach:**

- Implements the heap property through heapifyUp and heapifyDown algorithms
- Uses bit manipulation for efficient navigation
- Extends from a common abstract Heap base class

## 📈 Performance Analysis

### Time Complexity Highlights

| Operation | Dynamic Array | Linked List  | BST (balanced) |   Heap   |
|:---------:|:-------------:|:------------:|:--------------:|:--------:|
|  Search   |     O(n)      |     O(n)     |    O(log n)    |   O(n)   |
|  Insert   | O(1)* / O(n)  | O(1)* / O(n) |    O(log n)    | O(log n) |
|  Delete   | O(1)* / O(n)  | O(1)* / O(n) |    O(log n)    | O(log n) |
|  Min/Max  |     O(n)      |     O(n)     |    O(log n)    |   O(1)   |

*At the end/beginning of the structure

### Memory Usage

All implementations are designed with memory efficiency in mind:

- **Dynamic Array**: Adjusts capacity dynamically, using only necessary memory
- **Linked List**: Uses only the memory needed for actual elements plus minimal node overhead
- **Binary Trees/Heaps**: Optimal node structure with minimal overhead

### Exception Safety

All operations provide strong exception safety guarantees, ensuring that data structures remain in a valid state even
when exceptions occur.

## 🔧 Advanced Features

### Template Design

All data structures use C++17 template features for maximum flexibility, allowing them to store any data type that
supports the required operations.

### Resource Management

Implementations follow RAII principles, ensuring proper resource acquisition and release, with explicit support for:

- Copy semantics
- Move semantics
- Exception safety

### Validation Methods

Many data structures include self-validation methods:

- `isValidBST()` for Binary Search Trees
- `isValidHeap()` for Min/Max Heaps
- `isCompleteTree()` for Binary Trees

## 💻 Usage Examples

```cpp
// Dynamic Array with automatic resizing
DynamicArray<int> array;
array.addLast(10);
array.addFirst(5);
int value = array[0];  // 5
array.removeAt(1);     // Removes 10

// Doubly Linked List with bidirectional operations
LinkedList<std::string> list;
list.addLast("world");
list.addFirst("hello");
list.printForward();   // "hello world"
list.printBackward();  // "world hello"

// Stack with LIFO semantics
Stack<double> stack;
stack.push(3.14);
stack.push(2.71);
double top = stack.pop();  // 2.71

// Queue with FIFO operations
Queue<char> queue;
queue.enqueue('A');
queue.enqueue('B');
char front = queue.dequeue();  // 'A'

// Binary Search Tree with ordered operations
BinarySearchTree<int> bst;
bst.insert(50);
bst.insert(30);
bst.insert(70);
bst.printInOrder();  // "30 50 70"
int min = bst.findMinimum();  // 30

// Priority Queue implementation with Min Heap
MinHeap<int> minHeap;
minHeap.insert(5);
minHeap.insert(3);
minHeap.insert(7);
int highest_priority = minHeap.extractRoot();  // 3
```

## 🧪 Testing

Each data structure is rigorously tested with comprehensive test cases in the `src/test/data_structures` directory:

- **Unit Tests**: Verify individual operations
- **Integration Tests**: Ensure data structures work together correctly
- **Edge Cases**: Test boundary conditions and error handling
- **Performance Tests**: Validate complexity guarantees

## 🚧 Future Roadmap

- **Balanced Trees**: Implement AVL and Red-Black tree balancing algorithms
- **Advanced Data Structures**: Add Trie, Graph, and Hash Table implementations
- **Iterators**: Provide STL-compatible iterators for all containers
- **Parallelism**: Explore thread-safe variants of selected data structures
- **Benchmarking**: Add comprehensive performance benchmarking suite
- **Serialization**: Support for persistence and serialization operations

---

<div align="center">

**Built with ❤️ and Modern C++17**

</div>