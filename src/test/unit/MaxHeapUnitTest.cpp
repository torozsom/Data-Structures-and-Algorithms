#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "MaxHeap.hpp"


class MaxHeapUnitTest : public ::testing::Test {
  protected:
    void SetUp() override {
        // Setup code that runs before each test
    }

    void TearDown() override {
        // Cleanup code that runs after each test
    }
};


TEST_F(MaxHeapUnitTest, NewHeapShouldBeEmpty) {
    const MaxHeap<int> heap;
    EXPECT_EQ(heap.size(), 0);
    EXPECT_TRUE(heap.isEmpty());
    EXPECT_TRUE(heap.isValidHeap());
}


TEST_F(MaxHeapUnitTest, InsertShouldMaintainMaxHeapProperty) {
    MaxHeap<int> heap;
    heap.insert(5);
    heap.insert(3);
    heap.insert(7);
    heap.insert(1);
    heap.insert(4);

    EXPECT_EQ(heap.size(), 5);
    EXPECT_TRUE(heap.isValidHeap());
    EXPECT_EQ(heap.peekRoot(), 7); // Maximum element should be at root
}


TEST_F(MaxHeapUnitTest, ExtractMaxShouldRemoveLargestElement) {
    MaxHeap<int> heap;
    std::vector<int> values = {5, 3, 7, 1, 4, 9, 2};
    for (int val : values)
        heap.insert(val);

    std::sort(values.begin(), values.end(), std::greater<int>());

    for (int expectedMax : values) {
        EXPECT_EQ(heap.extractRoot(), expectedMax);
        EXPECT_TRUE(heap.isValidHeap());
    }

    EXPECT_TRUE(heap.isEmpty());
}


TEST_F(MaxHeapUnitTest, ArrayConstructorShouldCreateValidMaxHeap) {
    const int values[] = {5, 3, 7, 1, 4, 9, 2};
    const MaxHeap heap(values, 7);

    EXPECT_EQ(heap.size(), 7);
    EXPECT_TRUE(heap.isValidHeap());
    EXPECT_EQ(heap.peekRoot(), 9); // Maximum should be at root
}


TEST_F(MaxHeapUnitTest, CopyConstructorShouldCreateDeepCopy) {
    MaxHeap<int> original;
    original.insert(10);
    original.insert(5);
    original.insert(15);
    original.insert(3);

    const MaxHeap copy(original);
    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(copy.peekRoot(), original.peekRoot());
    EXPECT_TRUE(copy.isValidHeap());

    // Modify original to verify deep copy
    original.insert(20);
    EXPECT_NE(copy.size(), original.size());
    EXPECT_EQ(copy.peekRoot(), 15); // Copy should still have 15 as max
}


TEST_F(MaxHeapUnitTest, MoveConstructorShouldTransferOwnership) {
    MaxHeap<int> original;
    original.insert(10);
    original.insert(5);
    original.insert(15);

    const MaxHeap moved(std::move(original));
    EXPECT_TRUE(original.isEmpty());
    EXPECT_EQ(original.size(), 0);
    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved.peekRoot(), 15);
    EXPECT_TRUE(moved.isValidHeap());
}


TEST_F(MaxHeapUnitTest, AssignmentOperatorShouldCreateDeepCopy) {
    MaxHeap<int> original;
    original.insert(10);
    original.insert(5);
    original.insert(15);
    original.insert(3);

    MaxHeap<int> copy;
    copy = original;

    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(copy.peekRoot(), original.peekRoot());
    EXPECT_TRUE(copy.isValidHeap());

    // Modify original to verify deep copy
    original.extractRoot();
    EXPECT_NE(copy.size(), original.size());
    EXPECT_EQ(copy.peekRoot(), 15); // Copy should still have original max
}


