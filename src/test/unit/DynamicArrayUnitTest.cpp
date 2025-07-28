#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include <string>

#include "DynamicArray.hpp"
#include "ThrowingType.hpp"


class DynamicArrayUnitTest : public ::testing::Test {
  protected:
    void SetUp() override {
        // Setup code that runs before each test
    }

    void TearDown() override {
        // Cleanup code that runs after each test
    }
};


TEST_F(DynamicArrayUnitTest, DefaultConstructor) {
    const DynamicArray<int> arr;

    EXPECT_EQ(arr.size(), 0);
    EXPECT_EQ(arr.capacity(), 5); // DEFAULT_CAPACITY
    EXPECT_TRUE(arr.isEmpty());
}


TEST_F(DynamicArrayUnitTest, ConstructorWithInitialData) {
    int data[] = {1, 2, 3, 4, 5};
    DynamicArray arr(data, 5);

    EXPECT_EQ(arr.size(), 5);
    EXPECT_EQ(arr.capacity(), 5);
    EXPECT_FALSE(arr.isEmpty());

    for (int i = 0; i < 5; ++i)
        EXPECT_EQ(arr.get(i), data[i]);
}


TEST_F(DynamicArrayUnitTest, ConstructorWithInitialDataSmallerThanDefault) {
    int data[] = {1, 2};
    const DynamicArray arr(data, 2);

    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr.capacity(), 5); // Should use DEFAULT_CAPACITY
    EXPECT_FALSE(arr.isEmpty());
}


TEST_F(DynamicArrayUnitTest, ConstructorWithNullDataThrowsException) {
    EXPECT_THROW(DynamicArray<int> arr(nullptr, 5), std::invalid_argument);
}


TEST_F(DynamicArrayUnitTest, ConstructorWithZeroSizeAndNullData) {
    // This should be valid - zero size with null data
    EXPECT_NO_THROW(DynamicArray<int> arr(nullptr, 0));
    const DynamicArray<int> arr(nullptr, 0);
    EXPECT_EQ(arr.size(), 0);
    EXPECT_EQ(arr.capacity(), 5);
}


TEST_F(DynamicArrayUnitTest, CopyConstructor) {
    int data[] = {1, 2, 3};
    DynamicArray original(data, 3);
    DynamicArray copy(original);

    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(copy.capacity(), original.capacity());

    for (std::size_t i = 0; i < copy.size(); ++i)
        EXPECT_EQ(copy.get(i), original.get(i));

    // Verify deep copy - modify original shouldn't affect copy
    original.addLast(999);
    EXPECT_NE(copy.size(), original.size());
}


TEST_F(DynamicArrayUnitTest, MoveConstructor) {
    int data[] = {1, 2, 3};
    DynamicArray original(data, 3);
    const std::size_t original_size = original.size();
    const std::size_t original_capacity = original.capacity();

    DynamicArray moved(std::move(original));

    EXPECT_EQ(moved.size(), original_size);
    EXPECT_EQ(moved.capacity(), original_capacity);
    EXPECT_EQ(original.size(), 0);
    EXPECT_EQ(original.capacity(), 0);

    for (int i = 0; i < 3; ++i)
        EXPECT_EQ(moved.get(i), data[i]);
}


TEST_F(DynamicArrayUnitTest, CopyAssignmentOperator) {
    int data[] = {1, 2, 3};
    DynamicArray original(data, 3);
    DynamicArray<int> copy;
    copy = original;

    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(copy.capacity(), original.capacity());

    // Verify deep copy
    for (std::size_t i = 0; i < copy.size(); ++i)
        EXPECT_EQ(copy.get(i), original.get(i));

    // Self-assignment should work correctly
    original = original;
    EXPECT_EQ(original.size(), 3);
    for (int i = 0; i < 3; ++i)
        EXPECT_EQ(original.get(i), data[i]);
}


