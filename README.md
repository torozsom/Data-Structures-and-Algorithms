# Data Structures & Algorithms Visualization

<div align="center">

![C++23](https://img.shields.io/badge/C++-23-blue.svg)
![Qt6](https://img.shields.io/badge/Qt-6-brightgreen.svg)
![CMake](https://img.shields.io/badge/build-CMake-informational.svg)
![Tests](https://img.shields.io/badge/tests-Google%20Test-blue.svg)

</div>

## Overview

This project provides a modern C++23 playground for fundamental data structures and their algorithms.  A Qt-based desktop application visualizes the behaviour of the structures while a standalone library and unit-test suite keep the implementation clean and reusable.

* **Language & Tooling** – C++23, CMake, Qt6, Google Test
* **Design Goals** – efficiency, clarity, and ease of extension

## Data Structure Efficiency

The core library implements cache-friendly containers and trees with well-defined complexity guarantees.  Highlights include:

| Structure | Key Operations | Time Complexity | Space |
|-----------|----------------|-----------------|-------|
| Dynamic Array | access, push/pop back | O(1) amortised | O(n) |
| Linked List | insert/remove ends | O(1) | O(n) |
| Stack | push, pop, top | O(1) | O(n) |
| Queue | enqueue, dequeue, front/back | O(1) amortised | O(n) |
| Binary Search Tree | insert, search, delete | O(h) | O(n) |
| Min/Max Heap | insert, extract-root, peek | O(log n) | O(n) |

(\* `h` – height of the tree; balanced case `O(log n)`.)  See [`src/main/core/data_structures/README.md`](src/main/core/data_structures/README.md) for detailed analysis.

## Architecture & Patterns

The codebase follows a layered architecture:

* **`src/main/core`** – header-only templates for containers and algorithms.
* **`src/main/ui`** – Qt6 user interface following an MVVM-like pattern (`controller/`, `view/`, `viewmodel/`).
* **`src/main/app`** – thin entry point wiring Qt and the core library.
* **`src/test`** – Google Test suites verifying behaviour and exception safety.

Separation of concerns keeps the core library independent from the UI, enabling reuse in other projects.  RAII, templates and strongly-typed interfaces provide maintainability and safe resource management.

## Project Structure

```
Data-Structures-and-Algorithms/
├── CMakeLists.txt
├── README.md
└── src/
    ├── main/
    │   ├── app/
    │   ├── core/
    │   │   ├── algorithms/
    │   │   └── data_structures/
    │   └── ui/
    │       ├── controller/
    │       ├── view/
    │       └── viewmodel/
    └── test/
        ├── algorithms/
        └── data_structures/
```

## Extensibility & Maintainability

* **Header-only templates** – easily include and specialise for custom types.
* **Clean separation** between data structures, algorithms, and UI components keeps dependencies minimal.
* **Comprehensive unit tests** cover edge cases and document intended behaviour, reducing regression risk.
* **Consistent coding style and docs** (Doxygen-friendly comments) aid contributors in navigating and extending the code.

## Building & Testing

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

Unit tests use Google Test and cover both algorithms and data structures.  The Qt6 GUI target demonstrates the structures visually.

---

Enjoy exploring and extending this foundation for data structures and algorithms!  Contributions and feedback are welcome.
