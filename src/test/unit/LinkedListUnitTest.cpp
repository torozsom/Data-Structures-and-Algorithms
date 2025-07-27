#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "LinkedList.hpp"
#include "ThrowingType.hpp"


class LinkedListUnitTest : public ::testing::Test {
  protected:
    void SetUp() override {
        // Setup code that runs before each test
    }

    void TearDown() override {
        // Cleanup code that runs after each test
    }
};


TEST_F(LinkedListUnitTest, DefaultConstructor) {
    const LinkedList<int> list;

    EXPECT_EQ(list.getSize(), 0);
    EXPECT_TRUE(list.isEmpty());
}


TEST_F(LinkedListUnitTest, ArrayConstructorWithValidData) {
    int data[] = {1, 2, 3, 4, 5};
    const LinkedList<int> list(data, 5);

    EXPECT_EQ(list.getSize(), 5);
    EXPECT_FALSE(list.isEmpty());

    for (std::size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(list.get(i), data[i]);
    }
}


TEST_F(LinkedListUnitTest, ArrayConstructorWithEmptyData) {
    const LinkedList<int> list(nullptr, 0);

    EXPECT_EQ(list.getSize(), 0);
    EXPECT_TRUE(list.isEmpty());
}


TEST_F(LinkedListUnitTest, ArrayConstructorWithNullDataButPositiveSize) {
    EXPECT_THROW(LinkedList<int> list(nullptr, 5), std::invalid_argument);
}


TEST_F(LinkedListUnitTest, CopyConstructor) {
    LinkedList<int> original;
    original.addLast(10);
    original.addLast(20);
    original.addLast(30);

    LinkedList<int> copy(original);

    EXPECT_EQ(copy.getSize(), original.getSize());
    for (std::size_t i = 0; i < copy.getSize(); ++i) {
        EXPECT_EQ(copy.get(i), original.get(i));
    }

    // Verify deep copy by modifying original
    original.addLast(999);
    EXPECT_NE(copy.getSize(), original.getSize());
}


TEST_F(LinkedListUnitTest, CopyConstructorWithEmptyList) {
    const LinkedList<int> original;
    LinkedList<int> copy(original);

    EXPECT_TRUE(copy.isEmpty());
    EXPECT_EQ(copy.getSize(), 0);
}


TEST_F(LinkedListUnitTest, MoveConstructor) {
    LinkedList<int> original;
    original.addLast(10);
    original.addLast(20);
    original.addLast(30);

    const std::size_t original_size = original.getSize();

    LinkedList<int> moved(std::move(original));

    EXPECT_EQ(moved.getSize(), original_size);
    EXPECT_TRUE(original.isEmpty());
    EXPECT_EQ(original.getSize(), 0);

    EXPECT_EQ(moved.get(0), 10);
    EXPECT_EQ(moved.get(1), 20);
    EXPECT_EQ(moved.get(2), 30);
}


TEST_F(LinkedListUnitTest, CopyAssignmentOperator) {
    LinkedList<int> original;
    original.addLast(10);
    original.addLast(20);
    original.addLast(30);

    LinkedList<int> copy;
    copy = original;

    EXPECT_EQ(copy.getSize(), original.getSize());
    for (std::size_t i = 0; i < copy.getSize(); ++i) {
        EXPECT_EQ(copy.get(i), original.get(i));
    }

    // Verify deep copy
    original.addLast(999);
    EXPECT_NE(copy.getSize(), original.getSize());
}


TEST_F(LinkedListUnitTest, CopyAssignmentOperatorSelfAssignment) {
    LinkedList<int> list;
    list.addLast(10);
    list.addLast(20);

    list = list; // Self assignment

    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);
}


TEST_F(LinkedListUnitTest, MoveAssignmentOperator) {
    LinkedList<int> original;
    original.addLast(10);
    original.addLast(20);
    original.addLast(30);

    LinkedList<int> moved;
    const std::size_t original_size = original.getSize();

    moved = std::move(original);

    EXPECT_EQ(moved.getSize(), original_size);
    EXPECT_TRUE(original.isEmpty());
    EXPECT_EQ(original.getSize(), 0);

    EXPECT_EQ(moved.get(0), 10);
    EXPECT_EQ(moved.get(1), 20);
    EXPECT_EQ(moved.get(2), 30);
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wself-move"
TEST_F(LinkedListUnitTest, MoveAssignmentOperatorSelfAssignment) {
    LinkedList<int> list;
    list.addLast(10);
    list.addLast(20);

    list = std::move(list); // Self assignment

    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);
}
#pragma GCC diagnostic pop


