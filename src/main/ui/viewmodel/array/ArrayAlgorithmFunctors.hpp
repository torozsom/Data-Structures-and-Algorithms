#ifndef ARRAY_ALGORITHM_FUNCTORS_HPP
#define ARRAY_ALGORITHM_FUNCTORS_HPP


#include "ArrayAlgorithms.hpp"


struct LinearSearchFn {
    template <typename Array, typename Target, typename Callback>
    size_t operator()(const Array& array, const Target& target, Callback&& callback) {
        return array_algorithms::LinearSearch(array, target, callback);
    }
};


struct BinarySearchFn {
    template <typename Array, typename Target, typename Callback>
    size_t operator()(const Array& array, const Target& target, Callback&& callback) {
        return array_algorithms::BinarySearch(array, target, callback);
    }
};


struct BubbleSortFn {
    template <typename Array, typename Callback>
    void operator()(Array& array, Callback&& callback) {
        array_algorithms::BubbleSort(array, callback);
    }
};


struct ImprovedBubbleSortFn {
    template <typename Array, typename Callback>
    void operator()(Array& array, Callback&& callback) {
        array_algorithms::ImprovedBubbleSort(array, callback);
    }
};


struct InsertionSortLSFn {
    template <typename Array, typename Callback>
    void operator()(Array& array, Callback&& callback) {
        array_algorithms::InsertionSortWithLinearSearch(array, callback);
    }
};


struct InsertionSortBSFn {
    template <typename Array, typename Callback>
    void operator()(Array& array, Callback&& callback) {
        array_algorithms::InsertionSortWithBinarySearch(array, callback);
    }
};


struct QuickSortFn {
    template <typename Array, typename Callback>
    void operator()(Array& array, Callback&& callback) {
        array_algorithms::QuickSort(array, callback);
    }
};


struct MergeSortFn {
    template <typename Array, typename Callback>
    void operator()(Array& array, Callback&& callback) {
        array_algorithms::MergeSort(array, callback);
    }
};


struct HeapSortFn {
    template <typename Array, typename Callback>
    void operator()(Array& array, Callback&& callback) {
        array_algorithms::HeapSort(array, callback);
    }
};


#endif //ARRAY_ALGORITHM_FUNCTORS_HPP
