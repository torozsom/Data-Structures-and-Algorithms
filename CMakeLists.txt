cmake_minimum_required(VERSION 3.30)
project(Algorithms)

set(CMAKE_CXX_STANDARD 26)

# Main executable (your application)
add_executable(algorithms_main
        src/main/main.cpp
        src/main/data_structures/DynamicArray.hpp
        src/main/data_structures/LinkedList.hpp
        src/main/data_structures/Stack.hpp
        src/main/data_structures/BinaryTree.hpp
        src/main/data_structures/BinarySearchTree.hpp
        src/main/data_structures/MinHeap.hpp
        src/main/data_structures/Heap.hpp
        src/main/data_structures/MaxHeap.hpp
)

target_include_directories(algorithms_main PRIVATE src/main/data_structures)

# Test executable (your tests)
add_executable(algorithms_test
        src/test/test.cpp
        src/test/data_structures/Tests.h
        src/test/data_structures/BinaryTreeTest.cpp
        src/test/data_structures/DynamicArrayTest.cpp
        src/test/data_structures/LinkedListTest.cpp
        src/test/data_structures/StackTest.cpp
        src/test/data_structures/BinarySearchTreeTest.cpp
        src/test/data_structures/MinHeapTest.cpp
        src/test/data_structures/MaxHeapTest.cpp
        src/main/data_structures/DynamicArray.hpp
        src/main/data_structures/LinkedList.hpp
        src/main/data_structures/Stack.hpp
        src/main/data_structures/BinaryTree.hpp
        src/main/data_structures/BinarySearchTree.hpp
        src/main/data_structures/MinHeap.hpp
        src/main/data_structures/Heap.hpp
        src/main/data_structures/MaxHeap.hpp
)

target_include_directories(algorithms_test PRIVATE
        src/main/data_structures
        src/test/data_structures
)
