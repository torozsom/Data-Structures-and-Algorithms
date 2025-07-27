

#include "LinkedList.hpp"
#include "ThrowingType.hpp"
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>


class LinkedListUnitTest : public ::testing::Test {
  protected:
    void SetUp() override {
        // Setup code that runs before each test
    }

    void TearDown() override {
        // Cleanup code that runs after each test
    }
};



// ==================== CONSTRUCTION TESTS ====================

TEST_F(LinkedListUnitTest, DefaultConstructor) {
    const LinkedList<int> list;

    EXPECT_EQ(list.getSize(), 0);
    EXPECT_TRUE(list.isEmpty());
}


TEST_F(LinkedListUnitTest, ConstructorWithInitialData) {
    int data[] = {1, 2, 3, 4, 5};
    LinkedList list(data, 5);

    EXPECT_EQ(list.getSize(), 5);
    EXPECT_FALSE(list.isEmpty());

    for (int i = 0; i < 5; ++i)
        EXPECT_EQ(list.get(i), data[i]);
}


TEST_F(LinkedListUnitTest, ConstructorWithNullDataThrowsException) {
    EXPECT_THROW(LinkedList<int> list(nullptr, 5), std::invalid_argument);
}


TEST_F(LinkedListUnitTest, ConstructorWithZeroSizeAndNullData) {
    // This should be valid - zero size with null data
    EXPECT_NO_THROW(LinkedList<int> list(nullptr, 0));
    const LinkedList<int> list(nullptr, 0);
    EXPECT_EQ(list.getSize(), 0);
    EXPECT_TRUE(list.isEmpty());
}


TEST_F(LinkedListUnitTest, CopyConstructor) {
    int data[] = {1, 2, 3};
    LinkedList original(data, 3);
    LinkedList copy(original);

    EXPECT_EQ(copy.getSize(), original.getSize());

    for (std::size_t i = 0; i < copy.getSize(); ++i)
        EXPECT_EQ(copy.get(i), original.get(i));

    // Verify deep copy - modify original shouldn't affect copy
    original.addLast(999);
    EXPECT_NE(copy.getSize(), original.getSize());
}


TEST_F(LinkedListUnitTest, CopyConstructorWithEmptyList) {
    const LinkedList<int> original;
    const LinkedList copy(original);

    EXPECT_EQ(copy.getSize(), 0);
    EXPECT_TRUE(copy.isEmpty());
}


TEST_F(LinkedListUnitTest, MoveConstructor) {
    int data[] = {1, 2, 3};
    LinkedList original(data, 3);
    const std::size_t original_size = original.getSize();

    LinkedList moved(std::move(original));

    EXPECT_EQ(moved.getSize(), original_size);
    EXPECT_EQ(original.getSize(), 0);
    EXPECT_TRUE(original.isEmpty());

    for (int i = 0; i < 3; ++i)
        EXPECT_EQ(moved.get(i), data[i]);
}



// ==================== ASSIGNMENT OPERATOR TESTS ====================

TEST_F(LinkedListUnitTest, CopyAssignmentOperator) {
    int data[] = {1, 2, 3};
    LinkedList original(data, 3);
    LinkedList<int> copy;

    copy = original;

    EXPECT_EQ(copy.getSize(), original.getSize());

    for (std::size_t i = 0; i < copy.getSize(); ++i)
        EXPECT_EQ(copy.get(i), original.get(i));
}


TEST_F(LinkedListUnitTest, CopyAssignmentOperatorSelfAssignment) {
    int data[] = {1, 2, 3};
    LinkedList list(data, 3);

    list = list; // Self assignment

    EXPECT_EQ(list.getSize(), 3);
    for (int i = 0; i < 3; ++i)
        EXPECT_EQ(list.get(i), data[i]);
}


