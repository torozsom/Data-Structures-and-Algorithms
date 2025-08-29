cmake_minimum_required(VERSION 3.28)
project(Algorithms)

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
        src/main/model/data_structures/DynamicArray.hpp
        src/main/model/data_structures/LinkedList.hpp
        src/main/model/data_structures/Stack.hpp
        src/main/model/data_structures/BinaryTree.hpp
        src/main/model/data_structures/BinarySearchTree.hpp
        src/main/model/data_structures/MinHeap.hpp
        src/main/model/data_structures/Heap.hpp
        src/main/model/data_structures/MaxHeap.hpp
        src/main/model/data_structures/Queue.hpp

        src/main/model/algorithms/DynamicArrayAlgorithms.hpp
        src/main/view/ui/array/ArrayWidget.hpp
        src/main/view/ui/array/LinearSearchAnimator.hpp
        src/main/view/ui/array/BinarySearchAnimator.hpp
        src/main/view/ui/array/SearchAnimator.hpp
        src/main/view/ui/array/ArrayAnimations.cpp
        src/main/view/ui/array/ArrayAnimations.h
        src/main/view/ui/MainWindow.hpp
        src/main/controller/controls.cpp
        src/main/controller/controls.h
)

target_include_directories(algorithms_main PRIVATE
        src/main/model/data_structures
        src/main/model/algorithms
        src/main/view/ui
        src/main/view/ui/array
        src/main/controller
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
        src/main/model/data_structures/DynamicArray.hpp
        src/main/model/data_structures/LinkedList.hpp
        src/main/model/data_structures/Stack.hpp
        src/main/model/data_structures/BinaryTree.hpp
        src/main/model/data_structures/BinarySearchTree.hpp
        src/main/model/data_structures/MinHeap.hpp
        src/main/model/data_structures/Heap.hpp
        src/main/model/data_structures/MaxHeap.hpp
        src/main/model/data_structures/Queue.hpp
        # Test utilities
        src/test/data_structures/utilities/ThrowingType.hpp
        src/test/data_structures/utilities/Record.hpp
)

target_include_directories(data_struct_unit_tests PRIVATE
        src/main/model/data_structures
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
        src/main/model/data_structures/DynamicArray.hpp
        src/main/model/data_structures/LinkedList.hpp
        src/main/model/data_structures/Stack.hpp
        src/main/model/data_structures/BinaryTree.hpp
        src/main/model/data_structures/BinarySearchTree.hpp
        src/main/model/data_structures/MinHeap.hpp
        src/main/model/data_structures/Heap.hpp
        src/main/model/data_structures/MaxHeap.hpp
        src/main/model/data_structures/Queue.hpp
)

target_include_directories(algorithms_unit_tests PRIVATE
        src/main/model/data_structures
        src/main/model/algorithms
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