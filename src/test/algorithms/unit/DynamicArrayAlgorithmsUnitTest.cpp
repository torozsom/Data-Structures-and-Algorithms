#include "DynamicArray.hpp"
#include "DynamicArrayAlgorithms.hpp"

#include <gtest/gtest.h>


using data_structs::DynamicArray;
using namespace array_algorithms;


class DynamicArrayAlgorithmsUnitTest : public testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override { }
};


TEST_F(DynamicArrayAlgorithmsUnitTest, BubbleSortCorrectlySorts) {
    DynamicArray arr{6, 4, 9, 3, 3, 6, 2, 1, 7};
    BubbleSort(arr);
    for (std::size_t i = 1; i < arr.size(); i++)
        EXPECT_LE(arr[i - 1], arr[i]);
}


TEST_F(DynamicArrayAlgorithmsUnitTest, ImprovedBubbleSortCorrectlySorts) {
    DynamicArray arr{6, 4, 9, 3, 3, 6, 2, 1, 7};
    ImprovedBubbleSort(arr);
    for (std::size_t i = 1; i < arr.size(); i++)
        EXPECT_LE(arr[i - 1], arr[i]);
}


TEST_F(DynamicArrayAlgorithmsUnitTest, InsertionSortWithLinearSearchCorrectlySorts) {
    DynamicArray arr{6, 4, 9, 3, 3, 6, 2, 1, 7};
    InsertionSortWithLinearSearch(arr);
    for (std::size_t i = 1; i < arr.size(); i++)
        EXPECT_LE(arr[i - 1], arr[i]);
}


TEST_F(DynamicArrayAlgorithmsUnitTest, InsertionSortWithBinarySearchCorrectlySorts) {
    DynamicArray arr{6, 4, 9, 3, 3, 6, 2, 1, 7};
    InsertionSortWithBinarySearch(arr);
    for (std::size_t i = 1; i < arr.size(); i++)
        EXPECT_LE(arr[i - 1], arr[i]);
}


TEST_F(DynamicArrayAlgorithmsUnitTest, QuickSortCorrectlySorts) {
    DynamicArray arr{6, 4, 9, 3, 3, 6, 2, 1, 7};
    QuickSort(arr);
    for (std::size_t i = 1; i < arr.size(); i++)
        EXPECT_LE(arr[i - 1], arr[i]);
}


TEST_F(DynamicArrayAlgorithmsUnitTest, MergeSortCorrectlySorts) {
    DynamicArray arr{6, 4, 9, 3, 3, 6, 2, 1, 7};
    MergeSort(arr);
    for (std::size_t i = 1; i < arr.size(); i++)
        EXPECT_LE(arr[i - 1], arr[i]);
}


TEST_F(DynamicArrayAlgorithmsUnitTest, BinSortUniverseSizeCorrectlySorts) {
    DynamicArray arr{6, 4, 9, 3, 3, 6, 2, 1, 7};
    BinSort(arr, 10);
    for (std::size_t i = 1; i < arr.size(); i++)
        EXPECT_LE(arr[i - 1], arr[i]);
}


TEST_F(DynamicArrayAlgorithmsUnitTest, BinSortIntervalCorrectlySorts) {
    DynamicArray arr{6, 4, 9, 3, 3, 6, 2, 1, 7};
    BinSort(arr, 1, 9);
    for (std::size_t i = 1; i < arr.size(); i++)
        EXPECT_LE(arr[i - 1], arr[i]);
}


TEST_F(DynamicArrayAlgorithmsUnitTest, RadixSortLSDCorrectlySorts) {
    DynamicArray arr{6, 4, 9, 3, 3, 6, 2, 1, 7};
    RadixSortLSD(arr);
    for (std::size_t i = 1; i < arr.size(); i++)
        EXPECT_LE(arr[i - 1], arr[i]);
}


TEST_F(DynamicArrayAlgorithmsUnitTest, RadixSortMSDCorrectlySorts) {
    DynamicArray arr{6, 4, 9, 3, 3, 6, 2, 1, 7};
    RadixSortMSD(arr);
    for (std::size_t i = 1; i < arr.size(); i++)
        EXPECT_LE(arr[i - 1], arr[i]);
}


TEST_F(DynamicArrayAlgorithmsUnitTest, HeapSortCorrectlySorts) {
    DynamicArray arr{6, 4, 9, 3, 3, 6, 2, 1, 7};
    HeapSort(arr);
    for (std::size_t i = 1; i < arr.size(); i++)
        EXPECT_LE(arr[i - 1], arr[i]);
}


TEST_F(DynamicArrayAlgorithmsUnitTest, LinearSearchFindsElement) {
    const DynamicArray arr{6, 4, 9, 3, 3, 6, 2, 1, 7};
    EXPECT_EQ(LinearSearch(arr, 3), 3u);
}


TEST_F(DynamicArrayAlgorithmsUnitTest, LinearSearchCannotFindElement) {
    const DynamicArray arr{6, 4, 9, 3, 3, 6, 2, 1, 7};
    EXPECT_EQ(LinearSearch(arr, 0), arr.size());
}