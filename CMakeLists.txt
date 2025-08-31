cmake_minimum_required(VERSION 3.28)
project(Algorithms LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

enable_testing()

# Find or fetch Google Test
find_package(GTest QUIET)
if (NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.15.2
            GIT_SHALLOW TRUE
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif ()

find_package(Qt6 REQUIRED COMPONENTS Widgets)



# Main executable
add_executable(algorithms_main
        src/main/app/main.cpp
        src/main/app/controls.cpp
        src/main/app/controls.h

        src/main/core/data_structures/DynamicArray.hpp
        src/main/core/data_structures/LinkedList.hpp
        src/main/core/data_structures/Stack.hpp
        src/main/core/data_structures/BinaryTree.hpp
        src/main/core/data_structures/BinarySearchTree.hpp
        src/main/core/data_structures/MinHeap.hpp
        src/main/core/data_structures/Heap.hpp
        src/main/core/data_structures/MaxHeap.hpp
        src/main/core/data_structures/Queue.hpp

        src/main/core/algorithms/DynamicArrayAlgorithms.hpp

        src/main/ui/view/MainWindow.h
        src/main/ui/view/MainWindow.cpp
        src/main/ui/view/MainWindow.ui

        src/main/ui/view/array/ArrayPage.h
        src/main/ui/view/array/ArrayPage.cpp
        src/main/ui/view/array/ArrayPage.ui
        src/main/ui/view/array/ArrayWidget.hpp

        src/main/ui/viewmodel/array/LinearSearchAnimator.hpp
        src/main/ui/viewmodel/array/BinarySearchAnimator.hpp
        src/main/ui/viewmodel/array/SearchAnimator.hpp

        src/main/ui/controller/array/ArrayAnimations.cpp
        src/main/ui/controller/array/ArrayAnimations.h
)


target_include_directories(algorithms_main PRIVATE
        src/main/core
            src/main/core/data_structures
            src/main/core/algorithms
        src/main/ui/assets
        src/main/ui/common
        src/main/ui/controller
            src/main/ui/controller/array
        src/main/ui/view
            src/main/ui/view/array
        src/main/ui/viewmodel
            src/main/ui/viewmodel/array
)


set_target_properties(algorithms_main PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        AUTORRC ON
)


target_link_libraries(algorithms_main PRIVATE Qt6::Widgets)



# Data structures' unit tests executable (Google Test based)
add_executable(data_struct_unit_tests
        # Unit test files
        src/test/data_structures/unit/DynamicArrayUnitTest.cpp
        src/test/data_structures/unit/LinkedListUnitTest.cpp
        src/test/data_structures/unit/QueueUnitTest.cpp
        src/test/data_structures/unit/StackUnitTest.cpp
        src/test/data_structures/unit/BinaryTreeUnitTest.cpp
        src/test/data_structures/unit/BinarySearchTreeUnitTest.cpp
        src/test/data_structures/unit/MinHeapUnitTest.cpp
        src/test/data_structures/unit/MaxHeapUnitTest.cpp
        # Header files (for IDE support)
        src/main/core/data_structures/DynamicArray.hpp
        src/main/core/data_structures/LinkedList.hpp
        src/main/core/data_structures/Stack.hpp
        src/main/core/data_structures/BinaryTree.hpp
        src/main/core/data_structures/BinarySearchTree.hpp
        src/main/core/data_structures/MinHeap.hpp
        src/main/core/data_structures/Heap.hpp
        src/main/core/data_structures/MaxHeap.hpp
        src/main/core/data_structures/Queue.hpp
        # Test utilities
        src/test/data_structures/utilities/ThrowingType.hpp
        src/test/data_structures/utilities/Record.hpp
)


target_include_directories(data_struct_unit_tests PRIVATE
        src/main/core/data_structures
        src/test/data_structures/unit
        src/test/data_structures/utilities
)


target_link_libraries(data_struct_unit_tests
        GTest::gtest
        GTest::gtest_main
)


add_custom_target(run_data_struct_unit_tests
        COMMAND data_struct_unit_tests
        DEPENDS data_struct_unit_tests
        COMMENT "Running unit tests"
)



# Algorithms' unit tests executable (Google Test based)
add_executable(algorithms_unit_tests
        # Unit test files
        src/test/algorithms/unit/DynamicArrayAlgorithmsUnitTest.cpp
        # Header files (for IDE support)
        src/main/core/data_structures/DynamicArray.hpp
        src/main/core/data_structures/LinkedList.hpp
        src/main/core/data_structures/Stack.hpp
        src/main/core/data_structures/BinaryTree.hpp
        src/main/core/data_structures/BinarySearchTree.hpp
        src/main/core/data_structures/MinHeap.hpp
        src/main/core/data_structures/Heap.hpp
        src/main/core/data_structures/MaxHeap.hpp
        src/main/core/data_structures/Queue.hpp
)


target_include_directories(algorithms_unit_tests PRIVATE
        src/main/core/data_structures
        src/main/core/algorithms
        src/test/algorithms/unit
)


target_link_libraries(algorithms_unit_tests
        GTest::gtest
        GTest::gtest_main
)


add_custom_target(run_algorithms_unit_tests
        COMMAND algorithms_unit_tests
        DEPENDS algorithms_unit_tests
        COMMENT "Running unit tests"
)



# Register tests with CTest (moved after target creation)
include(GoogleTest)
gtest_discover_tests(
        data_struct_unit_tests
        algorithms_unit_tests
)


set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_FLAGS_RELEASE "-O3 -fno-fast-math -fno-unsafe-math-optimizations -frounding-math -march=native")
#set(CMAKE_C_FLAGS_DEBUG "-O2 -fno-fast-math -fno-unsafe-math-optimizations -frounding-math -march=native")

message(STATUS "Current build type: ${CMAKE_BUILD_TYPE}")

# Compiler-specific options for better testing
if (MSVC)
    target_compile_options(data_struct_unit_tests PRIVATE /W4)
    target_compile_options(algorithms_unit_tests PRIVATE /W4)
    target_compile_options(algorithms_main PRIVATE /W4)
else ()
    target_compile_options(data_struct_unit_tests PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(algorithms_unit_tests PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(algorithms_main PRIVATE -Wall -Wextra -Wpedantic)
endif ()

# Optional: Code coverage support (for GCC/Clang)
if (CMAKE_BUILD_TYPE MATCHES Debug AND NOT MSVC)
    option(ENABLE_COVERAGE "Enable coverage reporting" ON)
    if (ENABLE_COVERAGE)
        target_compile_options(data_struct_unit_tests PRIVATE --coverage)
        target_link_libraries(data_struct_unit_tests --coverage)
    endif ()
endif ()