TEST_F(LinkedListUnitTest, AddLast) {
    LinkedList<int> list;

    list.addLast(10);
    EXPECT_EQ(list.getSize(), 1);
    EXPECT_EQ(list.get(0), 10);

    list.addLast(20);
    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);

    list.addLast(30);
    EXPECT_EQ(list.getSize(), 3);
    EXPECT_EQ(list.get(2), 30);
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

    list.addFirst(1);
    EXPECT_EQ(list.getSize(), 3);
    EXPECT_EQ(list.get(0), 1);
    EXPECT_EQ(list.get(1), 5);
    EXPECT_EQ(list.get(2), 10);
}


TEST_F(LinkedListUnitTest, InsertAtValidIndices) {
    LinkedList<int> list;

    // Insert at beginning (empty list)
    list.insert(0, 10);
    EXPECT_EQ(list.getSize(), 1);
    EXPECT_EQ(list.get(0), 10);

    // Insert at end
    list.insert(1, 30);
    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.get(1), 30);

    // Insert in middle
    list.insert(1, 20);
    EXPECT_EQ(list.getSize(), 3);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);
    EXPECT_EQ(list.get(2), 30);
}


TEST_F(LinkedListUnitTest, InsertAtInvalidIndex) {
    LinkedList<int> list;
    list.addLast(10);

    EXPECT_THROW(list.insert(2, 20), std::out_of_range);
    EXPECT_THROW(list.insert(10, 99), std::out_of_range);
}


TEST_F(LinkedListUnitTest, RemoveFirstFromNonEmptyList) {
    LinkedList<int> list;
    list.addLast(10);
    list.addLast(20);
    list.addLast(30);

    list.removeFirst();
    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.get(0), 20);
    EXPECT_EQ(list.get(1), 30);

    list.removeFirst();
    EXPECT_EQ(list.getSize(), 1);
    EXPECT_EQ(list.get(0), 30);

    list.removeFirst();
    EXPECT_TRUE(list.isEmpty());
}


TEST_F(LinkedListUnitTest, RemoveFirstFromEmptyList) {
    LinkedList<int> list;

    // Should not crash or throw
    list.removeFirst();
    EXPECT_TRUE(list.isEmpty());
}


TEST_F(LinkedListUnitTest, RemoveLastFromNonEmptyList) {
    LinkedList<int> list;
    list.addLast(10);
    list.addLast(20);
    list.addLast(30);

    list.removeLast();
    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);

    list.removeLast();
    EXPECT_EQ(list.getSize(), 1);
    EXPECT_EQ(list.get(0), 10);

    list.removeLast();
    EXPECT_TRUE(list.isEmpty());
}


TEST_F(LinkedListUnitTest, RemoveLastFromEmptyList) {
    LinkedList<int> list;

    // Should not crash or throw
    list.removeLast();
    EXPECT_TRUE(list.isEmpty());
}


TEST_F(LinkedListUnitTest, RemoveAtValidIndices) {
    LinkedList<int> list;
    list.addLast(10);
    list.addLast(20);
    list.addLast(30);
    list.addLast(40);

    // Remove from middle
    list.removeAt(1);
    EXPECT_EQ(list.getSize(), 3);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 30);
    EXPECT_EQ(list.get(2), 40);

    // Remove from end
    list.removeAt(2);
    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.get(1), 30);

    // Remove from beginning
    list.removeAt(0);
    EXPECT_EQ(list.getSize(), 1);
    EXPECT_EQ(list.get(0), 30);
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


TEST_F(LinkedListUnitTest, RemoveElementThatExists) {
    LinkedList<int> list;
    list.addLast(10);
    list.addLast(20);
    list.addLast(30);
    list.addLast(20); // Duplicate

    // Should remove first occurrence
    list.remove(20);
    EXPECT_EQ(list.getSize(), 3);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 30);
    EXPECT_EQ(list.get(2), 20); // Second 20 should remain
}


TEST_F(LinkedListUnitTest, RemoveElementThatDoesNotExist) {
    LinkedList<int> list;
    list.addLast(10);
    list.addLast(20);

    list.remove(99); // Non-existent element
    EXPECT_EQ(list.getSize(), 2); // Size should remain unchanged
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);
}


TEST_F(LinkedListUnitTest, RemoveFromEmptyList) {
    LinkedList<int> list;

    list.remove(10); // Should not crash
    EXPECT_TRUE(list.isEmpty());
}


TEST_F(LinkedListUnitTest, GetValidIndices) {
    LinkedList<int> list;
    list.addLast(10);
    list.addLast(20);
    list.addLast(30);

    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);
    EXPECT_EQ(list.get(2), 30);

    // Test modification through reference
    list.get(1) = 99;
    EXPECT_EQ(list.get(1), 99);
}


