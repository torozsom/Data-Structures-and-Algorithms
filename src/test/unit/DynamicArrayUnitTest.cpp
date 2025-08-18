#include <cstdint>
#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include <string>


#include "DynamicArray.hpp"
#include "Record.hpp"
#include "ThrowingType.hpp"


class DynamicArrayUnitTest : public testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override { ThrowingType::reset(); }
};


TEST_F(DynamicArrayUnitTest, DefaultConstructor) {
    const DynamicArray<int> arr;
    EXPECT_EQ(arr.size(), 0u);
    EXPECT_EQ(arr.capacity(), 5u);
    EXPECT_TRUE(arr.isEmpty());
}


TEST_F(DynamicArrayUnitTest, ConstructorWithInitialData) {
    int data[] = {1, 2, 3, 4, 5};
    DynamicArray arr(data, 5);
    EXPECT_EQ(arr.size(), 5u);
    EXPECT_EQ(arr.capacity(), 5u);
    EXPECT_FALSE(arr.isEmpty());
    for (int i = 0; i < 5; ++i)
        EXPECT_EQ(arr.get(i), data[i]);
}


TEST_F(DynamicArrayUnitTest, ConstructorWithInitialDataSmallerThanDefault) {
    int data[] = {1, 2};
    const DynamicArray arr(data, 2);
    EXPECT_EQ(arr.size(), 2u);
    EXPECT_EQ(arr.capacity(), 5u);
    EXPECT_FALSE(arr.isEmpty());
}


TEST_F(DynamicArrayUnitTest, ConstructorWithNullDataThrowsException) {
    EXPECT_THROW(DynamicArray<int> arr(nullptr, 5), std::invalid_argument);
}


TEST_F(DynamicArrayUnitTest, ConstructorWithZeroSizeAndNullData) {
    EXPECT_NO_THROW(DynamicArray<int> arr(nullptr, 0));
    const DynamicArray<int> arr(nullptr, 0);
    EXPECT_EQ(arr.size(), 0u);
    EXPECT_EQ(arr.capacity(), 5u);
}


TEST_F(DynamicArrayUnitTest, CopyConstructor) {
    int data[] = {1, 2, 3};
    DynamicArray original(data, 3);
    DynamicArray copy(original);
    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(copy.capacity(), original.capacity());
    for (std::size_t i = 0; i < copy.size(); ++i)
        EXPECT_EQ(copy.get(i), original.get(i));
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
    EXPECT_EQ(original.size(), 0u);
    EXPECT_EQ(original.capacity(), 0u);
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
    for (std::size_t i = 0; i < copy.size(); ++i)
        EXPECT_EQ(copy.get(i), original.get(i));
    original = original;
    EXPECT_EQ(original.size(), 3u);
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
    EXPECT_EQ(original.size(), 0u);
    EXPECT_EQ(original.capacity(), 0u);
    moved = std::move(moved);
    EXPECT_EQ(moved.size(), original_size);
    for (int i = 0; i < 3; ++i)
        EXPECT_EQ(moved.get(i), data[i]);
}


TEST_F(DynamicArrayUnitTest, MoveAssignmentTransfersUniquePtrOwnership) {
    DynamicArray<std::unique_ptr<int>> original;
    original.addLast(std::make_unique<int>(1));
    original.addLast(std::make_unique<int>(2));
    const std::size_t original_size = original.size();
    const std::size_t original_capacity = original.capacity();
    DynamicArray<std::unique_ptr<int>> moved;
    moved = std::move(original);
    EXPECT_EQ(moved.size(), original_size);
    EXPECT_EQ(moved.capacity(), original_capacity);
    EXPECT_EQ(*moved.get(0), 1);
    EXPECT_EQ(*moved.get(1), 2);
    EXPECT_EQ(original.size(), 0u);
    EXPECT_EQ(original.capacity(), 0u);
}


TEST_F(DynamicArrayUnitTest, IsEmpty) {
    DynamicArray<int> arr;
    EXPECT_TRUE(arr.isEmpty());
    arr.addLast(10);
    EXPECT_FALSE(arr.isEmpty());
    arr.removeLast();
    EXPECT_TRUE(arr.isEmpty());
}


TEST_F(DynamicArrayUnitTest, InsertAtValidIndex) {
    DynamicArray<int> arr;
    arr.addLast(10);
    arr.addLast(30);
    arr.insert(20, 1);
    EXPECT_EQ(arr.size(), 3u);
    EXPECT_EQ(arr.get(0), 10);
    EXPECT_EQ(arr.get(1), 20);
    EXPECT_EQ(arr.get(2), 30);
}


TEST_F(DynamicArrayUnitTest, InsertAtBeginning) {
    DynamicArray<int> arr;
    arr.addLast(20);
    arr.addLast(30);
    arr.insert(10, 0);
    EXPECT_EQ(arr.size(), 3u);
    EXPECT_EQ(arr.get(0), 10);
    EXPECT_EQ(arr.get(1), 20);
    EXPECT_EQ(arr.get(2), 30);
}


