cmake_minimum_required(VERSION 3.30)
project(Algorithms)

set(CMAKE_CXX_STANDARD 26)

add_executable(Algorithms
        main.cpp
        DynamicArray.hpp
        LinkedList.hpp
        Stack.hpp
        Tests.h
        BinaryTree.hpp
        BinarySearchTree.hpp
        BinaryTreeTest.cpp
        DynamicArrayTest.cpp
        LinkedListTest.cpp
        StackTest.cpp
        BinarySearchTreeTest.cpp
        MinHeap.hpp
        Heap.hpp
        MinHeapTest.cpp
        MaxHeap.hpp
        MaxHeapTest.cpp
)