TEST_F(LinkedListUnitTest, GetConstValidIndices) {
    LinkedList<int> list;
    list.addLast(10);
    list.addLast(20);
    list.addLast(30);

    const LinkedList<int>& const_list = list;
    EXPECT_EQ(const_list.get(0), 10);
    EXPECT_EQ(const_list.get(1), 20);
    EXPECT_EQ(const_list.get(2), 30);
}


TEST_F(LinkedListUnitTest, GetInvalidIndex) {
    LinkedList<int> list;
    list.addLast(10);

    EXPECT_THROW(list.get(1), std::out_of_range);
    EXPECT_THROW(list.get(10), std::out_of_range);
}


TEST_F(LinkedListUnitTest, GetFromEmptyList) {
    LinkedList<int> list;

    EXPECT_THROW(list.get(0), std::out_of_range);
}


TEST_F(LinkedListUnitTest, OperatorBracket) {
    LinkedList<int> list;
    list.addLast(10);
    list.addLast(20);
    list.addLast(30);

    EXPECT_EQ(list[0], 10);
    EXPECT_EQ(list[1], 20);
    EXPECT_EQ(list[2], 30);

    // Test modification
    list[1] = 99;
    EXPECT_EQ(list[1], 99);
}


TEST_F(LinkedListUnitTest, OperatorBracketConst) {
    LinkedList<int> list;
    list.addLast(10);
    list.addLast(20);
    list.addLast(30);

    const LinkedList<int>& const_list = list;
    EXPECT_EQ(const_list[0], 10);
    EXPECT_EQ(const_list[1], 20);
    EXPECT_EQ(const_list[2], 30);
}


TEST_F(LinkedListUnitTest, Clear) {
    LinkedList<int> list;
    list.addLast(10);
    list.addLast(20);
    list.addLast(30);

    list.clear();

    EXPECT_TRUE(list.isEmpty());
    EXPECT_EQ(list.getSize(), 0);
}


TEST_F(LinkedListUnitTest, ClearEmptyList) {
    LinkedList<int> list;

    list.clear();

    EXPECT_TRUE(list.isEmpty());
    EXPECT_EQ(list.getSize(), 0);
}


TEST_F(LinkedListUnitTest, PrintForward) {
    LinkedList<int> list;
    list.addLast(10);
    list.addLast(20);
    list.addLast(30);

    // Capture stdout
    std::ostringstream output;
    std::streambuf* orig = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    list.printForward();

    // Restore stdout
    std::cout.rdbuf(orig);

    EXPECT_EQ(output.str(), "10 20 30 \n");
}


TEST_F(LinkedListUnitTest, PrintForwardEmptyList) {
    LinkedList<int> list;

    // Capture stdout
    std::ostringstream output;
    std::streambuf* orig = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    list.printForward();

    // Restore stdout
    std::cout.rdbuf(orig);

    EXPECT_EQ(output.str(), "\n");
}


TEST_F(LinkedListUnitTest, PrintBackward) {
    LinkedList<int> list;
    list.addLast(10);
    list.addLast(20);
    list.addLast(30);

    // Capture stdout
    std::ostringstream output;
    std::streambuf* orig = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    list.printBackward();

    // Restore stdout
    std::cout.rdbuf(orig);

    EXPECT_EQ(output.str(), "30 20 10 \n");
}


TEST_F(LinkedListUnitTest, PrintBackwardEmptyList) {
    LinkedList<int> list;

    // Capture stdout
    std::ostringstream output;
    std::streambuf* orig = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    list.printBackward();

    // Restore stdout
    std::cout.rdbuf(orig);

    EXPECT_EQ(output.str(), "\n");
}


TEST_F(LinkedListUnitTest, StringType) {
    LinkedList<std::string> list;

    list.addLast("First");
    list.addLast("Second");
    list.addLast("Third");

    EXPECT_EQ(list.getSize(), 3);
    EXPECT_EQ(list.get(0), "First");
    EXPECT_EQ(list.get(1), "Second");
    EXPECT_EQ(list.get(2), "Third");

    list.remove("Second");
    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.get(1), "Third");
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

    list.remove(Point(1, 2));
    EXPECT_EQ(list.getSize(), 1);
    EXPECT_EQ(list.get(0), Point(3, 4));
}


TEST_F(LinkedListUnitTest, LargeNumberOfElements) {
    LinkedList<int> list;
    constexpr int num_elements = 1000;

    // Add elements
    for (int i = 0; i < num_elements; ++i) {
        list.addLast(i);
    }

    EXPECT_EQ(list.getSize(), num_elements);

    // Verify all elements
    for (int i = 0; i < num_elements; ++i) {
        EXPECT_EQ(list.get(i), i);
    }

    // Remove elements from the end
    for (int i = num_elements - 1; i >= 0; --i) {
        list.removeLast();
        EXPECT_EQ(list.getSize(), i);
    }

    EXPECT_TRUE(list.isEmpty());
}