TEST_F(DynamicArrayUnitTest, InsertAtEnd) {
    DynamicArray<int> arr;
    arr.addLast(10);
    arr.addLast(20);
    arr.insert(30, 2);
    EXPECT_EQ(arr.size(), 3u);
    EXPECT_EQ(arr.get(2), 30);
}


TEST_F(DynamicArrayUnitTest, InsertAtInvalidIndex) {
    DynamicArray<int> arr;
    arr.addLast(10);
    EXPECT_THROW(arr.insert(20, 2), std::out_of_range);
}


TEST_F(DynamicArrayUnitTest, CapacityExpansionOnInsertion) {
    DynamicArray<int> arr;
    const std::size_t initial_capacity = arr.capacity();
    for (std::size_t i = 0; i < initial_capacity; ++i)
        arr.addLast(i);
    EXPECT_EQ(arr.capacity(), initial_capacity);
    arr.addLast(999);
    EXPECT_GE(arr.capacity(), initial_capacity * 2);
    EXPECT_EQ(arr.size(), initial_capacity + 1);
    EXPECT_EQ(arr.get(initial_capacity), 999);
}


TEST_F(DynamicArrayUnitTest, ReserveThenInsertPreservesAndUsesStorage) {
    DynamicArray<int> arr;
    arr.addLast(1);
    arr.reserve(128);
    EXPECT_GE(arr.capacity(), 128u);
    arr.insert(0, 0);
    EXPECT_EQ(arr.size(), 2u);
    EXPECT_EQ(arr.get(0), 0);
    EXPECT_EQ(arr.get(1), 1);
}


struct MoveThrowOnce {
    static int counter;
    int v;
    explicit MoveThrowOnce(const int x = 0) : v(x) {}
    MoveThrowOnce(MoveThrowOnce&& o) : v(o.v) {
        if (counter++ == 0)
            throw std::runtime_error("boom");
    }
    MoveThrowOnce& operator=(MoveThrowOnce&& o) {
        v = o.v;
        return *this;
    }
    MoveThrowOnce(const MoveThrowOnce&) = delete;
    MoveThrowOnce& operator=(const MoveThrowOnce&) = delete;
};
int MoveThrowOnce::counter = 0;


TEST_F(DynamicArrayUnitTest, InsertShiftRollbackOnMoveCtorThrow) {
    DynamicArray<MoveThrowOnce> arr;
    arr.reserve(10);
    for (int i = 0; i < 5; ++i)
        arr.emplaceLast(i);
    MoveThrowOnce::counter = 0;
    EXPECT_THROW(arr.insert(MoveThrowOnce(99), 2), std::runtime_error);
    EXPECT_EQ(arr.size(), 5u);
    for (int i = 0; i < 5; ++i)
        EXPECT_EQ(arr.get(i).v, i);
}


struct MoveCtorThrowOn2 {
    static int counter;
    int v;
    explicit MoveCtorThrowOn2(const int x = 0) : v(x) {}
    MoveCtorThrowOn2(const MoveCtorThrowOn2&) = delete;
    MoveCtorThrowOn2& operator=(const MoveCtorThrowOn2&) = delete;
    MoveCtorThrowOn2(MoveCtorThrowOn2&& other) : v(other.v) {
        if (++counter == 2)
            throw std::runtime_error("relocate move throw");
    }
    MoveCtorThrowOn2& operator=(MoveCtorThrowOn2&&) = delete;
};
int MoveCtorThrowOn2::counter = 0;


TEST_F(DynamicArrayUnitTest, RemoveAtRelocationMayThrowBasicGuarantee) {
    DynamicArray<MoveCtorThrowOn2> arr;
    for (int i = 0; i < 5; ++i)
        arr.emplaceLast(i);
    MoveCtorThrowOn2::counter = 0;
    EXPECT_THROW((void)arr.removeAt(1), std::runtime_error);
    EXPECT_NO_THROW(arr.removeAll());
    EXPECT_EQ(arr.size(), 0u);
}


TEST_F(DynamicArrayUnitTest, AddLast) {
    DynamicArray<int> arr;
    arr.addLast(10);
    EXPECT_EQ(arr.size(), 1u);
    EXPECT_EQ(arr.get(0), 10);
    arr.addLast(20);
    EXPECT_EQ(arr.size(), 2u);
    EXPECT_EQ(arr.get(1), 20);
}


TEST_F(DynamicArrayUnitTest, AddFirst) {
    DynamicArray<int> arr;
    arr.addFirst(10);
    EXPECT_EQ(arr.size(), 1u);
    EXPECT_EQ(arr.get(0), 10);
    arr.addFirst(5);
    EXPECT_EQ(arr.size(), 2u);
    EXPECT_EQ(arr.get(0), 5);
    EXPECT_EQ(arr.get(1), 10);
}


TEST_F(DynamicArrayUnitTest, AddFirstReallocationPreservesOrder) {
    DynamicArray<int> arr;
    for (int i = 1; i <= 5; ++i)
        arr.addLast(i);
    arr.addFirst(0);
    ASSERT_EQ(arr.size(), 6u);
    for (int i = 0; i < 6; ++i)
        EXPECT_EQ(arr.get(i), i);
}


