/**
 * @file DynamicArrayAlgorithms.hpp
 *
 * This file contains some sorting and searching algorithms
 * that can be executed over the data_structs::DynamicArray class
 * and also some utility functions for these algorithms.
 *
 * Note that these algorithms can EXCLUSIVELY be used on such
 * DynamicArrays that store built-in numeric types, so to be precise:
 * int, double, float, unsigned int and size_t.
 *
 * The purpose of these functions to ensure a basis for the
 * visualization of these data structures and its algorithms,
 * so there is no need to take care of user types.
 */


#ifndef DYNAMIC_ARRAY_ALGORITHMS_HPP
#define DYNAMIC_ARRAY_ALGORITHMS_HPP


#include <limits>
#include <stdexcept>
#include <utility>

#include "DynamicArray.hpp"


namespace array_algorithms {

using data_structs::DynamicArray;


//*** Utility Functions ***//

/// Swap two elements
template <typename Type>
void swap(Type& a, Type& b) noexcept {
    Type tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}


/**
 * @brief Merge two sorted sub-arrays into a single sorted array (numeric
 * types).
 *
 * Merges data_[left..mid] and data_[mid+1..right] using a single temporary
 * buffer. Original data is not modified until the temporary is fully built,
 * then we copy back in one pass.
 *
 * Strong exception guarantee under numeric-type assumptions (no-throw ops).
 *
 * @param array The array to sort.
 * @param left The left index of the first sub-array.
 * @param mid The middle index, end of the first sub-array.
 * @param right The right index of the second sub-array.
 */
template <typename Type>
void merge(DynamicArray<Type>& array, const std::size_t left,
           const std::size_t mid, const std::size_t right) {
    const std::size_t n1 = mid - left + 1;
    const std::size_t n2 = right - mid;
    const std::size_t n = n1 + n2;

    std::size_t i = left;
    std::size_t j = mid + 1;

    DynamicArray<Type> temp(n);

    // Merge into temp
    while (i <= mid && j <= right) {
        if (array[i] <= array[j])
            temp.addLast(array[i++]);
        else
            temp.addLast(array[j++]);
    }

    while (i <= mid)
        temp.addLast(array[i++]);
    while (j <= right)
        temp.addLast(array[j++]);

    // Copy back
    for (std::size_t t = 0; t < n; ++t)
        array[left + t] = temp[t];
}


/**
 * @brief Recursive merge sort implementation.
 *
 * Recursively splits the array into halves until single-element arrays are
 * reached, then merges them back together in sorted order.
 *
 * @param array The array to sort
 * @param left Left index of the sub-array to sort.
 * @param right Right index of the sub-array to sort.
 */
template <typename Type>
void mergeSortRecursive(DynamicArray<Type>& array, const std::size_t left,
                        const std::size_t right) {
    if (left >= right)
        return;

    const std::size_t mid = left + (right - left) / 2;

    // Recursively sort both halves
    mergeSortRecursive(array, left, mid);
    mergeSortRecursive(array, mid + 1, right);

    if (array[mid] < array[mid + 1])
        return;

    // Merge the sorted halves
    merge(array, left, mid, right);
}


/**
 * @brief Partition the array around a pivot for quicksort.
 *
 * Uses the Lomuto partition scheme, selecting the last element as the pivot.
 * Elements less than or equal to the pivot are moved to its left, and those
 * greater are moved to its right.
 *
 * @param array The array to partition.
 * @param left Left index of the sub-array to partition.
 * @param right Right index of the sub-array to partition.
 * @return The final index of the pivot after partitioning.
 */
template <typename Type>
std::size_t partition(DynamicArray<Type>& array, const std::size_t left,
                      const std::size_t right) {
    Type pivot = array[right]; // Lomuto: pivot is last element
    std::size_t i = left;

    for (std::size_t j = left; j < right; ++j)
        if (array[j] <= pivot)
            swap(array[i++], array[j]);

    swap(array[i], array[right]);
    return i;
}


/**
 * @brief Recursive quicksort implementation with tail recursion elimination.
 *
 * Partitions the array around a pivot, then recursively sorts the partitions.
 * Tail recursion is eliminated by always recursing into the smaller partition
 * first and iterating on the larger one.
 *
 * @param array The array to sort.
 * @param left Left index of the sub-array to sort.
 * @param right Right index of the sub-array to sort.
 */
template <typename Type>
void quickSortRecursive(DynamicArray<Type>& array, std::size_t left,
                        std::size_t right) {
    while (left < right) {
        // Tail recursion elimination: recurse into smaller side first
        if (const std::size_t p = partition(array, left, right);
            p > 0 && (p - left) < (right - p)) {
            quickSortRecursive(array, left, p - 1);
            left = p + 1;
        } else {
            if (p + 1 < right)
                quickSortRecursive(array, p + 1, right);
            if (p == 0)
                break; // prevent size_t underflow
            right = p - 1;
        }
    }
}


/**
 * @brief Sift a node down in a max-heap (array-based, 0-indexed).
 *
 * Restores heap property in the range [0, size). No effect if start is a leaf.
 *
 * @param array The array representing the heap.
 * @param start The index to sift down from (typically 0).
 * @param size The heap size (considered range is [0, size)).
 */
template <typename Type>
void siftDown(DynamicArray<Type>& array, const std::size_t start,
              const std::size_t size) {
    std::size_t i = start;
    while (true) {
        const std::size_t left = 2 * i + 1;
        if (left >= size)
            break;

        std::size_t largest = left;
        const std::size_t right = left + 1;

        if (right < size && array[right] > array[left])
            largest = right;

        if (array[i] >= array[largest])
            break;

        swap(array[i], array[largest]);
        i = largest;
    }
}


/**
 * @brief Sift a node up in a max-heap (array-based, 0-indexed).
 *
 * Restores heap property assuming the new element is at index idx.
 *
 * @param array The array representing the heap.
 * @param idx The index to sift up from (typically size-1 after an insertion).
 */
template <typename Type>
void siftUp(DynamicArray<Type>& array, std::size_t idx) {
    while (idx > 0) {
        const std::size_t parent = (idx - 1) / 2;
        if (array[parent] >= array[idx])
            break;
        swap(array[parent], array[idx]);
        idx = parent;
    }
}


/**
 * @brief Transform the array into a max-heap in-place.
 *
 * Uses bottom-up heap construction in O(n) time.
 *
 * @param array The array to heapify (range is [0, a.size())).
 */
template <typename Type>
void makeHeap(DynamicArray<Type>& array) {
    const std::size_t n = array.size();
    if (n <= 1)
        return;

    for (std::size_t i = (n - 2) / 2 + 1; i-- > 0;)
        siftDown(array, i, n);
}


/**
 * @brief Insert the last element into the heap (restore heap property).
 *
 * Assumes the logical heap occupies [0, size), with a new element placed at
 * index size-1. After the call, [0, size) is a valid max-heap.
 *
 * @param array The array containing the heap.
 * @param size The new heap size (must be in [1, a.size()]).
 */
template <typename Type>
void pushHeap(DynamicArray<Type>& array, const std::size_t size) {
    if (size == 0 || size > array.size())
        return;
    siftUp(array, size - 1);
}


/**
 * @brief Move the max element to the end (size-1) and restore heap on prefix.
 *
 * Swaps array[0] with array[size-1] and then fixes the heap on [0, size-1).
 * After the call, the range [size-1, size) contains the extracted maximum.
 *
 * @param array The array containing the heap.
 * @param size The current heap size (must be in [1, array.size()]).
 */
template <typename Type>
void popHeap(DynamicArray<Type>& array, const std::size_t size) {
    if (size <= 1 || size > array.size())
        return;
    swap(array[0], array[size - 1]);
    siftDown(array, 0, size - 1);
}


/// Checks if the array is sorted in ascending order.
template <typename Type>
bool isSorted(const DynamicArray<Type>& array) noexcept {
    for (std::size_t i = 1; i < array.size(); ++i)
        if (array[i] < array[i - 1])
            return false;

    return true;
}


//*** Sorting Algorithms ***//


/**
 * @brief Sorts the array in ascending (non-decreasing) order using the Bubble
 * Sort algorithm.
 *
 * This method repeatedly steps through the list, compares adjacent elements,
 * and swaps them if they are in the wrong order. The process is repeated until
 * the list is sorted. Stable.
 *
 * Note for float/double: Arrays containing NaN are unsupported for ordering;
 * results are unspecified.
 *
 * @par Complexity
 * - O(n^2) time complexity in the average and worst cases.
 * - O(n) time complexity in the best case (when the array is already sorted).
 * - O(1) additional space complexity.
 *
 * @param array The array to sort.
 */
template <typename Type>
void BubbleSort(DynamicArray<Type>& array) {
    if (array.size() <= 1 || isSorted(array))
        return;

    for (std::size_t i = 0; i < array.size() - 1; ++i)
        for (std::size_t j = 0; j < array.size() - i - 1; ++j)
            if (array[j] > array[j + 1])
                swap(array[j], array[j + 1]);
}


/** @brief Sorts the array in ascending (non-decreasing) order using an improved
 * version of Bubble Sort.
 *
 * This method includes optimizations such as early-exit if no swaps are made
 * during a pass, and reducing the range of subsequent passes based on the last
 * swap position. Stable.
 *
 * Note for float/double: Arrays containing NaN are unsupported for ordering;
 * results are unspecified.
 *
 * @par Complexity
 * - O(n^2) time complexity in the average and worst cases.
 * - O(n) time complexity in the best case (when the array is already sorted).
 * - O(1) additional space complexity.
 *
 * @param array The array to sort.
 */
template <typename Type>
void ImprovedBubbleSort(DynamicArray<Type>& array) {
    if (array.size() <= 1 || isSorted(array))
        return;

    // Early-exit + shrinking boundary optimization
    std::size_t n = array.size();
    while (n > 1) {
        bool swapped = false;
        std::size_t last_swap = 0;

        for (std::size_t j = 0; j + 1 < n; ++j) {
            if (array[j] > array[j + 1]) {
                swap(array[j], array[j + 1]);
                swapped = true;
                last_swap = j + 1;
            }
        }

        if (!swapped)
            break;
        n = last_swap;
    }
}


/**
 * @brief Sorts the array in ascending (non-decreasing) order using the
 * Insertion Sort algorithm with linear search.
 *
 * This method builds a sorted portion of the array by repeatedly taking the
 * next element and inserting it into its correct position within the sorted
 * portion using linear search. Stable.
 *
 * Note for float/double: Arrays containing NaN are unsupported for ordering;
 * results are unspecified.
 *
 * @par Complexity
 * - O(n^2) time complexity in the average and worst cases.
 * - O(n) time complexity in the best case (when the array is already sorted).
 * - O(1) additional space complexity.
 *
 * @param array The array to sort.
 */
template <typename Type>
void InsertionSortWithLinearSearch(DynamicArray<Type>& array) {
    if (array.size() <= 1 || isSorted(array))
        return;

    for (std::size_t i = 1; i < array.size(); ++i) {
        Type key = array[i];
        std::size_t j = i;
        while (j > 0 && array[j - 1] > key) {
            array[j] = array[j - 1];
            --j;
        }
        array[j] = key;
    }
}


/**
 * @brief Sorts the array in ascending (non-decreasing) order using the
 * Insertion Sort algorithm with binary search.
 *
 * This method builds a sorted portion of the array by repeatedly taking the
 * next element and inserting it into its correct position within the sorted
 * portion using binary search to find the insertion point. Stable (uses
 * upper_bound-style insertion).
 *
 * Note for float/double: Arrays containing NaN are unsupported for ordering;
 * results are unspecified.
 *
 * @par Complexity
 * - O(n^2) time complexity in the average and worst cases due to shifting
 * elements.
 * - O(n log n) time complexity for finding the insertion point using binary
 * search.
 * - O(1) additional space complexity.
 *
 * @param array The array to sort.
 */
template <typename Type>
void InsertionSortWithBinarySearch(DynamicArray<Type>& array) {
    if (array.size() <= 1 || isSorted(array))
        return;

    for (std::size_t i = 1; i < array.size(); ++i) {
        Type key = array[i];

        // Binary search for insertion position in [0, i)
        std::size_t left = 0, right = i;
        while (left < right) {
            std::size_t mid = left + (right - left) / 2;
            if (array[mid] <= key)
                left = mid + 1;
            else
                right = mid;
        }

        // Shift to make room
        for (std::size_t j = i; j > left; --j)
            array[j] = array[j - 1];

        array[left] = key;
    }
}


/**
 * @brief Sorts the array in ascending (non-decreasing) order using the Quick
 * Sort algorithm.
 *
 * Partitions the array around a pivot and recursively sorts the partitions.
 * Implementation partitions with "<= pivot" on the left side (Lomuto), and is
 * not stable.
 *
 * Note for float/double: Arrays containing NaN are unsupported for ordering;
 * results are unspecified.
 *
 * @par Complexity
 * - O(n log n) average time complexity.
 * - O(n^2) worst-case time complexity (e.g., already-sorted or duplicate-heavy
 * arrays with last-element pivot).
 * - O(log n) space complexity due to recursion stack (tail recursion eliminated
 * on larger side).
 *
 * @param array The array to sort.
 */
template <typename Type>
void QuickSort(DynamicArray<Type>& array) {
    if (array.size() <= 1 || isSorted(array))
        return;

    quickSortRecursive(array, 0, array.size() - 1);
}


/**
 * @brief Sorts the array in ascending (non-decreasing) order using the Merge
 * Sort algorithm.
 *
 * This method recursively divides the array into halves, sorts each half,
 * and then merges the sorted halves back together. It uses a temporary
 * buffer for merging. Stable.
 *
 * Note for float/double: Arrays containing NaN are unsupported for ordering;
 * results are unspecified.
 *
 * @par Complexity
 * - O(n log n) time complexity in all cases (best, average, worst).
 * - O(n) additional space complexity for the temporary buffer.
 *
 * @param array The array to sort.
 */
template <typename Type>
void MergeSort(DynamicArray<Type>& array) {
    if (array.size() <= 1 || isSorted(array))
        return;

    mergeSortRecursive(array, 0, array.size() - 1);
}


/**
 * @brief Bin Sort for a known 0-based universe.
 *
 * Universe: U = {0, 1, ..., universe_size-1}. Each element of the array must be
 * in U.
 *
 * Phase 1: For each element x in A, append x to the tail of bin B[x].
 * Phase 2: Traverse bins B[0]..B[m-1] and write their contents back to A in
 * order.
 *
 * Stable, O(n + m) time, O(n + m) extra space, where n = size(), m =
 * universe_size.
 *
 * @param array The array to sort.
 * @param universe_size The size of the universe (m). Must be > 0.
 *
 * @throws std::out_of_range if a value is outside [0, universe_size).
 */
template <typename Type>
void BinSort(DynamicArray<Type>& array, const std::size_t universe_size) {
    static_assert(std::numeric_limits<Type>::is_integer,
                  "BinSort(universe) requires an integral Type.");

    if (array.size() <= 1 || universe_size == 0)
        return;

    struct Node {
        Type value;
        Node* next;
    };

    DynamicArray<Node*> heads(universe_size);
    DynamicArray<Node*> tails(universe_size);

    for (std::size_t b = 0; b < universe_size; ++b) {
        heads.addLast(nullptr);
        tails.addLast(nullptr);
    }

    // Phase 1: distribute elements into bins
    try {
        for (std::size_t i = 0; i < array.size(); ++i) {
            const Type v = array[i];
            const auto bin = static_cast<std::size_t>(v);
            if (bin >= universe_size)
                throw std::out_of_range(
                    "BinSort: value out of [0, m) universe");

            Node* node = new Node{v, nullptr};
            if (!heads[bin]) {
                heads[bin] = tails[bin] = node;
            } else {
                tails[bin]->next = node;
                tails[bin] = node;
            }
        }
    } catch (...) {
        // Cleanup lists before rethrow
        for (std::size_t b = 0; b < universe_size; ++b) {
            Node* cur = heads[b];
            while (cur) {
                Node* nxt = cur->next;
                delete cur;
                cur = nxt;
            }
        }
        throw;
    }

    // Phase 2: collect back to A in increasing bin index
    std::size_t write = 0;
    for (std::size_t b = 0; b < universe_size; ++b) {
        Node* cur = heads[b];
        while (cur) {
            array[write++] = cur->value;
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
    }
}


/**
 * @brief Bin Sort for a known contiguous universe range.
 *
 * Universe: U = {min_value, min_value+1, ..., max_value}. Each element must lie
 * in this range. Internally maps key x to bin index (x - min_value), keeps
 * stability by appending to each bin, then writes bins back in ascending order.
 *
 * Stable, O(n + m) time, O(n + m) extra space, where n = size(),
 * m = static_cast<std::size_t>(max_value - min_value + 1).
 *
 * @param array The array to sort.
 * @param min_value Minimum value in the universe (inclusive).
 * @param max_value Maximum value in the universe (inclusive).
 *
 * @throws std::out_of_range if an element of the array is outside [min_value,
 * max_value].
 */
template <typename Type>
void BinSort(DynamicArray<Type>& array, const Type min_value,
             const Type max_value) {
    static_assert(std::numeric_limits<Type>::is_integer,
                  "BinSort(range) requires an integral Type.");

    if (array.size() <= 1 || max_value < min_value)
        return;

    // Define unsigned counterpart to avoid narrowing/UB
    using U = std::make_unsigned_t<Type>;

    // Map bounds to unsigned once
    const U umin = static_cast<U>(min_value);
    const U umax = static_cast<U>(max_value);
    const U span = static_cast<U>(umax - umin);

    // Ensure m = span + 1 fits into size_t to avoid overflow on allocation
    if (span >= static_cast<U>(std::numeric_limits<std::size_t>::max()))
        throw std::length_error("BinSort: universe too large to index");

    const std::size_t m = static_cast<std::size_t>(span) + 1;

    struct Node {
        Type value;
        Node* next;
    };

    DynamicArray<Node*> heads(m);
    DynamicArray<Node*> tails(m);

    for (std::size_t b = 0; b < m; ++b) {
        heads.addLast(nullptr);
        tails.addLast(nullptr);
    }

    // Phase 1: distribute elements into bins
    try {
        for (std::size_t i = 0; i < array.size(); ++i) {
            const Type v = array[i];
            if (v < min_value || v > max_value)
                throw std::out_of_range(
                    "BinSort: value out of [min,max] universe");

            // Compute bin index in unsigned domain without narrowing
            const auto bin = static_cast<std::size_t>(static_cast<U>(v) - umin);

            Node* node = new Node{v, nullptr};
            if (!heads[bin]) {
                heads[bin] = tails[bin] = node;
            } else {
                tails[bin]->next = node;
                tails[bin] = node;
            }
        }
    } catch (...) {
        for (std::size_t b = 0; b < m; ++b) {
            Node* cur = heads[b];
            while (cur) {
                Node* nxt = cur->next;
                delete cur;
                cur = nxt;
            }
        }
        throw;
    }

    // Phase 2: collect back to A in increasing bin index
    std::size_t write = 0;
    for (std::size_t b = 0; b < m; ++b) {
        Node* cur = heads[b];
        while (cur) {
            array[write++] = cur->value;
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
    }
}


/**
 * @brief Sorts the array in ascending order using LSD Radix Sort (base 256).
 *
 * Stable O(k·n) for integral types, where k = sizeof(Type) bytes.
 * Handles signed types by flipping the sign bit on the most significant byte
 * pass so that negatives come before non-negatives.
 *
 * @par Constraints
 * - Type must be an integral type.
 * - Uses O(n) temporary buffer.
 *
 * @param array The array to sort.
 */
template <typename Type>
void RadixSortLSD(DynamicArray<Type>& array) {
    static_assert(std::numeric_limits<Type>::is_integer,
                  "RadixSortLSD requires an integral Type.");

    const std::size_t n = array.size();
    if (n <= 1 || isSorted(array))
        return;

    using U = std::make_unsigned_t<Type>;
    constexpr std::size_t RADIX = 256;
    constexpr std::size_t BYTE_MASK = 0xFFu;
    const std::size_t bytes = sizeof(Type);

    // Temporary buffer for stable distribution
    DynamicArray<Type> temp(n);
    for (std::size_t i = 0; i < n; ++i)
        temp.addLast(Type());

    for (std::size_t pass = 0; pass < bytes; ++pass) {
        std::size_t count[RADIX] = {};
        const bool msb_pass = (pass + 1 == bytes);
        const bool use_array_as_source = (pass % 2 == 0);

        // Counting occurrences from the correct source
        if (use_array_as_source) {
            for (std::size_t i = 0; i < n; ++i) {
                U u = static_cast<U>(array[i]);
                auto digit =
                    static_cast<std::size_t>((u >> (8 * pass)) & BYTE_MASK);
                if constexpr (std::numeric_limits<Type>::is_signed)
                    if (msb_pass)
                        digit ^= 0x80u;
                ++count[digit];
            }
        } else {
            for (std::size_t i = 0; i < n; ++i) {
                U u = static_cast<U>(temp[i]);
                auto digit =
                    static_cast<std::size_t>((u >> (8 * pass)) & BYTE_MASK);
                if constexpr (std::numeric_limits<Type>::is_signed)
                    if (msb_pass)
                        digit ^= 0x80u;
                ++count[digit];
            }
        }

        // Prefix sums to get starting positions
        std::size_t pos[RADIX];
        pos[0] = 0;
        for (std::size_t d = 1; d < RADIX; ++d)
            pos[d] = pos[d - 1] + count[d - 1];

        // Stable distribution: even passes write to temp, odd passes back to
        // array
        if (use_array_as_source) {
            for (std::size_t i = 0; i < n; ++i) {
                U u = static_cast<U>(array[i]);
                auto digit =
                    static_cast<std::size_t>((u >> (8 * pass)) & BYTE_MASK);
                if constexpr (std::numeric_limits<Type>::is_signed)
                    if (msb_pass)
                        digit ^= 0x80u;
                temp[pos[digit]++] = array[i];
            }
        } else {
            for (std::size_t i = 0; i < n; ++i) {
                U u = static_cast<U>(temp[i]);
                auto digit =
                    static_cast<std::size_t>((u >> (8 * pass)) & BYTE_MASK);
                if constexpr (std::numeric_limits<Type>::is_signed)
                    if (msb_pass)
                        digit ^= 0x80u;
                array[pos[digit]++] = temp[i];
            }
        }
    }

    // If number of passes is odd, last write was into temp -> copy back
    if ((bytes % 2) == 1)
        for (std::size_t i = 0; i < n; ++i)
            array[i] = temp[i];
}


/**
 * @brief Sorts the array in ascending order using MSD Radix Sort (base 256).
 *
 * Stable O(k·n) for integral types, where k = sizeof(Type) bytes.
 * Starts from the most significant byte and recursively sorts buckets by the
 * next less significant byte. For signed types, the MSB pass flips the sign bit
 * so that negatives precede non-negatives.
 *
 * @par Constraints
 * - Type must be an integral type.
 * - Uses O(n) temporary buffer reused across recursion.
 *
 * @param array The array to sort.
 */
template <typename Type>
void RadixSortMSD(DynamicArray<Type>& array) {
    static_assert(std::numeric_limits<Type>::is_integer,
                  "RadixSortMSD requires an integral Type.");

    const std::size_t n = array.size();
    if (n <= 1 || isSorted(array))
        return;

    using U = std::make_unsigned_t<Type>;
    constexpr std::size_t RADIX = 256;

    // Helper to get the byte at position 'byte' (0-based from LSB)
    auto get_digit = [](U u, const int byte) -> std::size_t {
        return static_cast<std::size_t>((u >> (8 * byte)) & 0xFFu);
    };

    // Reusable temporary buffer for stable distribution
    DynamicArray<Type> temp(n);
    for (std::size_t i = 0; i < n; ++i)
        temp.addLast(Type());

    // Self-recursive lambda via fixpoint pattern: pass 'self' explicitly
    auto msd_rec = [&](auto&& self, const std::size_t lo, const std::size_t hi,
                       const int byte, const bool flip_msb) -> void {
        if (const std::size_t len = hi - lo; len <= 1 || byte < 0)
            return;

        std::size_t count[RADIX] = {};

        // Count digits
        for (std::size_t i = lo; i < hi; ++i) {
            U u = static_cast<U>(array[i]);
            std::size_t d = get_digit(u, byte);

            if (flip_msb)
                if constexpr (std::numeric_limits<Type>::is_signed)
                    d ^= 0x80u; // map sign bit so negatives come first

            ++count[d];
        }

        // Compute starting positions in [lo, hi)
        std::size_t pos[RADIX];
        pos[0] = lo;
        for (std::size_t d = 1; d < RADIX; ++d)
            pos[d] = pos[d - 1] + count[d - 1];

        // Stable distribute into temp
        for (std::size_t i = lo; i < hi; ++i) {
            U u = static_cast<U>(array[i]);
            std::size_t d = get_digit(u, byte);

            if (flip_msb)
                if constexpr (std::numeric_limits<Type>::is_signed)
                    d ^= 0x80u;

            temp[pos[d]++] = array[i];
        }

        // Copy back to array
        for (std::size_t i = lo; i < hi; ++i)
            array[i] = temp[i];

        // Recurse on each non-empty bucket with next byte (no sign flip below
        // MSB)
        std::size_t start = lo;
        for (const std::size_t cnt : count) {
            if (cnt > 1)
                self(self, start, start + cnt, byte - 1, false);
            start += cnt;
        }
    };

    // Start from the MSB (byte index sizeof(Type)-1), flip sign bit at MSB
    msd_rec(msd_rec, 0, n, static_cast<int>(sizeof(Type)) - 1, true);
}


/**
 * @brief Sorts the array in ascending order using HeapSort (max-heap).
 *
 * In-place, not stable. Builds a max-heap, then repeatedly moves the maximum
 * to the end and shrinks the heap.
 *
 * Note for float/double: Arrays containing NaN are unsupported for ordering;
 * results are unspecified.
 *
 * @par Complexity
 * - O(n log n) time.
 * - O(1) extra space (ignoring recursion stack as there is none).
 *
 * @param array The array to sort.
 */
template <typename Type>
void HeapSort(DynamicArray<Type>& array) {
    const std::size_t n = array.size();
    if (n <= 1 || isSorted(array))
        return;

    makeHeap(array);
    for (std::size_t heap_size = n; heap_size > 1; --heap_size)
        popHeap(array, heap_size);
}


/*** Searching Algorithms ***/


/**
 * @brief Performs a linear search in the array for the given element.
 *
 * Scans the array from start to end, comparing each element to the target.
 * Returns the index of the first occurrence of the element, or array.size()
 * if not found.
 *
 * @param array The array to search in.
 * @param element The element to search for.
 * @param callback The callback function to invoke on each index visited.
 * @return The index of the found element, or array.size() if not found.
 *
 * @par Complexity
 * - O(n) time.
 * - O(1) space.
 */
template <typename Type, typename Callback = void (*)(std::size_t)>
std::size_t LinearSearch(const DynamicArray<Type>& array, const Type& element,
                         Callback&& callback = +[](std::size_t) {}) {
    for (std::size_t i = 0; i < array.size(); ++i) {
        callback(i);
        if (array[i] == element)
            return i;
    }
    return array.size();
}


/**
 * @brief Performs a binary search for the given element.
 *
 * Requires the array to be sorted in non-decreasing (ascending) order.
 * Searches for an element equal to the given key and returns its index if
 * present; otherwise returns array.size().
 *
 * Note for float/double: Arrays containing NaN are unsupported for ordering;
 * results are unspecified.
 *
 * @param array The array to search in.
 * @param element The element to search for.
 * @param callback The callback function to invoke on each index visited.
 * @return The index of the found element, or array.size() if not found.
 *
 * @par Complexity
 * - O(log n) time.
 * - O(1) space.
 */
template <typename Type, typename Callback>
std::size_t BinarySearch(const DynamicArray<Type>& array, const Type& element,
                         Callback&& callback = +[](std::size_t) -> void {}) {
    // left: inclusive lower bound, right: exclusive upper bound
    std::size_t left = 0;
    std::size_t right = array.size() - 1;

    while (left < right) {
        std::size_t middle = left + (right - left) / 2;
        callback(middle);
        if (array[middle] == element)
            return middle;
        if (array[middle] < element)
            left = middle + 1;
        else
            right = middle - 1;
    }

    if (left < array.size())
        callback(left);

    // left is the first index not less than element
    if (left < array.size() && array[left] == element)
        return left;

    return array.size();
}

} // namespace array_algorithms

#endif // DYNAMIC_ARRAY_ALGORITHMS_HPP