TEST_F(LinkedListUnitTest, MoveAssignmentOperator) {
    int data[] = {1, 2, 3};
    LinkedList original(data, 3);
    LinkedList<int> moved;

    const std::size_t original_size = original.getSize();

    moved = std::move(original);

    EXPECT_EQ(moved.getSize(), original_size);
    EXPECT_EQ(original.getSize(), 0);
    EXPECT_TRUE(original.isEmpty());
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wself-move"
TEST_F(LinkedListUnitTest, MoveAssignmentOperatorSelfAssignment) {
    int data[] = {1, 2, 3};
    LinkedList list(data, 3);

    list = std::move(list); // Self assignment

    EXPECT_EQ(list.getSize(), 3);
    for (int i = 0; i < 3; ++i)
        EXPECT_EQ(list.get(i), data[i]);
}
#pragma GCC diagnostic pop



// ==================== INSERTION TESTS ====================

TEST_F(LinkedListUnitTest, AddLast) {
    LinkedList<int> list;

    list.addLast(10);
    EXPECT_EQ(list.getSize(), 1);
    EXPECT_EQ(list.get(0), 10);

    list.addLast(20);
    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);
}


TEST_F(LinkedListUnitTest, AddFirst) {
    LinkedList<int> list;

    list.addFirst(10);
    EXPECT_EQ(list.getSize(), 1);
    EXPECT_EQ(list.get(0), 10);

    list.addFirst(5);
    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.get(0), 5);
    EXPECT_EQ(list.get(1), 10);
}


TEST_F(LinkedListUnitTest, InsertAtValidIndex) {
    LinkedList<int> list;
    list.addLast(10);
    list.addLast(30);

    list.insert(1, 20);

    EXPECT_EQ(list.getSize(), 3);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);
    EXPECT_EQ(list.get(2), 30);
}


TEST_F(LinkedListUnitTest, InsertAtBeginning) {
    LinkedList<int> list;
    list.addLast(20);
    list.addLast(30);

    list.insert(0, 10);

    EXPECT_EQ(list.getSize(), 3);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);
    EXPECT_EQ(list.get(2), 30);
}


TEST_F(LinkedListUnitTest, InsertAtEnd) {
    LinkedList<int> list;
    list.addLast(10);
    list.addLast(20);

    list.insert(2, 30);

    EXPECT_EQ(list.getSize(), 3);
    EXPECT_EQ(list.get(2), 30);
}


TEST_F(LinkedListUnitTest, InsertInMiddleOptimizedTraversal) {
    LinkedList<int> list;
    // Create a list with 10 elements: [0, 10, 20, 30, 40, 50, 60, 70, 80, 90]
    for (int i = 0; i < 10; ++i)
        list.addLast(i * 10);

    // Test 1: Insert near beginning (index 2, should traverse from head)
    const std::size_t original_size = list.getSize();
    list.insert(2, 999);
    EXPECT_EQ(list.get(2), 999);
    EXPECT_EQ(list.getSize(), original_size + 1);

    // Verify the elements around the insertion point
    EXPECT_EQ(list.get(1), 10); // Element before insertion
    EXPECT_EQ(list.get(3), 20); // Element that was shifted right

    // Test 2: Insert near end (should traverse from tail)
    // Insert at second-to-last position
    const std::size_t current_size = list.getSize();
    const std::size_t near_end_index = current_size - 2;

    list.insert(near_end_index, 888);
    EXPECT_EQ(list.get(near_end_index), 888);
    EXPECT_EQ(list.getSize(), current_size + 1);
}


TEST_F(LinkedListUnitTest, InsertAtInvalidIndex) {
    LinkedList<int> list;
    list.addLast(10);

    EXPECT_THROW(list.insert(2, 20), std::out_of_range);
}



// ==================== REMOVAL TESTS ====================

TEST_F(LinkedListUnitTest, RemoveFirst) {
    int data[] = {10, 20, 30};
    LinkedList list(data, 3);

    list.removeFirst();

    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.get(0), 20);
    EXPECT_EQ(list.get(1), 30);
}