TEST_F(DynamicArrayUnitTest, AddLastStrongGuaranteeOnCtorThrow) {
    DynamicArray<ThrowingType> arr;
    for (int i = 0; i < 3; ++i)
        arr.addLast(ThrowingType(i));
    const std::size_t old_size = arr.size();
    ThrowingType::should_throw = true;
    EXPECT_THROW(arr.addLast(ThrowingType(999)), std::runtime_error);
    EXPECT_EQ(arr.size(), old_size);
    ThrowingType::should_throw = false;
}


TEST_F(DynamicArrayUnitTest, RemoveAt) {
    int data[] = {10, 20, 30, 40};
    DynamicArray arr(data, 4);
    const int removed = arr.removeAt(1);
    EXPECT_EQ(removed, 20);
    EXPECT_EQ(arr.size(), 3u);
    EXPECT_EQ(arr.get(0), 10);
    EXPECT_EQ(arr.get(1), 30);
    EXPECT_EQ(arr.get(2), 40);
}


TEST_F(DynamicArrayUnitTest, RemoveAtFrontAndBack) {
    DynamicArray<int> arr;
    for (int i = 0; i < 5; ++i)
        arr.addLast(i);
    EXPECT_EQ(arr.removeAt(0), 0);
    EXPECT_EQ(arr.get(0), 1);
    EXPECT_EQ(arr.removeAt(arr.size() - 1), 4);
    EXPECT_EQ(arr.get(arr.size() - 1), 3);
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


struct MoveAssignThrowOnce {
    static int counter;
    int v;
    explicit MoveAssignThrowOnce(const int x = 0) : v(x) {}
    MoveAssignThrowOnce(MoveAssignThrowOnce&&) = default;
    MoveAssignThrowOnce& operator=(MoveAssignThrowOnce&& o) {
        if (counter++ == 0)
            throw std::runtime_error("assign-throw");
        v = o.v;
        return *this;
    }
    MoveAssignThrowOnce(const MoveAssignThrowOnce&) = delete;
    MoveAssignThrowOnce& operator=(const MoveAssignThrowOnce&) = delete;
};
int MoveAssignThrowOnce::counter = 0;


TEST_F(DynamicArrayUnitTest, RemoveAtShiftLeftMayThrowBasicGuarantee) {
    DynamicArray<MoveAssignThrowOnce> arr;
    for (int i = 0; i < 5; ++i)
        arr.emplaceLast(i);
    MoveAssignThrowOnce::counter = 0;
    EXPECT_THROW((void)arr.removeAt(1), std::runtime_error);
    EXPECT_NO_THROW(arr.removeAll());
    EXPECT_EQ(arr.size(), 0u);
}


TEST_F(DynamicArrayUnitTest, RemoveFirst) {
    int data[] = {10, 20, 30};
    DynamicArray arr(data, 3);
    const int removed = arr.removeFirst();
    EXPECT_EQ(removed, 10);
    EXPECT_EQ(arr.size(), 2u);
    EXPECT_EQ(arr.get(0), 20);
    EXPECT_EQ(arr.get(1), 30);
}


TEST_F(DynamicArrayUnitTest, RemoveFirstFromEmptyThrows) {
    DynamicArray<int> arr;
    EXPECT_THROW(arr.removeFirst(), std::out_of_range);
}


TEST_F(DynamicArrayUnitTest, RemoveLast) {
    int data[] = {10, 20, 30};
    DynamicArray arr(data, 3);
    const int removed = arr.removeLast();
    EXPECT_EQ(removed, 30);
    EXPECT_EQ(arr.size(), 2u);
    EXPECT_EQ(arr.get(0), 10);
    EXPECT_EQ(arr.get(1), 20);
}


TEST_F(DynamicArrayUnitTest, RemoveLastFromEmptyThrows) {
    DynamicArray<int> arr;
    EXPECT_THROW(arr.removeLast(), std::out_of_range);
}


TEST_F(DynamicArrayUnitTest, CapacityReductionOnRemoval) {
    DynamicArray<int> arr;
    for (int i = 0; i < 10; ++i)
        arr.addLast(i);
    const std::size_t expanded_capacity = arr.capacity();
    while (arr.size() > expanded_capacity / 4)
        arr.removeLast();
    arr.removeLast();
    EXPECT_LT(arr.capacity(), expanded_capacity);
    EXPECT_EQ(arr.capacity(), expanded_capacity / 2);
}


TEST_F(DynamicArrayUnitTest, CapacityDoesNotReduceBelowDefault) {
    DynamicArray<int> arr;
    arr.addLast(1);
    arr.addLast(2);
    arr.removeLast();
    arr.removeLast();
    EXPECT_EQ(arr.capacity(), 5u);
}


TEST_F(DynamicArrayUnitTest, RemoveAllKeepsCapacityAndZeroesSize) {
    DynamicArray<int> arr;
    for (int i = 0; i < 12; ++i)
        arr.addLast(i);
    const std::size_t cap = arr.capacity();
    arr.removeAll();
    EXPECT_EQ(arr.size(), 0u);
    EXPECT_EQ(arr.capacity(), cap);
    arr.addLast(42);
    EXPECT_EQ(arr.get(0), 42);
}


struct NoAssign {
    int v;
    explicit NoAssign(const int x = 0) : v(x) {}
    NoAssign(const NoAssign&) = default;
    NoAssign(NoAssign&&) noexcept = default;
    NoAssign& operator=(const NoAssign&) = delete;
    NoAssign& operator=(NoAssign&&) = delete;
};


TEST_F(DynamicArrayUnitTest, RemoveAtRelocationForNonAssignableType) {
    DynamicArray<NoAssign> arr;
    for (int i = 0; i < 5; ++i)
        arr.emplaceLast(i);
    const NoAssign removed = arr.removeAt(1);
    EXPECT_EQ(removed.v, 1);
    ASSERT_EQ(arr.size(), 4u);
    EXPECT_EQ(arr.get(0).v, 0);
    EXPECT_EQ(arr.get(1).v, 2);
    EXPECT_EQ(arr.get(2).v, 3);
    EXPECT_EQ(arr.get(3).v, 4);
}


TEST_F(DynamicArrayUnitTest, RemoveAtReturnsMoveOnlyValue) {
    DynamicArray<std::unique_ptr<int>> arr;
    arr.addLast(std::make_unique<int>(7));
    arr.addLast(std::make_unique<int>(9));
    const auto p = arr.removeAt(0);
    ASSERT_TRUE(p != nullptr);
    EXPECT_EQ(*p, 7);
    ASSERT_EQ(arr.size(), 1u);
    EXPECT_EQ(*arr.get(0), 9);
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


TEST_F(DynamicArrayUnitTest, BracketOperatorThrowsOutOfRange) {
    DynamicArray<int> arr;
    arr.addLast(7);
    EXPECT_THROW((void)arr[1], std::out_of_range);
    const DynamicArray carr(arr);
    EXPECT_THROW((void)carr[2], std::out_of_range);
}


TEST_F(DynamicArrayUnitTest, GetFirst) {
    int data[] = {10, 20, 30};
    DynamicArray arr(data, 3);
    EXPECT_EQ(arr.getFirst(), 10);
    arr.getFirst() = 99;
    EXPECT_EQ(arr.getFirst(), 99);
}


TEST_F(DynamicArrayUnitTest, ConstGetFirst) {
    int data[] = {10, 20, 30};
    const DynamicArray arr(data, 3);
    const int first = arr.getFirst();
    EXPECT_EQ(first, 10);
}


TEST_F(DynamicArrayUnitTest, GetFirstFromEmptyArray) {
    DynamicArray<int> arr;
    EXPECT_THROW(arr.getFirst(), std::out_of_range);
}


TEST_F(DynamicArrayUnitTest, GetLast) {
    int data[] = {10, 20, 30};
    DynamicArray arr(data, 3);
    EXPECT_EQ(arr.getLast(), 30);
    arr.getLast() = 99;
    EXPECT_EQ(arr.getLast(), 99);
}


TEST_F(DynamicArrayUnitTest, GetLastFromEmptyArray) {
    DynamicArray<int> arr;
    EXPECT_THROW(arr.getLast(), std::out_of_range);
}


TEST_F(DynamicArrayUnitTest, ConstGetFirstFromEmptyThrows) {
    const DynamicArray<int> arr;
    EXPECT_THROW(arr.getFirst(), std::out_of_range);
}


TEST_F(DynamicArrayUnitTest, ConstGetLastFromEmptyThrows) {
    const DynamicArray<int> arr;
    EXPECT_THROW(arr.getLast(), std::out_of_range);
}


struct EmplaceTest {
    int x, y;
    EmplaceTest(const int a, const int b) : x(a), y(b) {}
};


TEST_F(DynamicArrayUnitTest, EmplaceAtMidAndBegin) {
    DynamicArray<std::pair<int, int>> arr;
    arr.emplaceLast(1, 10);
    arr.emplaceAt(0, 0, 0);
    arr.emplaceAt(1, 7, 70);
    EXPECT_EQ(arr.size(), 3u);
    EXPECT_EQ(arr.get(0).first, 0);
    EXPECT_EQ(arr.get(1).second, 70);
    EXPECT_EQ(arr.get(2).first, 1);
}


TEST_F(DynamicArrayUnitTest, EmplaceAtInvalidIndexThrows) {
    DynamicArray<int> arr;
    arr.addLast(1);
    EXPECT_THROW(arr.emplaceAt(3, 99), std::out_of_range);
}


TEST_F(DynamicArrayUnitTest, EmplaceLastConstructsInPlace) {
    DynamicArray<EmplaceTest> arr;
    arr.emplaceLast(3, 4);
    EXPECT_EQ(arr.size(), 1u);
    EXPECT_EQ(arr[0].x, 3);
    EXPECT_EQ(arr[0].y, 4);
}


struct CtorMaybeThrow {
    static bool should_throw;
    int v{};
    explicit CtorMaybeThrow(const int x) : v(x) {
        if (should_throw)
            throw std::runtime_error("ctor boom");
    }
};
bool CtorMaybeThrow::should_throw = false;


TEST_F(DynamicArrayUnitTest, InsertStrongGuaranteeOnNewElementCtorThrow) {
    DynamicArray<CtorMaybeThrow> arr;
    CtorMaybeThrow::should_throw = false;
    arr.emplaceLast(1);
    arr.emplaceLast(2);
    arr.emplaceLast(3);

    CtorMaybeThrow::should_throw = true;
    const std::size_t old_size = arr.size();
    EXPECT_THROW(arr.insert(CtorMaybeThrow(99), 1), std::runtime_error);
    EXPECT_EQ(arr.size(), old_size);
    EXPECT_EQ(arr.get(0).v, 1);
    EXPECT_EQ(arr.get(1).v, 2);
    EXPECT_EQ(arr.get(2).v, 3);
    CtorMaybeThrow::should_throw = false;
}


TEST_F(DynamicArrayUnitTest, EmplaceStrongGuaranteeOnNewElementCtorThrow) {
    DynamicArray<CtorMaybeThrow> arr;
    CtorMaybeThrow::should_throw = false;
    arr.emplaceLast(10);
    arr.emplaceLast(20);
    CtorMaybeThrow::should_throw = true;
    const std::size_t old_size = arr.size();
    EXPECT_THROW(arr.emplaceAt(1, 42), std::runtime_error);
    EXPECT_EQ(arr.size(), old_size);
    EXPECT_EQ(arr.get(0).v, 10);
    EXPECT_EQ(arr.get(1).v, 20);
    CtorMaybeThrow::should_throw = false;
}


TEST_F(DynamicArrayUnitTest, InsertAtBeginningWithReallocationPreservesOrder) {
    DynamicArray<int> arr;
    for (int i = 1; i <= 5; ++i)
        arr.addLast(i);
    arr.insert(0, 0);
    ASSERT_EQ(arr.size(), 6u);
    for (int i = 0; i < 6; ++i)
        EXPECT_EQ(arr.get(i), i);
}


TEST_F(DynamicArrayUnitTest, EmplaceAtMiddleWithReallocationPreservesOrder) {
    DynamicArray<std::pair<int, int>> arr;
    for (int i = 0; i < 5; ++i)
        arr.emplaceLast(i, i * 10);
    arr.emplaceAt(2, 99, 990);
    ASSERT_EQ(arr.size(), 6u);
    EXPECT_EQ(arr.get(2).first, 99);
    EXPECT_EQ(arr.get(1).first, 1);
    EXPECT_EQ(arr.get(3).first, 2);
}


struct alignas(64) Aligned64 {
    int x;
};


TEST_F(DynamicArrayUnitTest, AllocationAlignmentRespected) {
    DynamicArray<Aligned64> arr;
    arr.emplaceLast(Aligned64{42});
    auto* p = &arr.get(0);
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(p) % alignof(Aligned64), 0u);
}


TEST_F(DynamicArrayUnitTest, AlignmentPreservedAfterGrow) {
    DynamicArray<Aligned64> arr;
    for (int i = 0; i < 16; ++i)
        arr.emplaceLast(Aligned64{i});
    for (std::size_t i = 0; i < arr.size(); ++i)
        EXPECT_EQ(reinterpret_cast<std::uintptr_t>(&arr.get(i)) %
                      alignof(Aligned64),
                  0u);
}


struct CopyThrowOnSecondCopy {
    int v{};
    mutable int copied_from_count{0};
    CopyThrowOnSecondCopy() = default;
    explicit CopyThrowOnSecondCopy(const int x) : v(x) {}
    CopyThrowOnSecondCopy(const CopyThrowOnSecondCopy& other) : v(other.v) {
        if (other.copied_from_count++ >= 1)
            throw std::runtime_error("backup copy throws");
    }
    CopyThrowOnSecondCopy(CopyThrowOnSecondCopy&& other) noexcept(false)
        : v(other.v) {}
};


TEST_F(DynamicArrayUnitTest,
       Emplace_NewElementCtorThrows_StrongGuarantee_RemovedDuplicate) {
    SUCCEED();
}


TEST_F(DynamicArrayUnitTest, ReserveIncreasesCapacityOnly) {
    DynamicArray<int> arr;
    arr.addLast(1);
    const std::size_t old_size = arr.size();
    arr.reserve(100);
    EXPECT_GE(arr.capacity(), 100u);
    EXPECT_EQ(arr.size(), old_size);
    EXPECT_EQ(arr.get(0), 1);
}


TEST_F(DynamicArrayUnitTest, ReserveSmallerThanSizeDoesNothing) {
    DynamicArray<int> arr;
    for (int i = 0; i < 5; ++i)
        arr.addLast(i);
    EXPECT_NO_THROW(arr.reserve(3));
    EXPECT_EQ(arr.size(), 5u);
}


TEST_F(DynamicArrayUnitTest, ReserveThrowsOnExcessivelyLargeCapacity) {
    DynamicArray<int> arr;
    constexpr std::size_t too_large = SIZE_MAX / sizeof(int) + 1;
    EXPECT_THROW(arr.reserve(too_large), std::bad_alloc);
}


TEST_F(DynamicArrayUnitTest, ReserveCleansUpOnCopyException) {
    DynamicArray<ThrowingType> arr;
    for (int i = 0; i < 4; ++i)
        arr.addLast(ThrowingType(i));
    ThrowingType::should_throw = true;
    EXPECT_THROW(arr.reserve(20), std::runtime_error);
    EXPECT_EQ(arr.size(), 4u);
}


TEST_F(DynamicArrayUnitTest, ResizeToSameCapacityDoesNothing) {
    DynamicArray<int> arr;
    for (int i = 0; i < 5; ++i)
        arr.addLast(i);
    const std::size_t current_capacity = arr.capacity();
    arr.reserve(current_capacity);
    EXPECT_EQ(arr.capacity(), current_capacity);
}


TEST_F(DynamicArrayUnitTest, ResizeToLessThanDefaultCapacitySetsToDefault) {
    DynamicArray<int> arr;
    for (int i = 0; i < 3; ++i)
        arr.addLast(i);
    for (int i = 0; i < 3; ++i)
        arr.removeLast();
    arr.shrinkToFit();
    EXPECT_EQ(arr.capacity(), 5u);
}


TEST_F(DynamicArrayUnitTest, ReserveSmallerThanCapacityIsNoopForCapacity) {
    DynamicArray<int> arr;
    for (int i = 0; i < 8; ++i)
        arr.addLast(i);
    const std::size_t cap = arr.capacity();
    arr.reserve(cap - 1);
    EXPECT_EQ(arr.capacity(), cap);
    EXPECT_EQ(arr.size(), 8u);
}


struct CopyThrowOnN {
    static int counter;
    int v{};
    CopyThrowOnN() = default;
    explicit CopyThrowOnN(const int x) : v(x) {}
    CopyThrowOnN(CopyThrowOnN&& other) noexcept(false) : v(other.v) {}
    CopyThrowOnN& operator=(CopyThrowOnN&& other) noexcept {
        v = other.v;
        return *this;
    }
    CopyThrowOnN(const CopyThrowOnN& other) : v(other.v) {
        if (++counter == 2)
            throw std::runtime_error("copy boom");
    }
    CopyThrowOnN& operator=(const CopyThrowOnN&) = delete;
};
int CopyThrowOnN::counter = 0;


struct MoveOnlyThrowOnN {
    static int counter;
    int v{};
    MoveOnlyThrowOnN() = default;
    explicit MoveOnlyThrowOnN(const int x) : v(x) {}
    MoveOnlyThrowOnN(const MoveOnlyThrowOnN&) = delete;
    MoveOnlyThrowOnN& operator=(const MoveOnlyThrowOnN&) = delete;
    MoveOnlyThrowOnN(MoveOnlyThrowOnN&& other) noexcept(false) : v(other.v) {
        if (++counter == 3)
            throw std::runtime_error("move boom");
    }
    MoveOnlyThrowOnN& operator=(MoveOnlyThrowOnN&& other) noexcept(false) {
        v = other.v;
        return *this;
    }
};
int MoveOnlyThrowOnN::counter = 0;


TEST_F(DynamicArrayUnitTest, ResizeCopyHelperCatchCleansUp) {
    DynamicArray<CopyThrowOnN> arr;
    for (int i = 0; i < 4; ++i)
        arr.emplaceLast(i);
    CopyThrowOnN::counter = 0;
    EXPECT_THROW(arr.reserve(16), std::runtime_error);
    EXPECT_EQ(arr.size(), 4u);
    for (int i = 0; i < 4; ++i)
        EXPECT_EQ(arr.get(i).v, i);
}


TEST_F(DynamicArrayUnitTest, ResizeMoveHelperCatchCleansUp) {
    DynamicArray<MoveOnlyThrowOnN> arr;
    for (int i = 0; i < 5; ++i)
        arr.emplaceLast(i);
    MoveOnlyThrowOnN::counter = 0;
    EXPECT_THROW(arr.reserve(16), std::runtime_error);
    EXPECT_EQ(arr.size(), 5u);
    for (int i = 0; i < 5; ++i)
        EXPECT_EQ(arr.get(i).v, i);
}


TEST_F(DynamicArrayUnitTest, ShrinkToFit) {
    DynamicArray<int> arr;
    for (int i = 0; i < 20; ++i)
        arr.addLast(i);
    for (int i = 0; i < 15; ++i)
        arr.removeLast();
    const std::size_t old_capacity = arr.capacity();
    arr.shrinkToFit();
    EXPECT_GE(old_capacity, arr.capacity());
    EXPECT_EQ(arr.size(), 5u);
    for (int i = 0; i < 5; ++i)
        EXPECT_EQ(arr.get(i), i);
}


TEST_F(DynamicArrayUnitTest, ShrinkToFitReducesToDefaultCapacity) {
    DynamicArray<int> arr;
    for (int i = 0; i < 32; ++i)
        arr.addLast(i);
    for (int i = 0; i < 31; ++i)
        arr.removeLast();
    const std::size_t cap_before = arr.capacity();
    arr.shrinkToFit();
    EXPECT_EQ(arr.size(), 1u);
    EXPECT_LE(arr.capacity(), cap_before);
    EXPECT_GE(arr.capacity(), 5u);
}


TEST_F(DynamicArrayUnitTest, ShrinkToFitToExactSize) {
    DynamicArray<int> arr;
    for (int i = 0; i < 12; ++i)
        arr.addLast(i);
    for (int i = 0; i < 3; ++i)
        arr.removeLast();
    const std::size_t before = arr.capacity();
    arr.shrinkToFit();
    EXPECT_LE(arr.capacity(), before);
    EXPECT_GE(arr.capacity(), arr.size());
    EXPECT_EQ(arr.size(), 9u);
}


TEST_F(DynamicArrayUnitTest, CloneCreatesDeepCopy) {
    DynamicArray<std::string> arr;
    arr.addLast("apple");
    arr.addLast("banana");
    DynamicArray<std::string> clone = arr.clone();
    EXPECT_EQ(clone.size(), 2u);
    EXPECT_EQ(clone.get(0), "apple");
    EXPECT_EQ(clone.get(1), "banana");
    arr[0] = "modified";
    EXPECT_NE(arr[0], clone[0]);
}


struct Counted {
    static int alive;
    int v;
    explicit Counted(const int x = 0) : v(x) { ++alive; }
    Counted(const Counted& o) : v(o.v) { ++alive; }
    ~Counted() { --alive; }
};
int Counted::alive = 0;


TEST_F(DynamicArrayUnitTest, CloneLifetimeIndependence) {
    {
        DynamicArray<Counted> a;
        a.emplaceLast(1);
        a.emplaceLast(2);
        EXPECT_EQ(Counted::alive, 2);
        auto b = a.clone();
        EXPECT_EQ(Counted::alive, 4);
        a.removeLast();
        EXPECT_EQ(Counted::alive, 3);
        EXPECT_EQ(b.size(), 2u);
        EXPECT_EQ(b.get(1).v, 2);
    }
    EXPECT_EQ(Counted::alive, 0);
}


TEST_F(DynamicArrayUnitTest, CloneCopyThrowsLeavesOriginalIntact) {
    DynamicArray<CopyThrowOnN> a;
    for (int i = 0; i < 3; ++i)
        a.emplaceLast(i);
    CopyThrowOnN::counter = 1;
    EXPECT_THROW((void)a.clone(), std::runtime_error);
    ASSERT_EQ(a.size(), 3u);
    for (int i = 0; i < 3; ++i)
        EXPECT_EQ(a.get(i).v, i);
}


TEST_F(DynamicArrayUnitTest, RangeBasedForLoop) {
    int sum = 0;
    DynamicArray<int> arr;
    for (int i = 1; i <= 5; ++i)
        arr.addLast(i);
    for (const int value : arr)
        sum += value;
    EXPECT_EQ(sum, 15);
}


TEST_F(DynamicArrayUnitTest, ConstRangeBasedForLoop) {
    DynamicArray<int> arr;
    for (int i = 0; i < 3; ++i)
        arr.addLast(i + 1);
    const DynamicArray<int>& const_arr = arr;
    int expected = 1;
    for (const int val : const_arr)
        EXPECT_EQ(val, expected++);
}


TEST_F(DynamicArrayUnitTest, RangeForOnMovedFromIsNoop) {
    DynamicArray<int> a;
    a.addLast(1);
    DynamicArray<int> b = std::move(a);
    int count = 0;
    for (const int x : a) {
        (void)x;
        ++count;
    }
    EXPECT_EQ(count, 0);
}


TEST_F(DynamicArrayUnitTest, IteratorsEmptyAndConst) {
    DynamicArray<int> a;
    EXPECT_EQ(a.begin(), a.end());
    a.addLast(1);
    const auto& ca = a;
    EXPECT_EQ(*ca.cbegin(), 1);
    EXPECT_EQ(std::distance(ca.cbegin(), ca.cend()), 1);
}


TEST_F(DynamicArrayUnitTest, Clear) {
    int data[] = {10, 20, 30, 40, 50};
    DynamicArray arr(data, 5);
    arr.clear();
    EXPECT_EQ(arr.size(), 0u);
    EXPECT_EQ(arr.capacity(), 5u);
    EXPECT_TRUE(arr.isEmpty());
}


TEST_F(DynamicArrayUnitTest, ClearEmptyArray) {
    DynamicArray<int> arr;
    arr.clear();
    EXPECT_EQ(arr.size(), 0u);
    EXPECT_EQ(arr.capacity(), 5u);
    EXPECT_TRUE(arr.isEmpty());
}


TEST_F(DynamicArrayUnitTest, ClearAfterException) {
    DynamicArray<ThrowingType> arr;
    for (int i = 0; i < 4; ++i)
        arr.addLast(ThrowingType(i));
    ThrowingType::should_throw = true;
    EXPECT_THROW(arr.addLast(ThrowingType(999)), std::runtime_error);
    arr.clear();
    EXPECT_EQ(arr.size(), 0u);
    EXPECT_TRUE(arr.isEmpty());
    EXPECT_EQ(arr.capacity(), 5u);
}


struct DestructCounter {
    static int destruct_count;
    static void reset() { destruct_count = 0; }
    ~DestructCounter() { ++destruct_count; }
};
int DestructCounter::destruct_count = 0;


TEST_F(DynamicArrayUnitTest, DestructorDestroysElements) {
    DestructCounter::reset();
    {
        constexpr int default_capacity = 5;
        DynamicArray<DestructCounter> arr;
        for (int i = 0; i < default_capacity; ++i) {
            DestructCounter counter;
            arr.addLast(counter);
        }
        EXPECT_EQ(DestructCounter::destruct_count, 5);
    }
    EXPECT_EQ(DestructCounter::destruct_count, 10);
}


TEST_F(DynamicArrayUnitTest, StringType) {
    DynamicArray<std::string> arr;
    arr.addLast("Hello");
    arr.addLast("World");
    EXPECT_EQ(arr.size(), 2u);
    EXPECT_EQ(arr.get(0), "Hello");
    EXPECT_EQ(arr.get(1), "World");
}


TEST_F(DynamicArrayUnitTest, CustomObjectType) {
    struct Point {
        int x, y;
        explicit Point(const int xx = 0, const int yy = 0) : x(xx), y(yy) {}
        bool operator==(const Point& o) const { return x == o.x && y == o.y; }
    };
    DynamicArray<Point> arr;
    arr.addLast(Point(1, 2));
    arr.addLast(Point(3, 4));
    EXPECT_EQ(arr.size(), 2u);
    EXPECT_EQ(arr.get(0), Point(1, 2));
    EXPECT_EQ(arr.get(1), Point(3, 4));
}


TEST_F(DynamicArrayUnitTest, ReferenceTypeTracksExternalChanges) {
    int a = 1, b = 2;
    DynamicArray<Record> arr;
    arr.addLast(Record{a});
    arr.addLast(Record{b});
    EXPECT_EQ(arr.size(), 2u);
    EXPECT_EQ(arr.get(0).value, 1);
    EXPECT_EQ(arr.get(1).value, 2);
    a = 10;
    b = 20;
    EXPECT_EQ(arr.get(0).value, 10);
    EXPECT_EQ(arr.get(1).value, 20);
}


TEST_F(DynamicArrayUnitTest, LargeNumberOfInsertions) {
    DynamicArray<int> arr;
    constexpr int num_elements = 1000;
    for (int i = 0; i < num_elements; ++i)
        arr.addLast(i);
    EXPECT_EQ(arr.size(), static_cast<std::size_t>(num_elements));
    for (int i = 0; i < num_elements; ++i)
        EXPECT_EQ(arr.get(i), i);
}


TEST_F(DynamicArrayUnitTest, AlternatingInsertionAndRemoval) {
    DynamicArray<int> arr;
    for (int i = 0; i < 100; ++i) {
        arr.addLast(i * 2);
        arr.addLast(i * 2 + 1);
        if (arr.size() > 1)
            arr.removeFirst();
    }
    EXPECT_GT(arr.size(), 0u);
    for (std::size_t i = 0; i + 1 < arr.size(); ++i)
        EXPECT_LT(arr.get(i), arr.get(i + 1));
}


TEST_F(DynamicArrayUnitTest, EmplaceLastMoveOnly) {
    DynamicArray<std::unique_ptr<int>> arr;
    arr.emplaceLast(std::make_unique<int>(7));
    ASSERT_EQ(arr.size(), 1u);
    EXPECT_EQ(*arr.get(0), 7);
}


TEST_F(DynamicArrayUnitTest, MovedFromAllowsSafeOps) {
    DynamicArray<int> a;
    a.addLast(1);
    DynamicArray<int> b(std::move(a));
    EXPECT_THROW(a.get(0), std::out_of_range);
    EXPECT_NO_THROW(a.addLast(42));
    EXPECT_EQ(a.get(0), 42);
}


TEST_F(DynamicArrayUnitTest, ShrinkToFitOnEmptyKeepsDefault) {
    DynamicArray<int> a;
    a.shrinkToFit();
    EXPECT_EQ(a.capacity(), 5u);
}


TEST_F(DynamicArrayUnitTest, ReserveZeroIsNoopForCapacity) {
    DynamicArray<int> a;
    auto c = a.capacity();
    a.reserve(0);
    EXPECT_EQ(a.capacity(), c);
}


TEST_F(DynamicArrayUnitTest, RemoveAllWithMoveOnly) {
    DynamicArray<std::unique_ptr<int>> a;
    for (int i = 0; i < 8; ++i)
        a.emplaceLast(std::make_unique<int>(i));
    a.removeAll();
    EXPECT_EQ(a.size(), 0u);
    EXPECT_GE(a.capacity(), 5u);
}
