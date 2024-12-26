cmake_minimum_required(VERSION 3.30)
project(Algorithms)

set(CMAKE_CXX_STANDARD 26)

add_executable(Algorithms
        main.cpp
        DynamicArray.hpp
        LinkedList.hpp
        Stack.hpp
        DebugTest.cpp
        DebugTest.h
        BinaryTree.hpp
        BinarySearchTree.hpp
)
