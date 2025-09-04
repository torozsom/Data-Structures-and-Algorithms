<div align="center">

# 🚀 Advanced Data Structures Library

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
|   **Dynamic Array**    |     [`DynamicArray.hpp`](DynamicArray.hpp)     |    Access<br>Insert/Remove (end)<br>Insert/Remove (arbitrary)       |  O(1)<br>Amortized O(1)<br>O(n)   |       O(n)       |
|    **Linked List**     |       [`LinkedList.hpp`](LinkedList.hpp)       |    Access<br>Insert/Remove (ends)<br>Insert/Remove (middle)         |       O(n)<br>O(1)<br>O(n)        |       O(n)       |
|       **Stack**        |            [`Stack.hpp`](Stack.hpp)            |                         Push<br>Pop<br>Top                          |       O(1)<br>O(1)<br>O(1)        |       O(n)       |
|       **Queue**        |            [`Queue.hpp`](Queue.hpp)            |                 Enqueue<br>Dequeue<br>Front/Back                    |       O(1)<br>O(1)<br>O(1)        |       O(n)       |
|    **Binary Tree**     |       [`BinaryTree.hpp`](BinaryTree.hpp)       |            Insert (level-order)<br>Find/Contains<br>Height          |               O(n)                |       O(n)       |
| **Binary Search Tree** | [`BinarySearchTree.hpp`](BinarySearchTree.hpp) |           Insert<br>Search/Contains<br>Delete<br>Min/Max            | O(h)\*<br>O(h)\*<br>O(h)\*<br>O(h)\* |       O(n)       |
|      **Min Heap**      |          [`MinHeap.hpp`](MinHeap.hpp)          |                  Insert<br>Extract-Min<br>Peek                      |   O(log n)<br>O(log n)<br>O(1)    |       O(n)       |
|      **Max Heap**      |          [`MaxHeap.hpp`](MaxHeap.hpp)          |                  Insert<br>Extract-Max<br>Peek                      |   O(log n)<br>O(log n)<br>O(1)    |       O(n)       |
|      **Hash Map**      |          [`HashMap.hpp`](HashMap.hpp)          |        Insert/Update<br>Access<br>Remove        | Avg O(1)†<br>Avg O(1)†<br>Avg O(1)† |       O(n)       |

\* `h` is the tree height (worst case O(n), balanced case O(log n)). 

† Expected average-case complexity with a well-distributed hash function; worst-case O(n).

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

A First-In-First-Out (FIFO) container backed by a circular buffer built on
top of `DynamicArray`

**Key Features:**

- ✅ Amortized O(1) `enqueue` and `dequeue`
- ✅ Constant-time `front()` and `back()`
- ✅ Automatic capacity management (geometric growth and periodic shrink)
- ✅ Bidirectional iterators for traversal

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

### Hash Map

An open-addressing associative container with linear probing and tombstone handling.

**Key Features:**

- ✅ Average O(1) insertion, access, and removal
- ✅ Configurable load factor with automatic resizing
- ✅ Customizable hash functor with optional randomized seeding for security

**Distinctive Approach:**

- Buckets use aligned storage and `std::launder` to manage non-trivial types
- Power-of-two capacities enable efficient bit-masked indexing
- Tombstones preserve probe sequences for successful lookups after deletions

## 📈 Performance Analysis

### Time Complexity Highlights

| Operation | Dynamic Array | Linked List  | BST (balanced) |   Heap   | Hash Map |
|:---------:|:-------------:|:------------:|:--------------:|:--------:|:--------:|
|  Search   |     O(n)      |     O(n)     |    O(log n)    |   O(n)   | Avg O(1)† |
|  Insert   | O(1)* / O(n) | O(1)* / O(n)|    O(log n)    | O(log n) | Avg O(1)† |
|  Delete   | O(1)* / O(n) | O(1)* / O(n)|    O(log n)    | O(log n) | Avg O(1)† |
|  Min/Max  |     O(n)      |     O(n)     |    O(log n)    |   O(1)   |    N/A    |

\* At the end/beginning of the structure.

† Expected average-case complexity with a well-distributed hash function; worst-case O(n).

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

// Associative storage with open addressing
HashMap<int, std::string> map;
map.insert(1, "one");
map.insert(2, "two");
std::string one = map.at(1);       // "one"
map.remove(2);
```

## 🧪 Testing

Each data structure is rigorously tested with comprehensive test cases in the `src/test` directory:

- **Unit Tests**: Verify individual operations
- **Integration Tests**: Ensure data structures work together correctly
- **Edge Cases**: Test boundary conditions and error handling
- **Performance Tests**: Validate complexity guarantees

## 🚧 Future Roadmap

- **Balanced Trees**: Implement AVL and Red-Black tree balancing algorithms
- **Parallelism**: Explore thread-safe variants of selected data structures
- **Benchmarking**: Add comprehensive performance benchmarking suite
- **Serialization**: Support for persistence and serialization operations

---

<div align="center">

**Built with ❤️ and Modern C++23**

</div>