TEST_F(LinkedListUnitTest, RemoveFirstFromSingleElement) {
    LinkedList<int> list;
    list.addLast(42);

    list.removeFirst();

    EXPECT_EQ(list.getSize(), 0);
    EXPECT_TRUE(list.isEmpty());
}


TEST_F(LinkedListUnitTest, RemoveFirstFromEmptyList) {
    LinkedList<int> list;

    // Should not throw or crash
    EXPECT_NO_THROW(list.removeFirst());
    EXPECT_EQ(list.getSize(), 0);
}


TEST_F(LinkedListUnitTest, RemoveLast) {
    int data[] = {10, 20, 30};
    LinkedList list(data, 3);

    list.removeLast();

    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);
}


TEST_F(LinkedListUnitTest, RemoveLastFromSingleElement) {
    LinkedList<int> list;
    list.addLast(42);

    list.removeLast();

    EXPECT_EQ(list.getSize(), 0);
    EXPECT_TRUE(list.isEmpty());
}


TEST_F(LinkedListUnitTest, RemoveLastFromEmptyList) {
    LinkedList<int> list;

    // Should not throw or crash
    EXPECT_NO_THROW(list.removeLast());
    EXPECT_EQ(list.getSize(), 0);
}


TEST_F(LinkedListUnitTest, RemoveAt) {
    int data[] = {10, 20, 30, 40};
    LinkedList list(data, 4);

    list.removeAt(1);

    EXPECT_EQ(list.getSize(), 3);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 30);
    EXPECT_EQ(list.get(2), 40);
}


TEST_F(LinkedListUnitTest, RemoveAtFirstIndex) {
    int data[] = {10, 20, 30};
    LinkedList list(data, 3);

    list.removeAt(0);

    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.get(0), 20);
    EXPECT_EQ(list.get(1), 30);
}


TEST_F(LinkedListUnitTest, RemoveAtLastIndex) {
    int data[] = {10, 20, 30};
    LinkedList list(data, 3);

    list.removeAt(2);

    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);
}


TEST_F(LinkedListUnitTest, RemoveAtOptimizedTraversal) {
    LinkedList<int> list;
    // Create a list with 10 elements
    for (int i = 0; i < 10; ++i)
        list.addLast(i * 10);

    // Remove from first half (should traverse from head)
    list.removeAt(2);
    EXPECT_EQ(list.getSize(), 9);

    // Remove from second half (should traverse from tail)
    list.removeAt(6); // This would be index 7 in original 10-element list
    EXPECT_EQ(list.getSize(), 8);
}


TEST_F(LinkedListUnitTest, RemoveAtInvalidIndex) {
    LinkedList<int> list;
    list.addLast(10);

    EXPECT_THROW(list.removeAt(1), std::out_of_range);
    EXPECT_THROW(list.removeAt(10), std::out_of_range);
}


TEST_F(LinkedListUnitTest, RemoveAtFromEmptyList) {
    LinkedList<int> list;

    EXPECT_THROW(list.removeAt(0), std::out_of_range);
}


TEST_F(LinkedListUnitTest, RemoveByValue) {
    int data[] = {10, 20, 30, 20, 40};
    LinkedList list(data, 5);

    list.remove(20); // Should remove first occurrence

    EXPECT_EQ(list.getSize(), 4);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 30);
    EXPECT_EQ(list.get(2), 20); // Second 20 should remain
    EXPECT_EQ(list.get(3), 40);
}


TEST_F(LinkedListUnitTest, RemoveByValueFromHead) {
    int data[] = {10, 20, 30};
    LinkedList list(data, 3);

    list.remove(10);

    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.get(0), 20);
    EXPECT_EQ(list.get(1), 30);
}


TEST_F(LinkedListUnitTest, RemoveByValueFromTail) {
    int data[] = {10, 20, 30};
    LinkedList list(data, 3);

    list.remove(30);

    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);
}


