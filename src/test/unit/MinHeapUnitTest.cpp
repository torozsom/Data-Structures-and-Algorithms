#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "MinHeap.hpp"


class MinHeapUnitTest : public ::testing::Test {
  protected:
    void SetUp() override {
        // Setup code that runs before each test
    }

    void TearDown() override {
        // Cleanup code that runs after each test
    }
};


TEST_F(MinHeapUnitTest, NewHeapShouldBeEmpty) {
    const MinHeap<int> heap;
    EXPECT_EQ(heap.size(), 0);
    EXPECT_TRUE(heap.isEmpty());
    EXPECT_TRUE(heap.isValidHeap());
}


TEST_F(MinHeapUnitTest, InsertShouldMaintainMinHeapProperty) {
    MinHeap<int> heap;
    heap.insert(5);
    heap.insert(3);
    heap.insert(7);
    heap.insert(1);
    heap.insert(4);

    EXPECT_EQ(heap.size(), 5);
    EXPECT_TRUE(heap.isValidHeap());
    EXPECT_EQ(heap.peekRoot(), 1); // Minimum element should be at root
}


TEST_F(MinHeapUnitTest, ExtractMinShouldRemoveSmallestElement) {
    MinHeap<int> heap;
    std::vector<int> values = {5, 3, 7, 1, 4, 9, 2};
    for (int val : values)
        heap.insert(val);

    std::sort(values.begin(), values.end());

    for (int expectedMin : values) {
        EXPECT_EQ(heap.extractRoot(), expectedMin);
        EXPECT_TRUE(heap.isValidHeap());
    }

    EXPECT_TRUE(heap.isEmpty());
}


TEST_F(MinHeapUnitTest, ArrayConstructorShouldCreateValidMinHeap) {
    const int values[] = {5, 3, 7, 1, 4, 9, 2};
    const MinHeap heap(values, 7);

    EXPECT_EQ(heap.size(), 7);
    EXPECT_TRUE(heap.isValidHeap());
    EXPECT_EQ(heap.peekRoot(), 1); // Minimum should be at root
}


TEST_F(MinHeapUnitTest, CopyConstructorShouldCreateDeepCopy) {
    MinHeap<int> original;
    original.insert(10);
    original.insert(5);
    original.insert(15);
    original.insert(3);

    const MinHeap copy(original);
    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(copy.peekRoot(), original.peekRoot());
    EXPECT_TRUE(copy.isValidHeap());

    // Modify original to verify deep copy
    original.insert(2);
    EXPECT_NE(copy.size(), original.size());
    EXPECT_EQ(copy.peekRoot(), 3); // Copy should still have 3 as min
}


TEST_F(MinHeapUnitTest, MoveConstructorShouldTransferOwnership) {
    MinHeap<int> original;
    original.insert(10);
    original.insert(5);
    original.insert(15);

    const MinHeap moved(std::move(original));
    EXPECT_TRUE(original.isEmpty());
    EXPECT_EQ(original.size(), 0);
    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved.peekRoot(), 5);
    EXPECT_TRUE(moved.isValidHeap());
}


TEST_F(MinHeapUnitTest, AssignmentOperatorShouldCreateDeepCopy) {
    MinHeap<int> original;
    original.insert(10);
    original.insert(5);
    original.insert(15);
    original.insert(3);

    MinHeap<int> copy;
    copy = original;

    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(copy.peekRoot(), original.peekRoot());
    EXPECT_TRUE(copy.isValidHeap());

    // Modify original to verify deep copy
    original.extractRoot();
    EXPECT_NE(copy.size(), original.size());
    EXPECT_EQ(copy.peekRoot(), 3); // Copy should still have original min
}


TEST_F(MinHeapUnitTest, MoveAssignmentShouldTransferOwnership) {
    MinHeap<int> original;
    original.insert(10);
    original.insert(5);
    original.insert(15);

    MinHeap<int> moved;
    moved = std::move(original);

    EXPECT_TRUE(original.isEmpty());
    EXPECT_EQ(original.size(), 0);
    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved.peekRoot(), 5);
    EXPECT_TRUE(moved.isValidHeap());
}


