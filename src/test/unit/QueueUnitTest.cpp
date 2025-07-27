#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "Queue.hpp"
#include "ThrowingType.hpp"


class QueueUnitTest : public ::testing::Test {
  protected:
    void SetUp() override {
        // Setup code that runs before each test
    }

    void TearDown() override {
        // Cleanup code that runs after each test
    }
};


TEST_F(QueueUnitTest, DefaultConstructor) {
    const Queue<int> queue;

    EXPECT_TRUE(queue.isEmpty());
    EXPECT_EQ(queue.size(), 0);
}


TEST_F(QueueUnitTest, CopyConstructor) {
    Queue<int> original;
    original.enqueue(10);
    original.enqueue(20);
    original.enqueue(30);

    Queue copy(original);

    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(copy.front(), 10);
    EXPECT_EQ(copy.back(), 30);

    // Verify deep copy - modify original shouldn't affect copy
    original.enqueue(999);
    EXPECT_NE(copy.size(), original.size());
    EXPECT_EQ(copy.back(), 30);
}


TEST_F(QueueUnitTest, CopyConstructorWithEmptyQueue) {
    const Queue<int> original;
    const Queue copy(original);

    EXPECT_TRUE(copy.isEmpty());
    EXPECT_EQ(copy.size(), 0);
}


TEST_F(QueueUnitTest, MoveConstructor) {
    Queue<int> original;
    original.enqueue(10);
    original.enqueue(20);
    original.enqueue(30);

    const std::size_t original_size = original.size();

    Queue moved(std::move(original));

    EXPECT_EQ(moved.size(), original_size);
    EXPECT_EQ(moved.front(), 10);
    EXPECT_EQ(moved.back(), 30);
    EXPECT_TRUE(original.isEmpty());
}


TEST_F(QueueUnitTest, CopyAssignmentOperator) {
    Queue<int> original;
    original.enqueue(10);
    original.enqueue(20);
    original.enqueue(30);

    Queue<int> copy;
    copy = original;

    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(copy.front(), 10);
    EXPECT_EQ(copy.back(), 30);

    // Verify deep copy
    original.enqueue(999);
    EXPECT_NE(copy.size(), original.size());
    EXPECT_EQ(copy.back(), 30);
}


TEST_F(QueueUnitTest, CopyAssignmentOperatorSelfAssignment) {
    Queue<int> queue;
    queue.enqueue(10);
    queue.enqueue(20);

    queue = queue; // Self assignment

    EXPECT_EQ(queue.size(), 2);
    EXPECT_EQ(queue.front(), 10);
    EXPECT_EQ(queue.back(), 20);
}


TEST_F(QueueUnitTest, MoveAssignmentOperator) {
    Queue<int> original;
    original.enqueue(10);
    original.enqueue(20);
    original.enqueue(30);

    Queue<int> moved;
    const std::size_t original_size = original.size();

    moved = std::move(original);

    EXPECT_EQ(moved.size(), original_size);
    EXPECT_EQ(moved.front(), 10);
    EXPECT_EQ(moved.back(), 30);
    EXPECT_TRUE(original.isEmpty());
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wself-move"
TEST_F(QueueUnitTest, MoveAssignmentOperatorSelfAssignment) {
    Queue<int> queue;
    queue.enqueue(10);
    queue.enqueue(20);

    queue = std::move(queue); // Self assignment

    EXPECT_EQ(queue.size(), 2);
    EXPECT_EQ(queue.front(), 10);
    EXPECT_EQ(queue.back(), 20);
}
#pragma GCC diagnostic pop


TEST_F(QueueUnitTest, EnqueueSingleElement) {
    Queue<int> queue;

    queue.enqueue(42);

    EXPECT_FALSE(queue.isEmpty());
    EXPECT_EQ(queue.size(), 1);
    EXPECT_EQ(queue.front(), 42);
    EXPECT_EQ(queue.back(), 42);
}


TEST_F(QueueUnitTest, EnqueueMultipleElements) {
    Queue<int> queue;

    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);

    EXPECT_EQ(queue.size(), 3);
    EXPECT_EQ(queue.front(), 10); // FIFO - first enqueued should be at front
    EXPECT_EQ(queue.back(), 30);  // Last enqueued should be at back
}


TEST_F(QueueUnitTest, EnqueueLargeNumberOfElements) {
    Queue<int> queue;
    constexpr int num_elements = 1000;

    for (int i = 0; i < num_elements; ++i)
        queue.enqueue(i);

    EXPECT_EQ(queue.size(), num_elements);
    EXPECT_EQ(queue.front(), 0);
    EXPECT_EQ(queue.back(), num_elements - 1);
}