TEST_F(MaxHeapUnitTest, MoveAssignmentShouldTransferOwnership) {
    MaxHeap<int> original;
    original.insert(10);
    original.insert(5);
    original.insert(15);

    MaxHeap<int> moved;
    moved = std::move(original);

    EXPECT_TRUE(original.isEmpty());
    EXPECT_EQ(original.size(), 0);
    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved.peekRoot(), 15);
    EXPECT_TRUE(moved.isValidHeap());
}


TEST_F(MaxHeapUnitTest, PeekOnEmptyHeapShouldThrow) {
    const MaxHeap<int> heap;
    EXPECT_THROW(heap.peekRoot(), std::out_of_range);
}


TEST_F(MaxHeapUnitTest, ExtractOnEmptyHeapShouldThrow) {
    MaxHeap<int> heap;
    EXPECT_THROW(heap.extractRoot(), std::out_of_range);
}


TEST_F(MaxHeapUnitTest, SingleElementHeapShouldWorkCorrectly) {
    MaxHeap<int> heap;
    heap.insert(42);

    EXPECT_EQ(heap.size(), 1);
    EXPECT_EQ(heap.peekRoot(), 42);
    EXPECT_TRUE(heap.isValidHeap());

    EXPECT_EQ(heap.extractRoot(), 42);
    EXPECT_TRUE(heap.isEmpty());
}


TEST_F(MaxHeapUnitTest, DuplicateElementsShouldBeHandledCorrectly) {
    MaxHeap<int> heap;
    heap.insert(5);
    heap.insert(5);
    heap.insert(5);
    heap.insert(3);
    heap.insert(7);

    EXPECT_EQ(heap.size(), 5);
    EXPECT_TRUE(heap.isValidHeap());
    EXPECT_EQ(heap.peekRoot(), 7);

    // Extract all elements
    EXPECT_EQ(heap.extractRoot(), 7);
    EXPECT_EQ(heap.extractRoot(), 5);
    EXPECT_EQ(heap.extractRoot(), 5);
    EXPECT_EQ(heap.extractRoot(), 5);
    EXPECT_EQ(heap.extractRoot(), 3);
    EXPECT_TRUE(heap.isEmpty());
}


TEST_F(MaxHeapUnitTest, ShouldWorkWithNonTrivialTypes) {
    MaxHeap<std::string> heap;
    heap.insert("apple");
    heap.insert("zebra");
    heap.insert("banana");
    heap.insert("orange");

    EXPECT_EQ(heap.size(), 4);
    EXPECT_TRUE(heap.isValidHeap());
    EXPECT_EQ(heap.peekRoot(), "zebra"); // Lexicographically largest

    // Extract in descending order
    EXPECT_EQ(heap.extractRoot(), "zebra");
    EXPECT_EQ(heap.extractRoot(), "orange");
    EXPECT_EQ(heap.extractRoot(), "banana");
    EXPECT_EQ(heap.extractRoot(), "apple");
}


TEST_F(MaxHeapUnitTest, LargeHeapShouldMaintainProperty) {
    MaxHeap<int> heap;
    constexpr int numElements = 100;

    // Insert elements in random order
    for (int i = numElements; i >= 1; --i) {
        heap.insert(i);
    }

    EXPECT_EQ(heap.size(), numElements);
    EXPECT_TRUE(heap.isValidHeap());
    EXPECT_EQ(heap.peekRoot(), numElements);

    // Extract all elements - should come out in descending order
    for (int i = numElements; i >= 1; --i) {
        EXPECT_EQ(heap.extractRoot(), i);
        EXPECT_TRUE(heap.isValidHeap());
    }

    EXPECT_TRUE(heap.isEmpty());
}


TEST_F(MaxHeapUnitTest, HeightShouldBeLogarithmic) {
    MaxHeap<int> heap;

    // Insert powers of 2 to test height
    for (int i = 1; i <= 15; ++i) {
        heap.insert(i);
    }

    // Height should be log2(15) + 1 = 4
    EXPECT_EQ(heap.getHeight(), 4);
    EXPECT_TRUE(heap.isValidHeap());
}
