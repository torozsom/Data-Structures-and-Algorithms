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

    EXPECT_EQ(list.size(), 0);
    EXPECT_TRUE(list.isEmpty());
}


TEST_F(LinkedListUnitTest, ArrayConstructorWithValidData) {
    int data[] = {1, 2, 3, 4, 5};
    const LinkedList<int> list(data, 5);

    EXPECT_EQ(list.size(), 5);
    EXPECT_FALSE(list.isEmpty());

    for (std::size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(list.get(i), data[i]);
    }
}


TEST_F(LinkedListUnitTest, ArrayConstructorWithEmptyData) {
    const LinkedList<int> list(nullptr, 0);

    EXPECT_EQ(list.size(), 0);
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

    EXPECT_EQ(copy.size(), original.size());
    for (std::size_t i = 0; i < copy.size(); ++i) {
        EXPECT_EQ(copy.get(i), original.get(i));
    }

    // Verify deep copy by modifying original
    original.addLast(999);
    EXPECT_NE(copy.size(), original.size());
}


TEST_F(LinkedListUnitTest, CopyConstructorWithEmptyList) {
    const LinkedList<int> original;
    const LinkedList<int> copy(original);

    EXPECT_TRUE(copy.isEmpty());
    EXPECT_EQ(copy.size(), 0);
}


TEST_F(LinkedListUnitTest, MoveConstructor) {
    LinkedList<int> original;
    original.addLast(10);
    original.addLast(20);
    original.addLast(30);

    const std::size_t original_size = original.size();

    LinkedList<int> moved(std::move(original));

    EXPECT_EQ(moved.size(), original_size);
    EXPECT_TRUE(original.isEmpty());
    EXPECT_EQ(original.size(), 0);

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

    EXPECT_EQ(copy.size(), original.size());
    for (std::size_t i = 0; i < copy.size(); ++i) {
        EXPECT_EQ(copy.get(i), original.get(i));
    }

    // Verify deep copy
    original.addLast(999);
    EXPECT_NE(copy.size(), original.size());
}


TEST_F(LinkedListUnitTest, CopyAssignmentOperatorSelfAssignment) {
    LinkedList<int> list;
    list.addLast(10);
    list.addLast(20);

    list = list; // Self assignment

    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);
}


TEST_F(LinkedListUnitTest, MoveAssignmentOperator) {
    LinkedList<int> original;
    original.addLast(10);
    original.addLast(20);
    original.addLast(30);

    LinkedList<int> moved;
    const std::size_t original_size = original.size();

    moved = std::move(original);

    EXPECT_EQ(moved.size(), original_size);
    EXPECT_TRUE(original.isEmpty());
    EXPECT_EQ(original.size(), 0);

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

    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);
}
#pragma GCC diagnostic pop


TEST_F(LinkedListUnitTest, AddLast) {
    LinkedList<int> list;

    list.addLast(10);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.get(0), 10);

    list.addLast(20);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);

    list.addLast(30);
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.get(2), 30);
}


TEST_F(LinkedListUnitTest, AddFirst) {
    LinkedList<int> list;

    list.addFirst(10);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.get(0), 10);

    list.addFirst(5);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.get(0), 5);
    EXPECT_EQ(list.get(1), 10);

    list.addFirst(1);
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.get(0), 1);
    EXPECT_EQ(list.get(1), 5);
    EXPECT_EQ(list.get(2), 10);
}


TEST_F(LinkedListUnitTest, InsertAtValidIndices) {
    LinkedList<int> list;

    // Insert at beginning (empty list)
    list.insert(10, 0);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.get(0), 10);

    // Insert at end
    list.insert(30, 1);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.get(1), 30);

    // Insert in middle
    list.insert(20, 1);
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);
    EXPECT_EQ(list.get(2), 30);
}


TEST_F(LinkedListUnitTest, InsertAtInvalidIndex) {
    LinkedList<int> list;
    list.addLast(10);

    EXPECT_THROW(list.insert(20, 2), std::out_of_range);
    EXPECT_THROW(list.insert(99, 10), std::out_of_range);
}


TEST_F(LinkedListUnitTest, RemoveFirstFromNonEmptyList) {
    LinkedList<int> list;
    list.addLast(10);
    list.addLast(20);
    list.addLast(30);

    list.removeFirst();
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.get(0), 20);
    EXPECT_EQ(list.get(1), 30);

    list.removeFirst();
    EXPECT_EQ(list.size(), 1);
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
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);

    list.removeLast();
    EXPECT_EQ(list.size(), 1);
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
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 30);
    EXPECT_EQ(list.get(2), 40);

    // Remove from end
    list.removeAt(2);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.get(1), 30);

    // Remove from beginning
    list.removeAt(0);
    EXPECT_EQ(list.size(), 1);
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
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 30);
    EXPECT_EQ(list.get(2), 20); // Second 20 should remain
}


TEST_F(LinkedListUnitTest, RemoveElementThatDoesNotExist) {
    LinkedList<int> list;
    list.addLast(10);
    list.addLast(20);

    list.remove(99);              // Non-existent element
    EXPECT_EQ(list.size(), 2); // Size should remain unchanged
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
    EXPECT_EQ(list.size(), 0);
}


TEST_F(LinkedListUnitTest, ClearEmptyList) {
    LinkedList<int> list;

    list.clear();

    EXPECT_TRUE(list.isEmpty());
    EXPECT_EQ(list.size(), 0);
}