TEST_F(DynamicArrayUnitTest, MoveAssignmentOperator) {
    int data[] = {1, 2, 3};
    DynamicArray original(data, 3);
    const std::size_t original_size = original.size();
    const std::size_t original_capacity = original.capacity();

    DynamicArray<int> moved;
    moved = std::move(original);

    EXPECT_EQ(moved.size(), original_size);
    EXPECT_EQ(moved.capacity(), original_capacity);
    EXPECT_EQ(original.size(), 0);
    EXPECT_EQ(original.capacity(), 0);

    // Self-move assignment should work correctly
    moved = std::move(moved);
    EXPECT_EQ(moved.size(), original_size);
    for (int i = 0; i < 3; ++i)
        EXPECT_EQ(moved.get(i), data[i]);
}


TEST_F(DynamicArrayUnitTest, ConstructorWithNegativeSize) {
    // Test with very large size that would cause bad_alloc during construction
    EXPECT_THROW(
        {
            int* dummy_data =
                reinterpret_cast<int*>(1); // Non-null but invalid pointer
            DynamicArray<int> arr(dummy_data, SIZE_MAX);
        },
        std::bad_alloc);
}


TEST_F(DynamicArrayUnitTest, AddLast) {
    DynamicArray<int> arr;

    arr.addLast(10);
    EXPECT_EQ(arr.size(), 1);
    EXPECT_EQ(arr.get(0), 10);

    arr.addLast(20);
    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr.get(1), 20);
}


TEST_F(DynamicArrayUnitTest, AddFirst) {
    DynamicArray<int> arr;

    arr.addFirst(10);
    EXPECT_EQ(arr.size(), 1);
    EXPECT_EQ(arr.get(0), 10);

    arr.addFirst(5);
    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr.get(0), 5);
    EXPECT_EQ(arr.get(1), 10);
}


TEST_F(DynamicArrayUnitTest, InsertAtValidIndex) {
    DynamicArray<int> arr;
    arr.addLast(10);
    arr.addLast(30);

    arr.insert(1, 20);

    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr.get(0), 10);
    EXPECT_EQ(arr.get(1), 20);
    EXPECT_EQ(arr.get(2), 30);
}


TEST_F(DynamicArrayUnitTest, InsertAtBeginning) {
    DynamicArray<int> arr;
    arr.addLast(20);
    arr.addLast(30);

    arr.insert(0, 10);

    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr.get(0), 10);
    EXPECT_EQ(arr.get(1), 20);
    EXPECT_EQ(arr.get(2), 30);
}


TEST_F(DynamicArrayUnitTest, InsertAtEnd) {
    DynamicArray<int> arr;
    arr.addLast(10);
    arr.addLast(20);

    arr.insert(2, 30);

    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr.get(2), 30);
}


TEST_F(DynamicArrayUnitTest, InsertAtInvalidIndex) {
    DynamicArray<int> arr;
    arr.addLast(10);

    EXPECT_THROW(arr.insert(2, 20), std::out_of_range);
}


TEST_F(DynamicArrayUnitTest, CapacityExpansionOnInsertion) {
    DynamicArray<int> arr;
    const std::size_t initial_capacity = arr.capacity();

    // Fill to capacity
    for (std::size_t i = 0; i < initial_capacity; ++i)
        arr.addLast(i);

    EXPECT_EQ(arr.capacity(), initial_capacity);

    // Add one more to trigger expansion
    arr.addLast(999);

    EXPECT_EQ(arr.capacity(), initial_capacity * 2);
    EXPECT_EQ(arr.size(), initial_capacity + 1);
    EXPECT_EQ(arr.get(initial_capacity), 999);
}


TEST_F(DynamicArrayUnitTest, RemoveAt) {
    int data[] = {10, 20, 30, 40};
    DynamicArray arr(data, 4);

    const int removed = arr.removeAt(1);

    EXPECT_EQ(removed, 20);
    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr.get(0), 10);
    EXPECT_EQ(arr.get(1), 30);
    EXPECT_EQ(arr.get(2), 40);
}