TEST_F(QueueUnitTest, DequeueSingleElement) {
    Queue<int> queue;
    queue.enqueue(42);

    const int dequeued = queue.dequeue();

    EXPECT_EQ(dequeued, 42);
    EXPECT_TRUE(queue.isEmpty());
    EXPECT_EQ(queue.size(), 0);
}


TEST_F(QueueUnitTest, DequeueMultipleElements) {
    Queue<int> queue;
    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);

    // Dequeue elements should follow FIFO order
    EXPECT_EQ(queue.dequeue(), 10);
    EXPECT_EQ(queue.size(), 2);
    EXPECT_EQ(queue.front(), 20);

    EXPECT_EQ(queue.dequeue(), 20);
    EXPECT_EQ(queue.size(), 1);
    EXPECT_EQ(queue.front(), 30);

    EXPECT_EQ(queue.dequeue(), 30);
    EXPECT_TRUE(queue.isEmpty());
}


TEST_F(QueueUnitTest, DequeueFromEmptyQueue) {
    Queue<int> queue;
    EXPECT_THROW(queue.dequeue(), std::out_of_range);
}


TEST_F(QueueUnitTest, DequeueAllElementsThenEnqueueAgain) {
    Queue<int> queue;
    queue.enqueue(10);
    queue.enqueue(20);

    queue.dequeue();
    queue.dequeue();

    EXPECT_TRUE(queue.isEmpty());

    queue.enqueue(99);
    EXPECT_EQ(queue.front(), 99);
    EXPECT_EQ(queue.back(), 99);
    EXPECT_EQ(queue.size(), 1);
}


TEST_F(QueueUnitTest, FrontNonConstAccess) {
    Queue<int> queue;
    queue.enqueue(42);

    int& front_ref = queue.front();
    EXPECT_EQ(front_ref, 42);

    // Modify through reference
    front_ref = 99;
    EXPECT_EQ(queue.front(), 99);
}


TEST_F(QueueUnitTest, FrontConstAccess) {
    Queue<int> queue;
    queue.enqueue(42);

    const Queue<int>& const_queue = queue;
    const int& front_ref = const_queue.front();

    EXPECT_EQ(front_ref, 42);
}


TEST_F(QueueUnitTest, FrontWithMultipleElements) {
    Queue<int> queue;
    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);

    EXPECT_EQ(queue.front(), 10);

    queue.dequeue();
    EXPECT_EQ(queue.front(), 20);

    queue.enqueue(99);
    EXPECT_EQ(queue.front(), 20); // Front should remain 20
}


TEST_F(QueueUnitTest, FrontFromEmptyQueue) {
    Queue<int> queue;
    EXPECT_THROW(queue.front(), std::out_of_range);
}


TEST_F(QueueUnitTest, FrontFromEmptyConstQueue) {
    const Queue<int> queue;
    EXPECT_THROW(queue.front(), std::out_of_range);
}


// ==================== BACK TESTS ====================

TEST_F(QueueUnitTest, BackNonConstAccess) {
    Queue<int> queue;
    queue.enqueue(42);

    int& back_ref = queue.back();
    EXPECT_EQ(back_ref, 42);

    // Modify through reference
    back_ref = 99;
    EXPECT_EQ(queue.back(), 99);
}


TEST_F(QueueUnitTest, BackConstAccess) {
    Queue<int> queue;
    queue.enqueue(42);

    const Queue<int>& const_queue = queue;
    const int& back_ref = const_queue.back();

    EXPECT_EQ(back_ref, 42);
}


TEST_F(QueueUnitTest, BackWithMultipleElements) {
    Queue<int> queue;
    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);

    EXPECT_EQ(queue.back(), 30);

    queue.enqueue(99);
    EXPECT_EQ(queue.back(), 99);

    queue.dequeue();
    EXPECT_EQ(queue.back(), 99); // Back should remain 99
}


TEST_F(QueueUnitTest, BackFromEmptyQueue) {
    Queue<int> queue;
    EXPECT_THROW(queue.back(), std::out_of_range);
}


TEST_F(QueueUnitTest, BackFromEmptyConstQueue) {
    const Queue<int> queue;
    EXPECT_THROW(queue.back(), std::out_of_range);
}


// ==================== UTILITY FUNCTION TESTS ====================

TEST_F(QueueUnitTest, IsEmpty) {
    Queue<int> queue;

    EXPECT_TRUE(queue.isEmpty());

    queue.enqueue(10);
    EXPECT_FALSE(queue.isEmpty());

    queue.dequeue();
    EXPECT_TRUE(queue.isEmpty());
}


TEST_F(QueueUnitTest, Size) {
    Queue<int> queue;

    EXPECT_EQ(queue.size(), 0);

    queue.enqueue(10);
    EXPECT_EQ(queue.size(), 1);

    queue.enqueue(20);
    queue.enqueue(30);
    EXPECT_EQ(queue.size(), 3);

    queue.dequeue();
    EXPECT_EQ(queue.size(), 2);

    queue.clear();
    EXPECT_EQ(queue.size(), 0);
}


