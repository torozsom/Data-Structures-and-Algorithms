# 🚀 Advanced Data Structures Library

<div align="center">

![C++ Version](https://img.shields.io/badge/C++-23-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)

**A comprehensive, template-based implementation of fundamental data structures in modern C++23**

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
C++23 features. Each implementation is designed with a focus on:

- **Type Safety**: Template-based design for maximum flexibility
- **Exception Safety**: Strong guarantees where feasible and clearly documented; otherwise a basic guarantee
- **Memory Efficiency**: Careful resource management with RAII principles
- **Performance**: Algorithms with clear, documented complexity
- **Maintainability**: Clean, well-documented code with comprehensive test coverage

## 📊 Data Structures

|     Data Structure     |                 Implementation                 |                             Operations                              |          Time Complexity          | Space Complexity |
|:----------------------:|:----------------------------------------------:|:-------------------------------------------------------------------:|:---------------------------------:|:----------------:|
|   **Dynamic Array**    |     [`DynamicArray.hpp`](src/main/data_structures/DynamicArray.hpp)     |    Access<br>Insert/Remove (end)<br>Insert/Remove (arbitrary)       |  O(1)<br>Amortized O(1)<br>O(n)   |       O(n)       |
|    **Linked List**     |       [`LinkedList.hpp`](src/main/data_structures/LinkedList.hpp)       |    Access<br>Insert/Remove (ends)<br>Insert/Remove (middle)         |       O(n)<br>O(1)<br>O(n)        |       O(n)       |
|       **Stack**        |            [`Stack.hpp`](src/main/data_structures/Stack.hpp)            |                         Push<br>Pop<br>Top                          |       O(1)<br>O(1)<br>O(1)        |       O(n)       |
|       **Queue**        |            [`Queue.hpp`](src/main/data_structures/Queue.hpp)            |                 Enqueue<br>Dequeue<br>Front/Back                    |       O(1)<br>O(1)<br>O(1)        |       O(n)       |
|    **Binary Tree**     |       [`BinaryTree.hpp`](src/main/data_structures/BinaryTree.hpp)       |            Insert (level-order)<br>Find/Contains<br>Height          |               O(n)                |       O(n)       |
| **Binary Search Tree** | [`BinarySearchTree.hpp`](src/main/data_structures/BinarySearchTree.hpp) |           Insert<br>Search/Contains<br>Delete<br>Min/Max            | O(h)\*<br>O(h)\*<br>O(h)\*<br>O(h)\* |       O(n)       |
|      **Min Heap**      |          [`MinHeap.hpp`](src/main/data_structures/MinHeap.hpp)          |                  Insert<br>Extract-Min<br>Peek                      |   O(log n)<br>O(log n)<br>O(1)    |       O(n)       |
|      **Max Heap**      |          [`MaxHeap.hpp`](src/main/data_structures/MaxHeap.hpp)          |                  Insert<br>Extract-Max<br>Peek                      |   O(log n)<br>O(log n)<br>O(1)    |       O(n)       |

\* `h` is the tree height (worst case O(n), balanced case O(log n)).

## 🛠 Implementation Philosophy

### Dynamic Array

A versatile resizable array with dynamic memory management that grows (and may shrink) to accommodate elements efficiently.

**Key Features:**

- ✅ Random access via `operator[]` and `get()`-style APIs
- ✅ Efficient O(1) amortized insertion/removal at the end
- ✅ Move and copy semantics

**Distinctive Approach:**

- Uses a geometric growth strategy for amortized O(1) appends
- Clear exception-safety story and strong invariants around size/capacity

> _Note:_ This library does **not** rely on copy-on-write.

### Linked List

A doubly linked list offering efficient manipulation at both ends and bidirectional traversal via iterators.

**Key Features:**

- ✅ O(1) `addFirst`/`addLast` and `removeFirst`/`removeLast`
- ✅ Bidirectional iterators (`begin/end`, `cbegin/cend`)
- ✅ Index-based access that picks the nearer end for traversal

**Distinctive Approach:**

- Iterator design follows standard bidirectional iterator requirements
- Careful pointer/link management to prevent leaks

### Stack

A Last-In-First-Out (LIFO) container with straightforward semantics.

**Key Features:**

- ✅ Constant-time `push`, `pop`, and `top`
- ✅ Simple interface and predictable performance

### Queue

A First-In-First-Out (FIFO) container with optimal enqueue/dequeue semantics.

**Key Features:**

- ✅ O(1) `enqueue` and `dequeue`
- ✅ Constant-time `front()` and `back()`

### Binary Tree

A flexible, pointer-based binary tree with **level-order insertion**.

**Key Features:**

- ✅ Level-order insertion that keeps the shape compact (not a BST)
- ✅ Structure queries: `getHeight()`, `size()`, `isEmpty()`, `isCompleteTree()`
- ✅ Search helpers: `containsNode`, `findNode`/`findNodeLevelOrder`
- ✅ `clear()` for full teardown

> _Note:_ In-order / pre-order / post-order traversal helpers are not exposed as public APIs in this version.

### Binary Search Tree

An ordered binary tree maintaining the BST property for efficient searching.

**Key Features:**

- ✅ `insert` (no duplicates), `remove`, `contains`
- ✅ `findMinimum()` / `findMaximum()`
- ✅ `isValidBST()` validation helper

### Min/Max Heaps

Pointer-based **complete** binary heaps built on a shared abstract base (`Heap`).

**Key Features:**

- ✅ O(1) access to min/max via `peekRoot()`
- ✅ O(log n) `insert` and `extractRoot()`
- ✅ Navigation by bit-walking the 1-based level-order index
- ✅ Reheapification via `heapifyUp` / `heapifyDown` (data swaps, not pointer swaps)

## 📈 Performance Analysis

### Time Complexity Highlights

| Operation | Dynamic Array | Linked List  | BST (balanced) |   Heap   |
|:---------:|:-------------:|:------------:|:--------------:|:--------:|
|  Search   |     O(n)      |     O(n)     |    O(log n)    |   O(n)   |
|  Insert   | O(1)\* / O(n) | O(1)\* / O(n)|    O(log n)    | O(log n) |
|  Delete   | O(1)\* / O(n) | O(1)\* / O(n)|    O(log n)    | O(log n) |
|  Min/Max  |     O(n)      |     O(n)     |    O(log n)    |   O(1)   |

\* At the end/beginning of the structure.

### Memory Usage

All implementations are designed with memory efficiency in mind:

- **Dynamic Array**: Adjusts capacity dynamically
- **Linked List**: Only per-node overhead beyond elements
- **Binary Trees/Heaps**: Compact node structure with parent pointers

### Exception Safety

- Strong guarantees where feasible (e.g., many single-node insertions)
- Basic guarantees in operations that move payloads during reheapification or deep copies (e.g., tree copy helpers)
- Clear precondition checks with standard exceptions (`std::out_of_range`, `std::invalid_argument`)

## 🔧 Advanced Features

- **Template Design**: Store any type meeting the required operations (e.g., `operator<` for BST/Heaps)
- **Resource Management**: RAII across nodes/containers with copy and move semantics
- **Validation Methods**:
    - `isValidBST()` for Binary Search Trees
    - `isValidHeap()` for Min/Max Heaps
    - `isCompleteTree()` for Binary Trees

## 💻 Usage Examples

```cpp
// Dynamic Array with automatic resizing
DynamicArray<int> array;
array.addLast(10);
array.addFirst(5);
int v = array[0];        // 5
array.removeAt(1);       // Removes 10

// Doubly Linked List with iterators
LinkedList<std::string> list;
list.addLast("world");
list.addFirst("hello");
for (const auto& s : list) {
    // prints: hello world
    std::cout << s << ' ';
}

// Stack with LIFO semantics
Stack<double> stack;
stack.push(3.14);
stack.push(2.71);
double top = stack.pop();     // 2.71

// Queue with FIFO operations
Queue<char> q;
q.enqueue('A');
q.enqueue('B');
char front = q.dequeue();     // 'A'

// Binary Search Tree with ordered operations
BinarySearchTree<int> bst;
bst.insert(50);
bst.insert(30);
bst.insert(70);
bool has30 = bst.contains(30);      // true
int mn = bst.findMinimum();         // 30
int mx = bst.findMaximum();         // 70

// Priority queue behavior with Min Heap
MinHeap<int> minHeap;
minHeap.insert(5);
minHeap.insert(3);
minHeap.insert(7);
int minval = minHeap.extractRoot(); // 3
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

**Built with ❤️ and Modern C++23**

</div>