TEST_F(LinkedListUnitTest, InterleavedOperations) {
    LinkedList<int> list;

    // Pattern: add 2, remove 1, repeat
    for (int i = 0; i < 100; ++i) {
        list.addLast(i * 2);
        list.addLast(i * 2 + 1);
        if (list.getSize() > 1) {
            list.removeFirst();
        }
    }

    EXPECT_GT(list.getSize(), 0);

    // Verify list is still in valid state
    for (std::size_t i = 0; i < list.getSize() - 1; ++i) {
        EXPECT_LT(list.get(i), list.get(i + 1));
    }
}


TEST_F(LinkedListUnitTest, BiDirectionalTraversal) {
    LinkedList<int> list;
    for (int i = 1; i <= 5; ++i) {
        list.addLast(i);
    }

    // Test that we can access elements efficiently from both ends
    EXPECT_EQ(list.get(0), 1); // Should traverse from head
    EXPECT_EQ(list.get(4), 5); // Should traverse from tail
    EXPECT_EQ(list.get(2), 3); // Should traverse from closer end
}


TEST_F(LinkedListUnitTest, ExceptionSafetyBasic) {
    LinkedList<int> list;

    // Test exception handling for empty list operations
    EXPECT_THROW(list.get(0), std::out_of_range);
    EXPECT_THROW(list[0], std::out_of_range);
    EXPECT_THROW(list.removeAt(0), std::out_of_range);

    // List should still be usable after exceptions
    EXPECT_NO_THROW(list.addLast(42));
    EXPECT_EQ(list.get(0), 42);
    EXPECT_EQ(list.getSize(), 1);
}


TEST_F(LinkedListUnitTest, BasicThrowingTypeUsage) {
    // Test that ThrowingType works correctly with the list when not throwing
    ThrowingType::reset();
    ThrowingType::should_throw = false;

    LinkedList<ThrowingType> list;

    // Test basic operations
    list.addLast(ThrowingType(1));
    EXPECT_EQ(list.getSize(), 1);
    EXPECT_EQ(list.get(0).value, 1);

    list.addFirst(ThrowingType(0));
    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.get(0).value, 0);
    EXPECT_EQ(list.get(1).value, 1);

    // Test removal operations
    list.remove(ThrowingType(0));
    EXPECT_EQ(list.getSize(), 1);
    EXPECT_EQ(list.get(0).value, 1);

    ThrowingType::reset();
}


TEST_F(LinkedListUnitTest, SingleElementOperations) {
    LinkedList<int> list;

    // Test single element operations
    list.addLast(42);
    EXPECT_EQ(list.getSize(), 1);
    EXPECT_EQ(list.get(0), 42);

    // Remove and add again
    list.removeFirst();
    EXPECT_TRUE(list.isEmpty());

    list.addFirst(99);
    EXPECT_EQ(list.getSize(), 1);
    EXPECT_EQ(list.get(0), 99);

    list.removeLast();
    EXPECT_TRUE(list.isEmpty());
}


TEST_F(LinkedListUnitTest, DoublyLinkedListProperties) {
    LinkedList<int> list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);
    list.addLast(4);
    list.addLast(5);

    // Test that forward and backward traversals work correctly
    std::ostringstream forward, backward;

    // Capture forward print
    std::streambuf* orig = std::cout.rdbuf();
    std::cout.rdbuf(forward.rdbuf());
    list.printForward();

    // Capture backward print
    std::cout.rdbuf(backward.rdbuf());
    list.printBackward();

    // Restore stdout
    std::cout.rdbuf(orig);

    EXPECT_EQ(forward.str(), "1 2 3 4 5 \n");
    EXPECT_EQ(backward.str(), "5 4 3 2 1 \n");
}


TEST_F(LinkedListUnitTest, InsertAndRemoveAtBothEnds) {
    LinkedList<int> list;

    // Build list: 1 -> 2 -> 3
    list.addLast(2);
    list.addFirst(1);
    list.addLast(3);

    EXPECT_EQ(list.get(0), 1);
    EXPECT_EQ(list.get(1), 2);
    EXPECT_EQ(list.get(2), 3);

    // Remove from ends
    list.removeFirst(); // Remove 1
    list.removeLast();  // Remove 3

    EXPECT_EQ(list.getSize(), 1);
    EXPECT_EQ(list.get(0), 2);
}
