#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "Stack.hpp"
#include "ThrowingType.hpp"


class StackUnitTest : public testing::Test {
  protected:
    void SetUp() override {
        // Setup code that runs before each test
    }

    void TearDown() override {
        // Cleanup code that runs after each test
    }
};


TEST_F(StackUnitTest, DefaultConstructor) {
    const Stack<int> stack;

    EXPECT_TRUE(stack.isEmpty());
    EXPECT_EQ(stack.size(), 0);
}


TEST_F(StackUnitTest, CopyConstructor) {
    Stack<int> original;
    original.push(10);
    original.push(20);
    original.push(30);

    Stack copy(original);

    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(copy.top(), 30);

    // Verify deep copy - modify original shouldn't affect copy
    original.push(999);
    EXPECT_NE(copy.size(), original.size());
    EXPECT_EQ(copy.top(), 30);
}


TEST_F(StackUnitTest, CopyConstructorWithEmptyStack) {
    const Stack<int> original;
    const Stack copy(original);

    EXPECT_TRUE(copy.isEmpty());
    EXPECT_EQ(copy.size(), 0);
}


TEST_F(StackUnitTest, MoveConstructor) {
    Stack<int> original;
    original.push(10);
    original.push(20);
    original.push(30);

    const std::size_t original_size = original.size();

    Stack moved(std::move(original));

    EXPECT_EQ(moved.size(), original_size);
    EXPECT_EQ(moved.top(), 30);
    EXPECT_TRUE(original.isEmpty());
}


TEST_F(StackUnitTest, CopyAssignmentOperator) {
    Stack<int> original;
    original.push(10);
    original.push(20);
    original.push(30);

    Stack<int> copy;
    copy = original;

    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(copy.top(), 30);

    // Verify deep copy
    original.push(999);
    EXPECT_NE(copy.size(), original.size());
    EXPECT_EQ(copy.top(), 30);
}


TEST_F(StackUnitTest, CopyAssignmentOperatorSelfAssignment) {
    Stack<int> stack;
    stack.push(10);
    stack.push(20);

    stack = stack; // Self assignment

    EXPECT_EQ(stack.size(), 2);
    EXPECT_EQ(stack.top(), 20);
}


