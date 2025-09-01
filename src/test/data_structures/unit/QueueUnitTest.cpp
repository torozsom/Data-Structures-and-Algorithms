#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include <utility>

#include "Queue.hpp"
#include "ThrowingType.hpp"


using containers::Queue;
using std::size_t;


class QueueUnitTest : public testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};


struct Record {
    int& value;
};


TEST_F(QueueUnitTest, DefaultConstructor) {
    const Queue<int> queue;

    EXPECT_TRUE(queue.isEmpty());
    EXPECT_EQ(queue.size(), 0);
}

TEST_F(QueueUnitTest, ConstructorWithInitialCapacity) {
    const Queue<int> queue(50);
    EXPECT_TRUE(queue.isEmpty());
    EXPECT_GE(queue.capacity(), 50u);
}

TEST_F(QueueUnitTest, ConstructorWithInitialData) {
    int data[] = {1, 2, 3};
    Queue queue(data, 3);
    EXPECT_EQ(queue.size(), 3);
    EXPECT_EQ(queue.front(), 1);
    EXPECT_EQ(queue.back(), 3);
}


TEST_F(QueueUnitTest, ConstructorWithNullDataThrowsException) {
    EXPECT_THROW(Queue<int> queue(nullptr, 2), std::invalid_argument);
}

TEST_F(QueueUnitTest, ConstructorWithZeroSizeAndNullData) {
    EXPECT_NO_THROW(Queue<int> queue(nullptr, 0));
    const Queue<int> queue(nullptr, 0);
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

    const size_t original_size = original.size();

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
    const size_t original_size = original.size();

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


TEST_F(QueueUnitTest, EnqueueRecordsReflectExternalChanges) {
    int first = 1;
    int second = 2;

    Queue<Record> queue;
    queue.enqueue(Record{first});
    queue.enqueue(Record{second});

    EXPECT_EQ(queue.front().value, 1);
    EXPECT_EQ(queue.back().value, 2);

    first = 10;
    second = 20;

    EXPECT_EQ(queue.front().value, 10);
    EXPECT_EQ(queue.back().value, 20);
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
    const size_t initial_capacity = queue.capacity();

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
        explicit Point(const int x = 0, const int y = 0) : x(x), y(y) {}
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


TEST_F(QueueUnitTest, WrapAroundDoesNotAllocate) {
    Queue<int> queue;
    const size_t initial_capacity = queue.capacity();

    for (size_t i = 0; i < initial_capacity; ++i)
        queue.enqueue(static_cast<int>(i));

    constexpr size_t num_dequeues = 2;
    for (size_t i = 0; i < num_dequeues; ++i)
        EXPECT_EQ(queue.dequeue(), static_cast<int>(i));

    for (size_t i = 0; i < num_dequeues; ++i)
        queue.enqueue(static_cast<int>(initial_capacity + i));

    EXPECT_EQ(queue.size(), initial_capacity);
    EXPECT_EQ(queue.front(), static_cast<int>(num_dequeues));
    EXPECT_EQ(queue.back(),
              static_cast<int>(initial_capacity + num_dequeues - 1));

    EXPECT_EQ(queue.capacity(), initial_capacity);

    for (int expected = num_dequeues;
         expected < static_cast<int>(num_dequeues + initial_capacity);
         ++expected)
        EXPECT_EQ(queue.dequeue(), expected);
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


TEST_F(QueueUnitTest, HandlesMoveOnlyTypes) {
    Queue<std::unique_ptr<int>> queue;

    queue.enqueue(std::make_unique<int>(1));
    queue.enqueue(std::make_unique<int>(2));
    queue.enqueue(std::make_unique<int>(3));

    int expected = 1;
    for (const auto& ptr : queue)
        EXPECT_EQ(*ptr, expected++);

    const auto first = queue.dequeue();
    EXPECT_EQ(*first, 1);
    EXPECT_EQ(queue.size(), 2);
    EXPECT_EQ(*queue.front(), 2);
    EXPECT_EQ(*queue.back(), 3);

    expected = 2;
    for (const auto& ptr : queue)
        EXPECT_EQ(*ptr, expected++);

    queue.dequeue();
    queue.dequeue();
    EXPECT_TRUE(queue.isEmpty());
}


TEST_F(QueueUnitTest, CapacityShrinksAfterDequeueInterval) {
    Queue<int> queue;

    constexpr int num_elements = 100;
    for (int i = 0; i < num_elements; ++i)
        queue.enqueue(i);

    const size_t initial_capacity = queue.capacity();
    constexpr size_t shrink_check_interval = 16;
    const size_t target_size = initial_capacity / 4;
    const size_t total_dequeues =
        num_elements - target_size + shrink_check_interval;

    for (size_t i = 0; i < total_dequeues; ++i)
        queue.dequeue();

    EXPECT_LT(queue.capacity(), initial_capacity);
}


TEST_F(QueueUnitTest, EmplaceBackWithStdStringArgs) {
    Queue<std::string> queue;

    queue.emplaceBack(5, 'a');
    EXPECT_EQ(queue.size(), 1);
    EXPECT_EQ(queue.front(), std::string(5, 'a'));
}


TEST_F(QueueUnitTest, EmplaceBackWithCustomType) {
    struct Record {
        int id;
        std::string name;
        double value;
        Record(const int id, std::string n, const double v)
            : id(id), name(std::move(n)), value(v) {}
    };

    Queue<Record> queue;
    queue.emplaceBack(1, "first", 1.5);
    queue.emplaceBack(2, "second", 2.5);

    EXPECT_EQ(queue.size(), 2);

    const Record& first = queue.front();
    EXPECT_EQ(first.id, 1);
    EXPECT_EQ(first.name, "first");
    EXPECT_EQ(first.value, 1.5);

    const Record& last = queue.back();
    EXPECT_EQ(last.id, 2);
    EXPECT_EQ(last.name, "second");
    EXPECT_EQ(last.value, 2.5);
}


TEST_F(QueueUnitTest, RangeBasedTraversalHandlesEmpty) {
    Queue<int> queue;
    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);

    int expected = 1;
    for (int value : queue)
        EXPECT_EQ(value, expected++);
    EXPECT_EQ(expected, 4);

    int count = 0;
    for (Queue<int> empty; const int value : empty) {
        (void)value;
        ++count;
    }
    EXPECT_EQ(count, 0);
}


TEST_F(QueueUnitTest, ManualIteratorTraversal) {
    Queue<int> queue;
    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);

    auto it = queue.begin();
    EXPECT_EQ(*it, 1);

    ++it;
    EXPECT_EQ(*it, 2);

    auto post_inc = it++;
    EXPECT_EQ(*post_inc, 2);
    EXPECT_EQ(*it, 3);

    auto post_dec = it--;
    EXPECT_EQ(*post_dec, 3);
    EXPECT_EQ(*it, 2);

    --it;
    EXPECT_EQ(*it, 1);

    Queue<int> empty;
    EXPECT_EQ(empty.begin(), empty.end());
}


struct EmplaceTest {
    int x, y;
    EmplaceTest(const int a, const int b) : x(a), y(b) {}
};

TEST_F(QueueUnitTest, EmplaceLastConstructsInPlace) {
    Queue<EmplaceTest> queue;

    for (int i = 0; i < 10; ++i)
        queue.emplaceBack(i, i + 1);

    EXPECT_EQ(queue.size(), 10);

    for (int i = 0; i < 10; ++i) {
        EmplaceTest item = queue.dequeue();
        EXPECT_EQ(item.x, i);
        EXPECT_EQ(item.y, i + 1);
    }

    EXPECT_EQ(queue.size(), 0);
}
