cmake_minimum_required(VERSION 3.28)
project(Algorithms)

set(CMAKE_CXX_STANDARD 23)

# Enable testing
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
        src/main/data_structures/Queue.hpp

        src/main/algorithms/DynamicArrayAlgorithms.hpp
)

target_include_directories(algorithms_main PRIVATE src/main/data_structures)

# Unit test executable (Google Test based)
add_executable(unit_tests
        # Unit test files
        src/test/unit/DynamicArrayUnitTest.cpp
        src/test/unit/LinkedListUnitTest.cpp
        src/test/unit/QueueUnitTest.cpp
        src/test/unit/StackUnitTest.cpp
        src/test/unit/BinaryTreeUnitTest.cpp
        src/test/unit/BinarySearchTreeUnitTest.cpp
        src/test/unit/MinHeapUnitTest.cpp
        src/test/unit/MaxHeapUnitTest.cpp
        # Header files (for IDE support)
        src/main/data_structures/DynamicArray.hpp
        src/main/data_structures/LinkedList.hpp
        src/main/data_structures/Stack.hpp
        src/main/data_structures/BinaryTree.hpp
        src/main/data_structures/BinarySearchTree.hpp
        src/main/data_structures/MinHeap.hpp
        src/main/data_structures/Heap.hpp
        src/main/data_structures/MaxHeap.hpp
        src/main/data_structures/Queue.hpp
        # Test utilities
        src/test/utilities/ThrowingType.hpp
        src/test/utilities/Record.hpp
)

target_include_directories(unit_tests PRIVATE
        src/main/data_structures
        src/test/unit
        src/test/utilities
)

target_link_libraries(unit_tests
        GTest::gtest
        GTest::gtest_main
)

# Register tests with CTest (moved after target creation)
include(GoogleTest)
gtest_discover_tests(unit_tests)

# Optional: Add custom test target for convenience
add_custom_target(run_unit_tests
        COMMAND unit_tests
        DEPENDS unit_tests
        COMMENT "Running unit tests"
)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_FLAGS_RELEASE "-O3 -fno-fast-math -fno-unsafe-math-optimizations -frounding-math -march=native")
#set(CMAKE_C_FLAGS_DEBUG "-O2 -fno-fast-math -fno-unsafe-math-optimizations -frounding-math -march=native")

message(STATUS "Current build type: ${CMAKE_BUILD_TYPE}")

# Compiler-specific options for better testing
if (MSVC)
    target_compile_options(unit_tests PRIVATE /W4)
    target_compile_options(algorithms_main PRIVATE /W4)
else ()
    target_compile_options(unit_tests PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(algorithms_main PRIVATE -Wall -Wextra -Wpedantic)
endif ()

# Optional: Code coverage support (for GCC/Clang)
if (CMAKE_BUILD_TYPE MATCHES Debug AND NOT MSVC)
    option(ENABLE_COVERAGE "Enable coverage reporting" ON)
    if (ENABLE_COVERAGE)
        target_compile_options(unit_tests PRIVATE --coverage)
        target_link_libraries(unit_tests --coverage)
    endif ()
endif ()