TEST_F(LinkedListUnitTest, StringType) {
    LinkedList<std::string> list;

    list.addLast("First");
    list.addLast("Second");
    list.addLast("Third");

    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.get(0), "First");
    EXPECT_EQ(list.get(1), "Second");
    EXPECT_EQ(list.get(2), "Third");

    list.remove("Second");
    EXPECT_EQ(list.size(), 2);
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

    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.get(0), Point(1, 2));
    EXPECT_EQ(list.get(1), Point(3, 4));

    list.remove(Point(1, 2));
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.get(0), Point(3, 4));
}


TEST_F(LinkedListUnitTest, LargeNumberOfElements) {
    LinkedList<int> list;
    constexpr int num_elements = 1000;

    // Add elements
    for (int i = 0; i < num_elements; ++i) {
        list.addLast(i);
    }

    EXPECT_EQ(list.size(), num_elements);

    // Verify all elements
    for (int i = 0; i < num_elements; ++i) {
        EXPECT_EQ(list.get(i), i);
    }

    // Remove elements from the end
    for (int i = num_elements - 1; i >= 0; --i) {
        list.removeLast();
        EXPECT_EQ(list.size(), i);
    }

    EXPECT_TRUE(list.isEmpty());
}


TEST_F(LinkedListUnitTest, InterleavedOperations) {
    LinkedList<int> list;

    // Pattern: add 2, remove 1, repeat
    for (int i = 0; i < 100; ++i) {
        list.addLast(i * 2);
        list.addLast(i * 2 + 1);
        if (list.size() > 1) {
            list.removeFirst();
        }
    }

    EXPECT_GT(list.size(), 0);

    // Verify list is still in valid state
    for (std::size_t i = 0; i < list.size() - 1; ++i) {
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
    EXPECT_EQ(list.size(), 1);
}


TEST_F(LinkedListUnitTest, BasicThrowingTypeUsage) {
    // Test that ThrowingType works correctly with the list when not throwing
    ThrowingType::reset();
    ThrowingType::should_throw = false;

    LinkedList<ThrowingType> list;

    // Test basic operations
    list.addLast(ThrowingType(1));
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.get(0).value, 1);

    list.addFirst(ThrowingType(0));
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.get(0).value, 0);
    EXPECT_EQ(list.get(1).value, 1);

    // Test removal operations
    list.remove(ThrowingType(0));
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.get(0).value, 1);

    ThrowingType::reset();
}


TEST_F(LinkedListUnitTest, SingleElementOperations) {
    LinkedList<int> list;

    // Test single element operations
    list.addLast(42);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.get(0), 42);

    // Remove and add again
    list.removeFirst();
    EXPECT_TRUE(list.isEmpty());

    list.addFirst(99);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.get(0), 99);

    list.removeLast();
    EXPECT_TRUE(list.isEmpty());
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

    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.get(0), 2);
}


TEST_F(LinkedListUnitTest, RemoveAllRemovesEveryMatchAndReturnsCount) {
    LinkedList<int> list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(1);
    list.addLast(3);
    list.addLast(1);

    const std::size_t removed_count = list.removeAll(1);
    EXPECT_EQ(removed_count, 3);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.get(0), 2);
    EXPECT_EQ(list.get(1), 3);
}


TEST_F(LinkedListUnitTest, RemoveAllHandlesEmptyList) {
    LinkedList<int> list;

    EXPECT_EQ(list.removeAll(1), 0);
    EXPECT_TRUE(list.isEmpty());
    EXPECT_EQ(list.size(), 0);
}


TEST_F(LinkedListUnitTest, HandlesMoveOnlyTypes) {
    LinkedList<std::unique_ptr<int>> list;
    list.addLast(std::make_unique<int>(1));
    list.addLast(std::make_unique<int>(2));
    list.addLast(std::make_unique<int>(3));

    int expected = 1;
    for (const auto& ptr : list)
        EXPECT_EQ(*ptr, expected++);

    list.removeFirst();
    list.removeLast();

    EXPECT_EQ(list.size(), 1);
    EXPECT_FALSE(list.isEmpty());
    EXPECT_EQ(*list.get(0), 2);

    expected = 2;
    for (const auto& ptr : list)
        EXPECT_EQ(*ptr, expected++);
}


TEST_F(LinkedListUnitTest, RangeBasedTraversalHandlesEmpty) {
    LinkedList<int> list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);

    int expeceted = 1;
    for (int item : list)
        EXPECT_EQ(item, expeceted++);

    EXPECT_EQ(expeceted, 4);

    LinkedList<int> empty;
    int count = 0;
    for (int item : empty) {
        (void)item; // Suppress unused variable warning
        ++count;
    }
    EXPECT_EQ(count, 0); // Should not iterate over anything
}


TEST_F(LinkedListUnitTest, ManualIteratorTraversal) {
    LinkedList<int> list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);

    auto it = list.begin();
    EXPECT_EQ(*it, 1);

    ++it;
    EXPECT_EQ(*it, 2);

    auto post_inc = it++;
    EXPECT_EQ(*post_inc, 2); // Should return the value before increment
    EXPECT_EQ(*it, 3); // Now it should point to 3

    auto post_dec = it--;
    EXPECT_EQ(*post_dec, 3); // Should return the value before decrement
    EXPECT_EQ(*it, 2); // Now it should point back to 2

    --it;
    EXPECT_EQ(*it, 1); // Should point back to the first element

    LinkedList<int> empty;
    EXPECT_EQ(empty.begin(), empty.end());
}

TEST_F(LinkedListUnitTest, ClearShouldResetList) {
    LinkedList<int> list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);
    list.clear();
    EXPECT_TRUE(list.isEmpty());
    EXPECT_EQ(list.size(), 0);
    EXPECT_THROW(list.get(0), std::out_of_range);
}