TEST_F(LinkedListUnitTest, RemoveByValueNotFound) {
    int data[] = {10, 20, 30};
    LinkedList list(data, 3);

    list.remove(999); // Element not in list

    EXPECT_EQ(list.getSize(), 3); // Size should remain unchanged
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);
    EXPECT_EQ(list.get(2), 30);
}


TEST_F(LinkedListUnitTest, RemoveFromEmptyList) {
    LinkedList<int> list;

    // Should not throw or crash
    EXPECT_NO_THROW(list.remove(42));
    EXPECT_EQ(list.getSize(), 0);
}



// ==================== ACCESS TESTS ====================

TEST_F(LinkedListUnitTest, GetValidIndex) {
    int data[] = {10, 20, 30};
    LinkedList list(data, 3);

    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);
    EXPECT_EQ(list.get(2), 30);
}


TEST_F(LinkedListUnitTest, GetOptimizedTraversal) {
    LinkedList<int> list;
    // Create a list with 10 elements
    for (int i = 0; i < 10; ++i)
        list.addLast(i * 10);

    // Access near beginning (should traverse from head)
    EXPECT_EQ(list.get(2), 20);
    // Access near end (should traverse from tail)
    EXPECT_EQ(list.get(8), 80);
}


TEST_F(LinkedListUnitTest, GetInvalidIndex) {
    LinkedList<int> list;
    list.addLast(10);

    EXPECT_THROW(list.get(1), std::out_of_range);
    EXPECT_THROW(list.get(10), std::out_of_range);
}


TEST_F(LinkedListUnitTest, OperatorBracket) {
    int data[] = {10, 20, 30};
    LinkedList list(data, 3);

    EXPECT_EQ(list[0], 10);
    EXPECT_EQ(list[1], 20);
    EXPECT_EQ(list[2], 30);

    // Test modification through operator[]
    list[1] = 99;
    EXPECT_EQ(list[1], 99);
}


TEST_F(LinkedListUnitTest, OperatorBracketConst) {
    int data[] = {10, 20, 30};
    const LinkedList list(data, 3);

    EXPECT_EQ(list[0], 10);
    EXPECT_EQ(list[1], 20);
    EXPECT_EQ(list[2], 30);
}


// ==================== CLEAR TESTS ====================

TEST_F(LinkedListUnitTest, Clear) {
    int data[] = {10, 20, 30, 40, 50};
    LinkedList list(data, 5);

    list.clear();

    EXPECT_EQ(list.getSize(), 0);
    EXPECT_TRUE(list.isEmpty());
}


TEST_F(LinkedListUnitTest, ClearEmptyList) {
    LinkedList<int> list;

    list.clear();

    EXPECT_EQ(list.getSize(), 0);
    EXPECT_TRUE(list.isEmpty());
}


// ==================== UTILITY FUNCTION TESTS ====================

TEST_F(LinkedListUnitTest, IsEmpty) {
    LinkedList<int> list;

    EXPECT_TRUE(list.isEmpty());

    list.addLast(10);
    EXPECT_FALSE(list.isEmpty());

    list.removeFirst();
    EXPECT_TRUE(list.isEmpty());
}



// ==================== PRINT FUNCTION TESTS ====================

TEST_F(LinkedListUnitTest, PrintForward) {
    int data[] = {10, 20, 30};
    const LinkedList list(data, 3);

    // Capture stdout
    const std::ostringstream output;
    std::streambuf* orig = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    list.printForward();

    // Restore stdout
    std::cout.rdbuf(orig);

    EXPECT_EQ(output.str(), "10 20 30 \n");
}

TEST_F(LinkedListUnitTest, PrintBackward) {
    int data[] = {10, 20, 30};
    const LinkedList list(data, 3);

    // Capture stdout
    const std::ostringstream output;
    std::streambuf* orig = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    list.printBackward();

    // Restore stdout
    std::cout.rdbuf(orig);

    EXPECT_EQ(output.str(), "30 20 10 \n");
}