TEST_F(MinHeapUnitTest, PeekOnEmptyHeapShouldThrow) {
    const MinHeap<int> heap;
    EXPECT_THROW(heap.peekRoot(), std::out_of_range);
}


TEST_F(MinHeapUnitTest, ExtractOnEmptyHeapShouldThrow) {
    MinHeap<int> heap;
    EXPECT_THROW(heap.extractRoot(), std::out_of_range);
}


TEST_F(MinHeapUnitTest, SingleElementHeapShouldWorkCorrectly) {
    MinHeap<int> heap;
    heap.insert(42);

    EXPECT_EQ(heap.size(), 1);
    EXPECT_EQ(heap.peekRoot(), 42);
    EXPECT_TRUE(heap.isValidHeap());

    EXPECT_EQ(heap.extractRoot(), 42);
    EXPECT_TRUE(heap.isEmpty());
}


TEST_F(MinHeapUnitTest, DuplicateElementsShouldBeHandledCorrectly) {
    MinHeap<int> heap;
    heap.insert(5);
    heap.insert(5);
    heap.insert(5);
    heap.insert(3);
    heap.insert(7);

    EXPECT_EQ(heap.size(), 5);
    EXPECT_TRUE(heap.isValidHeap());
    EXPECT_EQ(heap.peekRoot(), 3);

    // Extract all elements
    EXPECT_EQ(heap.extractRoot(), 3);
    EXPECT_EQ(heap.extractRoot(), 5);
    EXPECT_EQ(heap.extractRoot(), 5);
    EXPECT_EQ(heap.extractRoot(), 5);
    EXPECT_EQ(heap.extractRoot(), 7);
    EXPECT_TRUE(heap.isEmpty());
}


TEST_F(MinHeapUnitTest, ShouldWorkWithNonTrivialTypes) {
    MinHeap<std::string> heap;
    heap.insert("zebra");
    heap.insert("apple");
    heap.insert("orange");
    heap.insert("banana");

    EXPECT_EQ(heap.size(), 4);
    EXPECT_TRUE(heap.isValidHeap());
    EXPECT_EQ(heap.peekRoot(), "apple"); // Lexicographically smallest

    // Extract in ascending order
    EXPECT_EQ(heap.extractRoot(), "apple");
    EXPECT_EQ(heap.extractRoot(), "banana");
    EXPECT_EQ(heap.extractRoot(), "orange");
    EXPECT_EQ(heap.extractRoot(), "zebra");
}


TEST_F(MinHeapUnitTest, LargeHeapShouldMaintainProperty) {
    MinHeap<int> heap;
    constexpr int numElements = 100;

    // Insert elements in reverse order
    for (int i = numElements; i >= 1; --i) {
        heap.insert(i);
    }

    EXPECT_EQ(heap.size(), numElements);
    EXPECT_TRUE(heap.isValidHeap());
    EXPECT_EQ(heap.peekRoot(), 1);

    // Extract all elements - should come out in ascending order
    for (int i = 1; i <= numElements; ++i) {
        EXPECT_EQ(heap.extractRoot(), i);
        EXPECT_TRUE(heap.isValidHeap());
    }

    EXPECT_TRUE(heap.isEmpty());
}


TEST_F(MinHeapUnitTest, HeightShouldBeLogarithmic) {
    MinHeap<int> heap;

    // Insert powers of 2 to test height
    for (int i = 1; i <= 15; ++i) {
        heap.insert(i);
    }

    // Height should be log2(15) + 1 = 4
    EXPECT_EQ(heap.getHeight(), 4);
    EXPECT_TRUE(heap.isValidHeap());
}


TEST_F(MinHeapUnitTest, EmptyHeapOperationsShouldWork) {
    MinHeap<int> heap;

    EXPECT_TRUE(heap.isEmpty());
    EXPECT_EQ(heap.size(), 0);
    EXPECT_EQ(heap.getHeight(), 0);
    EXPECT_TRUE(heap.isValidHeap());

    // These should throw
    EXPECT_THROW(heap.peekRoot(), std::out_of_range);
    EXPECT_THROW(heap.extractRoot(), std::out_of_range);
}