TEST_F(QueueUnitTest, Capacity) {
    Queue<int> queue;

    // Initially should have default capacity
    EXPECT_GT(queue.capacity(), 0);

    // Add elements and verify capacity grows as needed
    const std::size_t initial_capacity = queue.capacity();

    for (int i = 0; i < static_cast<int>(initial_capacity * 2); ++i)
        queue.enqueue(i);

    EXPECT_GE(queue.capacity(), initial_capacity * 2);
}


TEST_F(QueueUnitTest, Clear) {
    Queue<int> queue;
    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);

    queue.clear();

    EXPECT_TRUE(queue.isEmpty());
    EXPECT_EQ(queue.size(), 0);
}


TEST_F(QueueUnitTest, ClearEmptyQueue) {
    Queue<int> queue;

    queue.clear();

    EXPECT_TRUE(queue.isEmpty());
    EXPECT_EQ(queue.size(), 0);
}


TEST_F(QueueUnitTest, PrintQueue) {
    Queue<int> queue;
    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);

    // Capture stdout
    std::ostringstream output;
    std::streambuf* orig = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    queue.print();

    // Restore stdout
    std::cout.rdbuf(orig);

    EXPECT_EQ(output.str(), "Queue (front to back): 10 20 30 \n");
}


TEST_F(QueueUnitTest, PrintEmptyQueue) {
    const Queue<int> queue;

    // Capture stdout
    std::ostringstream output;
    std::streambuf* orig = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    queue.print();

    // Restore stdout
    std::cout.rdbuf(orig);

    EXPECT_EQ(output.str(), "Queue is empty\n");
}


TEST_F(QueueUnitTest, PrintSingleElement) {
    Queue<int> queue;
    queue.enqueue(42);

    // Capture stdout
    std::ostringstream output;
    std::streambuf* orig = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    queue.print();

    // Restore stdout
    std::cout.rdbuf(orig);

    EXPECT_EQ(output.str(), "Queue (front to back): 42 \n");
}


TEST_F(QueueUnitTest, StringType) {
    Queue<std::string> queue;

    queue.enqueue("First");
    queue.enqueue("Second");
    queue.enqueue("Third");

    EXPECT_EQ(queue.size(), 3);
    EXPECT_EQ(queue.front(), "First");
    EXPECT_EQ(queue.back(), "Third");

    EXPECT_EQ(queue.dequeue(), "First");
    EXPECT_EQ(queue.dequeue(), "Second");
    EXPECT_EQ(queue.dequeue(), "Third");
}


TEST_F(QueueUnitTest, CustomObjectType) {
    struct Point {
        int x, y;
        explicit Point(int x = 0, int y = 0) : x(x), y(y) {}
        bool operator==(const Point& other) const {
            return x == other.x && y == other.y;
        }
    };

    Queue<Point> queue;
    queue.enqueue(Point(1, 2));
    queue.enqueue(Point(3, 4));

    EXPECT_EQ(queue.size(), 2);
    EXPECT_EQ(queue.front(), Point(1, 2));
    EXPECT_EQ(queue.back(), Point(3, 4));

    const Point dequeued = queue.dequeue();
    EXPECT_EQ(dequeued, Point(1, 2));
    EXPECT_EQ(queue.front(), Point(3, 4));
}


TEST_F(QueueUnitTest, FIFOBehavior) {
    Queue<int> queue;

    // Enqueue elements in order
    for (int i = 1; i <= 5; ++i)
        queue.enqueue(i * 10);

    // Dequeue elements should be in same order (FIFO)
    for (int i = 1; i <= 5; ++i)
        EXPECT_EQ(queue.dequeue(), i * 10);

    EXPECT_TRUE(queue.isEmpty());
}


TEST_F(QueueUnitTest, InterleavedEnqueueDequeue) {
    Queue<int> queue;

    queue.enqueue(10);
    queue.enqueue(20);
    EXPECT_EQ(queue.dequeue(), 10);

    queue.enqueue(30);
    queue.enqueue(40);
    EXPECT_EQ(queue.dequeue(), 20);
    EXPECT_EQ(queue.dequeue(), 30);
    EXPECT_EQ(queue.dequeue(), 40);

    EXPECT_TRUE(queue.isEmpty());
}


TEST_F(QueueUnitTest, LargeNumberOfOperations) {
    Queue<int> queue;
    constexpr int num_operations = 1000;

    // Enqueue many elements
    for (int i = 0; i < num_operations; ++i)
        queue.enqueue(i);

    EXPECT_EQ(queue.size(), num_operations);

    // Dequeue all elements and verify FIFO order
    for (int i = 0; i < num_operations; ++i)
        EXPECT_EQ(queue.dequeue(), i);

    EXPECT_TRUE(queue.isEmpty());
}