TEST_F(DynamicArrayUnitTest, RemoveAtInvalidIndex) {
    DynamicArray<int> arr;
    arr.addLast(10);

    EXPECT_THROW(arr.removeAt(1), std::out_of_range);
    EXPECT_THROW(arr.removeAt(10), std::out_of_range);
}


TEST_F(DynamicArrayUnitTest, RemoveFromEmptyArray) {
    DynamicArray<int> arr;

    EXPECT_THROW(arr.removeAt(0), std::out_of_range);
}


TEST_F(DynamicArrayUnitTest, RemoveFirst) {
    int data[] = {10, 20, 30};
    DynamicArray arr(data, 3);

    const int removed = arr.removeFirst();

    EXPECT_EQ(removed, 10);
    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr.get(0), 20);
    EXPECT_EQ(arr.get(1), 30);
}


TEST_F(DynamicArrayUnitTest, RemoveLast) {
    int data[] = {10, 20, 30};
    DynamicArray arr(data, 3);

    const int removed = arr.removeLast();

    EXPECT_EQ(removed, 30);
    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr.get(0), 10);
    EXPECT_EQ(arr.get(1), 20);
}


TEST_F(DynamicArrayUnitTest, CapacityReductionOnRemoval) {
    DynamicArray<int> arr;

    // Fill array to trigger expansion
    for (int i = 0; i < 10; ++i)
        arr.addLast(i);

    const std::size_t expanded_capacity = arr.capacity();

    // Remove elements until size is 1/4 of capacity
    while (arr.size() > expanded_capacity / 4)
        arr.removeLast();

    std::size_t size_before_trigger = arr.size();
    arr.removeLast(); // This should trigger capacity reduction

    EXPECT_LT(arr.capacity(), expanded_capacity);
    EXPECT_EQ(arr.capacity(), expanded_capacity / 2);
}


TEST_F(DynamicArrayUnitTest, CapacityDoesNotReduceBelowDefault) {
    DynamicArray<int> arr;
    arr.addLast(1);
    arr.addLast(2);

    arr.removeLast();
    arr.removeLast();

    EXPECT_EQ(arr.capacity(), 5); // Should remain at DEFAULT_CAPACITY
}


TEST_F(DynamicArrayUnitTest, GetValidIndex) {
    int data[] = {10, 20, 30};
    DynamicArray arr(data, 3);

    EXPECT_EQ(arr.get(0), 10);
    EXPECT_EQ(arr.get(1), 20);
    EXPECT_EQ(arr.get(2), 30);
}


TEST_F(DynamicArrayUnitTest, GetInvalidIndex) {
    DynamicArray<int> arr;
    arr.addLast(10);

    EXPECT_THROW(arr.get(1), std::out_of_range);
    EXPECT_THROW(arr.get(10), std::out_of_range);
}


TEST_F(DynamicArrayUnitTest, OperatorBracket) {
    int data[] = {10, 20, 30};
    DynamicArray arr(data, 3);

    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 30);

    // Test modification through operator[]
    arr[1] = 99;
    EXPECT_EQ(arr[1], 99);
}


TEST_F(DynamicArrayUnitTest, OperatorBracketConst) {
    int data[] = {10, 20, 30};
    const DynamicArray arr(data, 3);

    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 30);
}


TEST_F(DynamicArrayUnitTest, GetFirst) {
    int data[] = {10, 20, 30};
    DynamicArray arr(data, 3);

    EXPECT_EQ(arr.getFirst(), 10);

    // Test modification
    arr.getFirst() = 99;
    EXPECT_EQ(arr.getFirst(), 99);
}


TEST_F(DynamicArrayUnitTest, GetFirstFromEmptyArray) {
    DynamicArray<int> arr;

    EXPECT_THROW(arr.getFirst(), std::out_of_range);
}