TEST_F(LinkedListUnitTest, PrintEmptyList) {
    const LinkedList<int> list;

    // Capture stdout
    const std::ostringstream output;
    std::streambuf* orig = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    list.printForward();

    // Restore stdout
    std::cout.rdbuf(orig);

    EXPECT_EQ(output.str(), "\n");
}



// ==================== DIFFERENT DATA TYPES TESTS ====================

TEST_F(LinkedListUnitTest, StringType) {
    LinkedList<std::string> list;

    list.addLast("Hello");
    list.addLast("World");

    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.get(0), "Hello");
    EXPECT_EQ(list.get(1), "World");
}


TEST_F(LinkedListUnitTest, CustomObjectType) {
    struct Point {
        int x, y;
        explicit Point(int x = 0, int y = 0) : x(x), y(y) {}
        bool operator==(const Point& other) const {
            return x == other.x && y == other.y;
        }
    };

    LinkedList<Point> list;
    list.addLast(Point(1, 2));
    list.addLast(Point(3, 4));

    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.get(0), Point(1, 2));
    EXPECT_EQ(list.get(1), Point(3, 4));
}



// ==================== STRESS TESTS ====================

TEST_F(LinkedListUnitTest, LargeNumberOfInsertions) {
    LinkedList<int> list;
    constexpr int num_elements = 1000;

    for (int i = 0; i < num_elements; ++i)
        list.addLast(i);

    EXPECT_EQ(list.getSize(), num_elements);

    for (int i = 0; i < num_elements; ++i)
        EXPECT_EQ(list.get(i), i);
}


TEST_F(LinkedListUnitTest, AlternatingInsertionAndRemoval) {
    LinkedList<int> list;

    // Pattern: insert 2, remove 1, repeat
    for (int i = 0; i < 100; ++i) {
        list.addLast(i * 2);
        list.addLast(i * 2 + 1);
        if (list.getSize() > 1)
            list.removeFirst();
    }

    EXPECT_GT(list.getSize(), 0);
    // Verify list is still in valid state
    for (std::size_t i = 0; i < list.getSize() - 1; ++i)
        EXPECT_LT(list.get(i), list.get(i + 1));
}



// ==================== BIDIRECTIONAL INTEGRITY TESTS ====================

TEST_F(LinkedListUnitTest, BidirectionalIntegrityAfterInsertions) {
    LinkedList<int> list;

    // Add elements in various ways
    list.addLast(10);
    list.addFirst(5);
    list.insert(1, 7);
    list.addLast(15);

    // Test forward traversal
    EXPECT_EQ(list[0], 5);
    EXPECT_EQ(list[1], 7);
    EXPECT_EQ(list[2], 10);
    EXPECT_EQ(list[3], 15);

    // Test backward traversal by accessing from the end
    EXPECT_EQ(list[3], 15);
    EXPECT_EQ(list[2], 10);
    EXPECT_EQ(list[1], 7);
    EXPECT_EQ(list[0], 5);
}


TEST_F(LinkedListUnitTest, BidirectionalIntegrityAfterRemovals) {
    int data[] = {1, 2, 3, 4, 5};
    LinkedList list(data, 5);

    list.removeAt(2); // Remove middle element
    list.removeFirst();
    list.removeLast();

    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list[0], 2);
    EXPECT_EQ(list[1], 4);
}



// ==================== MEMORY MANAGEMENT TESTS ====================

TEST_F(LinkedListUnitTest, NoMemoryLeaksOnExceptions) {
    // This test ensures that if an exception occurs during construction,
    // partial data is cleaned up properly

    ThrowingType::reset();
    ThrowingType::should_throw = true;

    // This should throw after adding 2 elements
    EXPECT_THROW(
        {
            LinkedList<ThrowingType> list;
            for (int i = 0; i < 5; ++i)
                list.addLast(ThrowingType(i));
        },
        std::runtime_error);

    // All constructed objects should be cleaned up
    EXPECT_EQ(ThrowingType::construction_count, 0);

    ThrowingType::reset();
}
