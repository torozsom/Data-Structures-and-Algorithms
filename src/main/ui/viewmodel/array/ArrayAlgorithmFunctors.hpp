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


struct LinearInsertionSortFn {
    template <typename Array, typename Callback>
    void operator()(Array& array, Callback&& callback) {
        array_algorithms::LinearInsertionSort(array, callback);
    }
};


struct BinaryInsertionSortFn {
    template <typename Array, typename Callback>
    void operator()(Array& array, Callback&& callback) {
        array_algorithms::BinaryInsertionSort(array, callback);
    }
};


struct QuickSortFn {
    template <typename Array, typename Callback>
    void operator()(Array& array, Callback&& callback) {
        array_algorithms::QuickSort(array, callback);
    }
};


struct MergeSortInPlaceFn {
    template <typename Array, typename Callback>
    void operator()(Array& array, Callback&& callback) {
        array_algorithms::MergeSortInPlace(array, callback);
    }
};


struct HeapSortFn {
    template <typename Array, typename Callback>
    void operator()(Array& array, Callback&& callback) {
        array_algorithms::HeapSort(array, callback);
    }
};


#endif //ARRAY_ALGORITHM_FUNCTORS_HPP