TEST_F(DynamicArrayUnitTest, GetLast) {
    int data[] = {10, 20, 30};
    DynamicArray arr(data, 3);

    EXPECT_EQ(arr.getLast(), 30);

    // Test modification
    arr.getLast() = 99;
    EXPECT_EQ(arr.getLast(), 99);
}


TEST_F(DynamicArrayUnitTest, GetLastFromEmptyArray) {
    DynamicArray<int> arr;

    EXPECT_THROW(arr.getLast(), std::out_of_range);
}


TEST_F(DynamicArrayUnitTest, Clear) {
    int data[] = {10, 20, 30, 40, 50};
    DynamicArray arr(data, 5);

    arr.clear();

    EXPECT_EQ(arr.size(), 0);
    EXPECT_EQ(arr.capacity(), 5); // Should reset to DEFAULT_CAPACITY
    EXPECT_TRUE(arr.isEmpty());
}


TEST_F(DynamicArrayUnitTest, ClearEmptyArray) {
    DynamicArray<int> arr;

    arr.clear();

    EXPECT_EQ(arr.size(), 0);
    EXPECT_EQ(arr.capacity(), 5);
    EXPECT_TRUE(arr.isEmpty());
}


TEST_F(DynamicArrayUnitTest, IsEmpty) {
    DynamicArray<int> arr;

    EXPECT_TRUE(arr.isEmpty());

    arr.addLast(10);
    EXPECT_FALSE(arr.isEmpty());

    arr.removeLast();
    EXPECT_TRUE(arr.isEmpty());
}


TEST_F(DynamicArrayUnitTest, StringType) {
    DynamicArray<std::string> arr;

    arr.addLast("Hello");
    arr.addLast("World");

    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr.get(0), "Hello");
    EXPECT_EQ(arr.get(1), "World");
}


TEST_F(DynamicArrayUnitTest, CustomObjectType) {
    struct Point {
        int x, y;
        explicit Point(int x = 0, int y = 0) : x(x), y(y) {}
        bool operator==(const Point& other) const {
            return x == other.x && y == other.y;
        }
    };

    DynamicArray<Point> arr;
    arr.addLast(Point(1, 2));
    arr.addLast(Point(3, 4));

    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr.get(0), Point(1, 2));
    EXPECT_EQ(arr.get(1), Point(3, 4));
}


TEST_F(DynamicArrayUnitTest, LargeNumberOfInsertions) {
    DynamicArray<int> arr;
    constexpr int num_elements = 1000;

    for (int i = 0; i < num_elements; ++i)
        arr.addLast(i);

    EXPECT_EQ(arr.size(), num_elements);

    for (int i = 0; i < num_elements; ++i)
        EXPECT_EQ(arr.get(i), i);
}


TEST_F(DynamicArrayUnitTest, AlternatingInsertionAndRemoval) {
    DynamicArray<int> arr;

    // Pattern: insert 2, remove 1, repeat
    for (int i = 0; i < 100; ++i) {
        arr.addLast(i * 2);
        arr.addLast(i * 2 + 1);
        if (arr.size() > 1)
            arr.removeFirst();
    }

    EXPECT_GT(arr.size(), 0);
    // Verify array is still in valid state
    for (std::size_t i = 0; i < arr.size() - 1; ++i)
        EXPECT_LT(arr.get(i), arr.get(i + 1));
}


TEST_F(DynamicArrayUnitTest, ExceptionSafetyDuringExpansion) {
    ThrowingType::reset();

    DynamicArray<ThrowingType> arr;

    // Fill array to near capacity
    for (int i = 0; i < 4; ++i)
        arr.addLast(ThrowingType(i));

    ThrowingType::should_throw = true;

    // This should throw during expansion
    EXPECT_THROW(arr.addLast(ThrowingType(999)), std::runtime_error);

    // Array should still be in valid state
    EXPECT_EQ(arr.size(), 4);
    for (std::size_t i = 0; i < arr.size(); ++i)
        EXPECT_EQ(arr.get(i).value, static_cast<int>(i));

    ThrowingType::reset();
}