TEST_F(StackUnitTest, MoveAssignmentOperator) {
    Stack<int> original;
    original.push(10);
    original.push(20);
    original.push(30);

    Stack<int> moved;
    const std::size_t original_size = original.size();

    moved = std::move(original);

    EXPECT_EQ(moved.size(), original_size);
    EXPECT_EQ(moved.top(), 30);
    EXPECT_TRUE(original.isEmpty());
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wself-move"
TEST_F(StackUnitTest, MoveAssignmentOperatorSelfAssignment) {
    Stack<int> stack;
    stack.push(10);
    stack.push(20);

    stack = std::move(stack); // Self assignment

    EXPECT_EQ(stack.size(), 2);
    EXPECT_EQ(stack.top(), 20);
}
#pragma GCC diagnostic pop


TEST_F(StackUnitTest, PushSingleElement) {
    Stack<int> stack;

    stack.push(42);

    EXPECT_FALSE(stack.isEmpty());
    EXPECT_EQ(stack.size(), 1);
    EXPECT_EQ(stack.top(), 42);
}


TEST_F(StackUnitTest, PushMultipleElements) {
    Stack<int> stack;

    stack.push(10);
    stack.push(20);
    stack.push(30);

    EXPECT_EQ(stack.size(), 3);
    EXPECT_EQ(stack.top(), 30); // LIFO - last pushed should be on top
}


TEST_F(StackUnitTest, PushLargeNumberOfElements) {
    Stack<int> stack;
    constexpr int num_elements = 1000;

    for (int i = 0; i < num_elements; ++i)
        stack.push(i);

    EXPECT_EQ(stack.size(), num_elements);
    EXPECT_EQ(stack.top(), num_elements - 1);
}


TEST_F(StackUnitTest, PopSingleElement) {
    Stack<int> stack;
    stack.push(42);

    const int popped = stack.pop();

    EXPECT_EQ(popped, 42);
    EXPECT_TRUE(stack.isEmpty());
    EXPECT_EQ(stack.size(), 0);
}


TEST_F(StackUnitTest, PopMultipleElements) {
    Stack<int> stack;
    stack.push(10);
    stack.push(20);
    stack.push(30);

    // Pop elements should follow LIFO order
    EXPECT_EQ(stack.pop(), 30);
    EXPECT_EQ(stack.size(), 2);

    EXPECT_EQ(stack.pop(), 20);
    EXPECT_EQ(stack.size(), 1);

    EXPECT_EQ(stack.pop(), 10);
    EXPECT_TRUE(stack.isEmpty());
}


TEST_F(StackUnitTest, PopFromEmptyStack) {
    Stack<int> stack;

    EXPECT_THROW(stack.pop(), std::out_of_range);
}


TEST_F(StackUnitTest, PopAllElementsThenPushAgain) {
    Stack<int> stack;
    stack.push(10);
    stack.push(20);

    stack.pop();
    stack.pop();

    EXPECT_TRUE(stack.isEmpty());

    stack.push(99);
    EXPECT_EQ(stack.top(), 99);
    EXPECT_EQ(stack.size(), 1);
}


TEST_F(StackUnitTest, TopNonConstAccess) {
    Stack<int> stack;
    stack.push(42);

    int& top_ref = stack.top();
    EXPECT_EQ(top_ref, 42);

    // Modify through reference
    top_ref = 99;
    EXPECT_EQ(stack.top(), 99);
}


TEST_F(StackUnitTest, TopConstAccess) {
    Stack<int> stack;
    stack.push(42);

    const Stack<int>& const_stack = stack;
    const int& top_ref = const_stack.top();

    EXPECT_EQ(top_ref, 42);
}


TEST_F(StackUnitTest, TopWithMultipleElements) {
    Stack<int> stack;
    stack.push(10);
    stack.push(20);
    stack.push(30);

    EXPECT_EQ(stack.top(), 30);

    stack.pop();
    EXPECT_EQ(stack.top(), 20);

    stack.push(99);
    EXPECT_EQ(stack.top(), 99);
}


TEST_F(StackUnitTest, TopFromEmptyStack) {
    Stack<int> stack;
    EXPECT_THROW(stack.top(), std::out_of_range);
}


TEST_F(StackUnitTest, TopFromEmptyConstStack) {
    const Stack<int> stack;
    EXPECT_THROW(stack.top(), std::out_of_range);
}


TEST_F(StackUnitTest, IsEmpty) {
    Stack<int> stack;

    EXPECT_TRUE(stack.isEmpty());

    stack.push(10);
    EXPECT_FALSE(stack.isEmpty());

    stack.pop();
    EXPECT_TRUE(stack.isEmpty());
}


TEST_F(StackUnitTest, Size) {
    Stack<int> stack;

    EXPECT_EQ(stack.size(), 0);

    stack.push(10);
    EXPECT_EQ(stack.size(), 1);

    stack.push(20);
    stack.push(30);
    EXPECT_EQ(stack.size(), 3);

    stack.pop();
    EXPECT_EQ(stack.size(), 2);

    stack.clear();
    EXPECT_EQ(stack.size(), 0);
}


TEST_F(StackUnitTest, Clear) {
    Stack<int> stack;
    stack.push(10);
    stack.push(20);
    stack.push(30);

    stack.clear();

    EXPECT_TRUE(stack.isEmpty());
    EXPECT_EQ(stack.size(), 0);
}


TEST_F(StackUnitTest, ClearEmptyStack) {
    Stack<int> stack;

    stack.clear();

    EXPECT_TRUE(stack.isEmpty());
    EXPECT_EQ(stack.size(), 0);
}


TEST_F(StackUnitTest, StringType) {
    Stack<std::string> stack;

    stack.push("First");
    stack.push("Second");
    stack.push("Third");

    EXPECT_EQ(stack.size(), 3);
    EXPECT_EQ(stack.top(), "Third");

    EXPECT_EQ(stack.pop(), "Third");
    EXPECT_EQ(stack.pop(), "Second");
    EXPECT_EQ(stack.pop(), "First");
}


TEST_F(StackUnitTest, CustomObjectType) {
    struct Point {
        int x, y;
        explicit Point(const int x = 0, const int y = 0) : x(x), y(y) {}
        bool operator==(const Point& other) const {
            return x == other.x && y == other.y;
        }
    };

    Stack<Point> stack;
    stack.push(Point(1, 2));
    stack.push(Point(3, 4));

    EXPECT_EQ(stack.size(), 2);
    EXPECT_EQ(stack.top(), Point(3, 4));

    const Point popped = stack.pop();
    EXPECT_EQ(popped, Point(3, 4));
    EXPECT_EQ(stack.top(), Point(1, 2));
}


TEST_F(StackUnitTest, LIFOBehavior) {
    Stack<int> stack;

    // Push elements in order
    for (int i = 1; i <= 5; ++i)
        stack.push(i * 10);

    // Pop elements should be in reverse order (LIFO)
    for (int i = 5; i >= 1; --i)
        EXPECT_EQ(stack.pop(), i * 10);

    EXPECT_TRUE(stack.isEmpty());
}


TEST_F(StackUnitTest, InterleavedPushPop) {
    Stack<int> stack;

    stack.push(10);
    stack.push(20);
    EXPECT_EQ(stack.pop(), 20);

    stack.push(30);
    stack.push(40);
    EXPECT_EQ(stack.pop(), 40);
    EXPECT_EQ(stack.pop(), 30);
    EXPECT_EQ(stack.pop(), 10);

    EXPECT_TRUE(stack.isEmpty());
}


TEST_F(StackUnitTest, LargeNumberOfOperations) {
    Stack<int> stack;
    constexpr int num_operations = 1000;

    // Push many elements
    for (int i = 0; i < num_operations; ++i)
        stack.push(i);

    EXPECT_EQ(stack.size(), num_operations);

    // Pop all elements and verify LIFO order
    for (int i = num_operations - 1; i >= 0; --i)
        EXPECT_EQ(stack.pop(), i);

    EXPECT_TRUE(stack.isEmpty());
}


TEST_F(StackUnitTest, AlternatingPushPop) {
    Stack<int> stack;

    // Pattern: push 2, pop 1, repeat
    for (int i = 0; i < 100; ++i) {
        stack.push(i * 2);
        stack.push(i * 2 + 1);
        if (stack.size() > 1)
            stack.pop();
    }

    EXPECT_GT(stack.size(), 0);

    // Verify stack is still in valid state
    int previous = stack.pop();
    while (!stack.isEmpty()) {
        int current = stack.pop();
        EXPECT_LT(current, previous);
        previous = current;
    }
}


TEST_F(StackUnitTest, ExceptionSafetyBasic) {
    // Test basic exception handling for empty stack operations
    Stack<int> stack;

    // These should throw std::out_of_range
    EXPECT_THROW(stack.pop(), std::out_of_range);
    EXPECT_THROW(stack.top(), std::out_of_range);

    const Stack<int>& const_stack = stack;
    EXPECT_THROW(const_stack.top(), std::out_of_range);

    // Stack should still be usable after exceptions
    EXPECT_NO_THROW(stack.push(42));
    EXPECT_EQ(stack.top(), 42);
    EXPECT_EQ(stack.size(), 1);
}


TEST_F(StackUnitTest, BasicThrowingTypeUsage) {
    // Test that ThrowingType works correctly with the stack when not throwing
    ThrowingType::reset();
    ThrowingType::should_throw = false;

    Stack<ThrowingType> stack;

    // Test basic operations
    stack.push(ThrowingType(1));
    EXPECT_EQ(stack.size(), 1);
    EXPECT_EQ(stack.top().value, 1);

    stack.push(ThrowingType(2));
    EXPECT_EQ(stack.size(), 2);
    EXPECT_EQ(stack.top().value, 2);

    // Test pop operations
    ThrowingType popped = stack.pop();
    EXPECT_EQ(popped.value, 2);
    EXPECT_EQ(stack.size(), 1);

    popped = stack.pop();
    EXPECT_EQ(popped.value, 1);
    EXPECT_TRUE(stack.isEmpty());

    ThrowingType::reset();
}


TEST_F(StackUnitTest, PushPopSingleElementMultipleTimes) {
    Stack<int> stack;

    for (int i = 0; i < 10; ++i) {
        stack.push(i);
        EXPECT_EQ(stack.top(), i);
        EXPECT_EQ(stack.pop(), i);
        EXPECT_TRUE(stack.isEmpty());
    }
}


TEST_F(StackUnitTest, TopDoesNotModifyStack) {
    Stack<int> stack;
    stack.push(10);
    stack.push(20);

    // Multiple calls to top() should not change the stack
    EXPECT_EQ(stack.top(), 20);
    EXPECT_EQ(stack.top(), 20);
    EXPECT_EQ(stack.top(), 20);
    EXPECT_EQ(stack.size(), 2);

    // Stack should still work normally
    EXPECT_EQ(stack.pop(), 20);
    EXPECT_EQ(stack.top(), 10);
}


TEST_F(StackUnitTest, CopyConstructorPreservesOrder) {
    Stack<int> original;
    for (int i = 1; i <= 5; ++i)
        original.push(i);

    Stack copy(original);

    // Both stacks should pop elements in the same order
    for (int i = 5; i >= 1; --i) {
        EXPECT_EQ(original.pop(), i);
        EXPECT_EQ(copy.pop(), i);
    }
}


struct EmplaceTest {
    int x, y;
    EmplaceTest(const int a, const int b) : x(a), y(b) {}
};

TEST_F(StackUnitTest, EmplaceConstructsInPlace) {
    Stack<EmplaceTest> stack;
    stack.emplace(3, 4);

    EXPECT_EQ(stack.size(), 1);
    EXPECT_EQ(stack.top().x, 3);
    EXPECT_EQ(stack.top().y, 4);
}