TEST_F(QueueUnitTest, AlternatingEnqueueDequeue) {
    Queue<int> queue;

    // Pattern: enqueue 2, dequeue 1, repeat
    for (int i = 0; i < 100; ++i) {
        queue.enqueue(i * 2);
        queue.enqueue(i * 2 + 1);
        if (queue.size() > 1)
            queue.dequeue();
    }

    EXPECT_GT(queue.size(), 0);

    // Verify queue is still in valid state
    int previous = queue.dequeue();
    while (!queue.isEmpty()) {
        int current = queue.dequeue();
        EXPECT_GT(current, previous);
        previous = current;
    }
}


TEST_F(QueueUnitTest, ExceptionSafetyBasic) {
    // Test basic exception handling for empty queue operations
    Queue<int> queue;

    // These should throw std::out_of_range
    EXPECT_THROW(queue.dequeue(), std::out_of_range);
    EXPECT_THROW(queue.front(), std::out_of_range);
    EXPECT_THROW(queue.back(), std::out_of_range);

    const Queue<int>& const_queue = queue;
    EXPECT_THROW(const_queue.front(), std::out_of_range);
    EXPECT_THROW(const_queue.back(), std::out_of_range);

    // Queue should still be usable after exceptions
    EXPECT_NO_THROW(queue.enqueue(42));
    EXPECT_EQ(queue.front(), 42);
    EXPECT_EQ(queue.back(), 42);
    EXPECT_EQ(queue.size(), 1);
}


TEST_F(QueueUnitTest, BasicThrowingTypeUsage) {
    // Test that ThrowingType works correctly with the queue when not throwing
    ThrowingType::reset();
    ThrowingType::should_throw = false;

    Queue<ThrowingType> queue;

    // Test basic operations
    queue.enqueue(ThrowingType(1));
    EXPECT_EQ(queue.size(), 1);
    EXPECT_EQ(queue.front().value, 1);
    EXPECT_EQ(queue.back().value, 1);

    queue.enqueue(ThrowingType(2));
    EXPECT_EQ(queue.size(), 2);
    EXPECT_EQ(queue.front().value, 1);
    EXPECT_EQ(queue.back().value, 2);

    // Test dequeue operations
    ThrowingType dequeued = queue.dequeue();
    EXPECT_EQ(dequeued.value, 1);
    EXPECT_EQ(queue.size(), 1);
    EXPECT_EQ(queue.front().value, 2);

    dequeued = queue.dequeue();
    EXPECT_EQ(dequeued.value, 2);
    EXPECT_TRUE(queue.isEmpty());

    ThrowingType::reset();
}


TEST_F(QueueUnitTest, EnqueueDequeueSingleElementMultipleTimes) {
    Queue<int> queue;

    for (int i = 0; i < 10; ++i) {
        queue.enqueue(i);
        EXPECT_EQ(queue.front(), i);
        EXPECT_EQ(queue.back(), i);
        EXPECT_EQ(queue.dequeue(), i);
        EXPECT_TRUE(queue.isEmpty());
    }
}


TEST_F(QueueUnitTest, FrontBackDoNotModifyQueue) {
    Queue<int> queue;
    queue.enqueue(10);
    queue.enqueue(20);

    // Multiple calls to front() and back() should not change the queue
    EXPECT_EQ(queue.front(), 10);
    EXPECT_EQ(queue.back(), 20);
    EXPECT_EQ(queue.front(), 10);
    EXPECT_EQ(queue.back(), 20);
    EXPECT_EQ(queue.size(), 2);

    // Queue should still work normally
    EXPECT_EQ(queue.dequeue(), 10);
    EXPECT_EQ(queue.front(), 20);
    EXPECT_EQ(queue.back(), 20);
}


TEST_F(QueueUnitTest, CopyConstructorPreservesOrder) {
    Queue<int> original;
    for (int i = 1; i <= 5; ++i)
        original.enqueue(i);

    Queue copy(original);

    // Both queues should dequeue elements in the same order
    for (int i = 1; i <= 5; ++i) {
        EXPECT_EQ(original.dequeue(), i);
        EXPECT_EQ(copy.dequeue(), i);
    }
}


TEST_F(QueueUnitTest, QueueWithSingleElementFrontBackEqual) {
    Queue<int> queue;
    queue.enqueue(42);

    EXPECT_EQ(queue.front(), queue.back());
    EXPECT_EQ(queue.front(), 42);

    // Modify front and verify back changes too (they point to same element)
    queue.front() = 99;
    EXPECT_EQ(queue.back(), 99);
}
