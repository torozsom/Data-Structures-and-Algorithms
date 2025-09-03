/**
 * @file ArrayAlgorithms.hpp
 *
 * This file contains sorting and searching algorithms that operate on the
 * containers::DynamicArray class, along with utility functions used by these
 * algorithms.
 *
 * Note that these algorithms can EXCLUSIVELY be used on DynamicArrays that
 * store built-in numeric types, specifically: int, double, float, unsigned int
 * and size_t.
 *
 * The purpose of these functions is to provide a basis for the visualization
 * of these data structures and their algorithms, so there is no need to support
 * user-defined types.
 */


#ifndef DYNAMIC_ARRAY_ALGORITHMS_HPP
#define DYNAMIC_ARRAY_ALGORITHMS_HPP


#include <limits>
#include <stdexcept>
#include <utility>

#include "DynamicArray.hpp"


namespace array_algorithms {

using containers::DynamicArray;
using std::size_t;


//*** Utility Functions ***//

/// Swap two elements
template <typename Type>
void swap(Type& a, Type& b) noexcept {
    Type tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}


/// Checks if the array is sorted in ascending order.
template <typename Type>
bool isSorted(const DynamicArray<Type>& array) noexcept {
    for (size_t i = 1; i < array.size(); ++i)
        if (array[i] < array[i - 1])
            return false;

    return true;
}


// -- Searching Algorithms -- //


/**
 * @brief Performs a linear search in the array for the given element.
 *
 * Scans the array from start to end, comparing each element to the target.
 * Returns the index of the first occurrence of the element, or array.size()
 * if not found.
 *
 * @param array The array to search in.
 * @param target The element to search for.
 * @param callback The callback function to invoke on each index visited.
 * @return The index of the found element, or array.size() if not found.
 *
 * @par Complexity
 * - O(n) time.
 * - O(1) space.
 */
template <typename Type, typename Callback = void (*)(size_t)>
size_t LinearSearch(const DynamicArray<Type>& array, const Type& target,
                    Callback&& callback = [](size_t) {}) {
    for (size_t i = 0; i < array.size(); ++i) {
        callback(i);
        if (array[i] == target)
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
 * @param target The element to search for.
 * @param callback The callback function to invoke on each index visited.
 * @return The index of the found element, or array.size() if not found.
 *
 * @par Complexity
 * - O(log n) time.
 * - O(1) space.
 */
template <typename Type, typename Callback>
size_t BinarySearch(const DynamicArray<Type>& array, const Type& target,
                    Callback&& callback = [](size_t) -> void {}) {
    // left: inclusive lower bound, right: exclusive upper bound
    size_t left = 0;
    size_t right = array.size() - 1;

    while (left < right) {
        size_t middle = left + (right - left) / 2;
        callback(middle);
        if (array[middle] == target)
            return middle;
        if (array[middle] < target)
            left = middle + 1;
        else
            right = middle;
    }

    if (left < array.size())
        callback(left);

    // left is the first index not less than element
    if (left < array.size() && array[left] == target)
        return left;

    return array.size();
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
 * @param callback Optional callback function to report each operation:
 * The callback receives events as (code, a, b):
 *  - code = 0: Compare(a, b)           — comparing indices a and b
 *  - code = 1: Swap(a, b)              — swapping indices a and b
 *  - code = 2: MarkSorted(a, ignored)  — index a is now in final sorted place
 */
template <typename Type, typename Callback = void (*)(size_t, size_t, size_t)>
void BubbleSort(DynamicArray<Type>& array,
                Callback&& callback = [](size_t, size_t, size_t) -> void {}) {
    const size_t n = array.size();
    if (n <= 1 || isSorted(array)) {
        for (size_t i = 0; i < n; ++i)
            callback(2, i, 0);
        return;
    }

    for (size_t i = 0; i < n - 1; ++i) {
        for (size_t j = 0; j < n - i - 1; ++j) {
            callback(0, j, j + 1); // compare
            if (array[j] > array[j + 1]) {
                callback(1, j, j + 1); // swap
                swap(array[j], array[j + 1]);
            }
        }
        // Mark the last element of this pass as sorted
        callback(2, n - i - 1, 0);
    }
    // First element is also in its correct place after the final pass
    callback(2, 0, 0);
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
 * @param callback Optional callback function to report each operation:
 * The callback receives events as (code, a, b):
 *  - code = 0: Compare(a, b)           — comparing indices a and b
 *  - code = 1: Swap(a, b)              — swapping indices a and b
 *  - code = 2: MarkSorted(a, ignored)  — index a is now in final sorted place
 */
template <typename Type, typename Callback = void (*)(size_t, size_t, size_t)>
void ImprovedBubbleSort(DynamicArray<Type>& array,
                Callback&& callback = [](size_t, size_t, size_t) -> void {}) {
    size_t n = array.size();
    if (n <= 1 || isSorted(array)) {
        for (size_t i = 0; i < n; ++i)
            callback(2, i, 0);
        return;
    }

    // Early-exit + shrinking boundary optimization
    while (n > 1) {
        bool swapped = false;
        size_t last_swap = 0;

        for (size_t j = 0; j + 1 < n; ++j) {
            callback(0, j, j + 1); // compare
            if (array[j] > array[j + 1]) {
                callback(1, j, j + 1); // swap
                swap(array[j], array[j + 1]);
                swapped = true;
                last_swap = j + 1;
            }
        }

        // If no swaps, the remaining prefix is already sorted
        if (!swapped) {
            for (size_t i = 0; i < n; ++i)
                callback(2, i, 0);
            return;
        }

        // Everything after last_swap is in final position this pass
        const size_t old_n = n;
        for (size_t k = last_swap; k < old_n; ++k)
            callback(2, k, 0);

        n = last_swap;
    }
    // First element is also in its correct place after the final pass
    callback(2, 0, 0);
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
 * @param callback Optional callback function to report each operation:
 * The callback receives events as (code, a, b):
 *  - code = 0: Compare(a, b)           — comparing indices a and b
 *  - code = 1: Swap(a, b)              — swapping indices a and b
 *  - code = 2: MarkSorted(a, ignored)  — index a is now in final sorted place
 */
template <typename Type, typename Callback = void (*)(size_t, size_t, size_t)>
void LinearInsertionSort(DynamicArray<Type>& array,
                                   Callback&& callback = [](size_t, size_t, size_t) -> void {}) {
    const size_t n = array.size();
    if (n <= 1 || isSorted(array)) {
        for (size_t i = 0; i < n; ++i)
            callback(2, i, 0);
        return;
    }

    // Mark first element as sorted
    callback(2, 0, 0);

    for (size_t i = 1; i < n; ++i) {
        Type key = array[i];
        size_t j = i;

        // Linear search backwards to find insertion position
        while (j > 0) {
            callback(0, j - 1, i); // Compare with key (stored at original position i)
            if (array[j - 1] > key) {
                callback(1, j - 1, j); // Swap (shift element right)
                array[j] = array[j - 1];
                --j;
            } else {
                break;
            }
        }

        // Insert key at position j
        array[j] = key;

        // Mark the newly sorted element
        callback(2, j, 0);
    }
    // First element is also in its correct place after the final pass
    callback(2, 0, 0);
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
 * @param callback Optional callback function to report each operation:
 * The callback receives events as (code, a, b):
 *  - code = 0: Compare(a, b)           — comparing indices a and b
 *  - code = 1: Swap(a, b)              — swapping indices a and b
 *  - code = 2: MarkSorted(a, ignored)  — index a is now in final sorted place
 */
template <typename Type, typename Callback = void (*)(size_t, size_t, size_t)>
void BinaryInsertionSort(DynamicArray<Type>& array,
                                   Callback&& callback = [](size_t, size_t, size_t) -> void {}) {
    const size_t n = array.size();
    if (n <= 1 || isSorted(array)) {
        for (size_t i = 0; i < n; ++i)
            callback(2, i, 0);
        return;
    }

    // Mark first element as sorted
    callback(2, 0, 0);

    for (size_t i = 1; i < n; ++i) {
        Type key = array[i];

        // Binary search for insertion position in [0, i)
        size_t left = 0, right = i;
        while (left < right) {
            size_t mid = left + (right - left) / 2;
            callback(0, mid, i);
            if (array[mid] <= key)
                left = mid + 1;
            else
                right = mid;
        }

        // Shift to make room
        for (size_t j = i; j > left; --j) {
            array[j] = array[j - 1];
            callback(1, j, j - 1); // Swap (shift element right)
        }

        // Insert key
        array[left] = key;
        // Mark the newly sorted element
        callback(2, left, 0);
    }
    // First element is also in its correct place after the final pass
    callback(2, 0, 0);
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
 * @param callback Optional callback function to report each operation:
 * The callback receives events as (code, a, b):
 *  - code = 0: Compare(a, b)           — comparing indices a and b
 *  - code = 1: Swap(a, b)              — swapping indices a and b
 *  - code = 2: MarkSorted(a, ignored)  — index a is now in final sorted place
 */
template <typename Type, typename Callback = void (*)(size_t, size_t, size_t)>
void QuickSort(DynamicArray<Type>& array,
               Callback&& callback = [](size_t, size_t, size_t) -> void {}) {
    const size_t n = array.size();
    if (n <= 1 || isSorted(array)) {
        for (size_t i = 0; i < n; ++i)
            callback(2, i, 0);
        return;
    }

    auto partition = [&](const size_t left, const size_t right) -> size_t {
        Type pivot = array[right]; // Lomuto: pivot is last element
        size_t i = left;

        for (size_t j = left; j < right; ++j) {
            callback(0, j, right); // compare A[j] with pivot at 'right'
            if (array[j] <= pivot) {
                if (i != j)
                    callback(1, i, j); // swap A[i] <-> A[j]
                swap(array[i++], array[j]);
            }
        }

        if (i != right)
            callback(1, i, right); // place pivot into final position
        swap(array[i], array[right]);
        callback(2, i, 0); // pivot at index i is now in its final place
        return i;
    };


    auto quick_sort = [&](auto&& self, size_t left, size_t right) -> void {
        while (left < right) {
            const size_t p = partition(left, right);

            // Sizes of the two partitions [left..p-1] and [p+1..right]
            const size_t leftSize  = (p > left)  ? (p - left)   : 0;
            const size_t rightSize = (p < right) ? (right - p)  : 0;

            if (leftSize < rightSize) {
                // Recurse on the smaller left side, iterate on the right
                if (p > 0) self(self, left, p - 1);
                left = p + 1;
            } else {
                // Recurse on the smaller right side, iterate on the left
                if (p + 1 <= right) self(self, p + 1, right);
                if (p == 0) break; // prevent underflow on p-1
                right = p - 1;
            }
        }

        // When left == right, a single element remains unsorted; mark it.
        if (left == right)
            callback(2, left, 0);
    };

    quick_sort(quick_sort, 0, n - 1);
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
    const size_t n = array.size();
    if (n <= 1 || isSorted(array))
        return;

    // Recursive merge sort on inclusive bounds [left, right]
    auto merge_sort = [&](auto&& self, size_t left, size_t right) -> void {
        if (left >= right)
            return;

        size_t mid = left + (right - left) / 2;

        // Sort both halves
        self(self, left, mid);
        self(self, mid + 1, right);

        size_t n1 = mid - left + 1;
        size_t n2 = right - mid;

        // Create temp arrays
        DynamicArray<Type> Left(n1), Right(n2);

        // Copy data to temp arrays
        for (int i = 0; i < n1; i++)
            Left.addLast(array[left + i]);
        for (int j = 0; j < n2; j++)
            Right.addLast(array[mid + 1 + j]);

        size_t i = 0, j = 0, k = left;

        // Merge the temp arrays back into array[left..right]
        while (i < n1 && j < n2) {
            if (Left[i] <= Right[j])
                array[k++] = Left[i++];
            else
                array[k++] = Right[j++];
        }

        // Copy the remaining elements of Left if there are any
        while (i < n1)
            array[k++] = Left[i++];

        // Copy the remaining elements of Right if there are any
        while (j < n2)
            array[k++] = Right[j++];
    };

    merge_sort(merge_sort, 0, n - 1);
}


/**
 * @brief Sorts the array in ascending (non-decreasing) order using a
 * divide-and-conquer merge-sort structure with an in-place merge.
 *
 * The array is recursively split into halves and each half is sorted. The
 * merge step is performed in-place by repeatedly moving the next out-of-order
 * element from the right half leftwards using adjacent swaps (insertion-style
 * merging). This keeps the algorithm stable and uses O(1) auxiliary space
 * (besides the recursion stack), but the merge can be quadratic when many
 * elements must be moved across the boundary.
 *
 * Note for float/double: Arrays containing NaN are unsupported for ordering;
 * results are unspecified.
 *
 * @par Complexity
 * - Time: Θ(n^2) in the worst case and typically on average for random inputs,
 *   due to potentially quadratic merging; best case can approach O(n log n)
 *   when the two halves are already almost merged.
 * - Space: O(1) auxiliary space for data (plus O(log n) recursion stack).
 *
 * @param array The array to sort.
 * @param callback Optional callback function to report each operation:
 * The callback receives events as (code, a, b):
 *  - code = 0: Compare(a, b)           — comparing indices a and b
 *  - code = 1: Swap(a, b)              — swapping indices a and b
 *  - code = 2: MarkSorted(a, ignored)  — index a is now in final sorted place
 */
template <typename Type, typename Callback = void (*)(size_t, size_t, size_t)>
void MergeSortInPlace(DynamicArray<Type>& array,
               Callback&& callback = [](size_t, size_t, size_t) -> void {}) {
    const size_t n = array.size();
    if (n <= 1 || isSorted(array)) {
        for (size_t i = 0; i < n; ++i)
            callback(2, i, 0);
        return;
    }

    // Recursive merge sort on inclusive bounds [left, right]
    auto merge_sort = [&](auto&& self, size_t left, size_t right) -> void {
        if (left >= right)
            return;

        size_t mid = left + (right - left) / 2;

        // Sort both halves
        self(self, left, mid);
        self(self, mid + 1, right);

        // Merge the two sorted halves in-place using adjacent swaps.
        size_t i = left;
        size_t j = mid + 1;

        while (i <= mid && j <= right) {
            callback(0, i, j); // compare
            if (array[i] <= array[j]) {
                ++i;
            } else {
                // Move array[j] into position i by swapping it leftwards
                size_t index = j;
                while (index > i) {
                    callback(1, index, index - 1); // swap
                    swap(array[index], array[index - 1]);
                    --index;
                }
                ++i;
                ++mid; // one more element now in left half
                ++j;
            }
        }
    };

    merge_sort(merge_sort, 0, n - 1);

    if (isSorted(array))
        for (size_t i = 0; i < n; ++i)
            callback(2, i, 0);
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
 * @param callback Optional callback function to report each operation:
 * The callback receives events as (code, a, b):
 *  - code = 0: Compare(a, b)           — comparing indices a and b
 *  - code = 1: Swap(a, b)              — swapping indices a and b
 *  - code = 2: MarkSorted(a, ignored)  — index a is now in final sorted place
 */
template <typename Type, typename Callback = void (*)(size_t, size_t, size_t)>
void HeapSort(DynamicArray<Type>& array,
               Callback&& callback = [](size_t, size_t, size_t) -> void {}) {
    const size_t n = array.size();
    if (n <= 1 || isSorted(array)) {
        for (size_t i = 0; i < n; ++i)
            callback(2, i, 0);
        return;
    }

    auto sift_down = [&](const size_t start, const size_t size) -> void {
        size_t i = start;
        while (true) {
            const size_t left = 2 * i + 1;
            if (left >= size)
                break;

            size_t largest = left;
            const size_t right = left + 1;

            if (right < size) {
                callback(0, right, left); // compare children
                if (array[right] > array[left])
                    largest = right;
            }

            callback(0, i, largest); // compare parent with largest child

            if (array[i] >= array[largest])
                break;

            if (i != largest)
                callback(1, i, largest); // swap parent with largest child
            swap(array[i], array[largest]);
            i = largest;
        }
    };

    // Make heap
    for (size_t i = (n - 2) / 2 + 1; i-- > 0;)
        sift_down(i, n);


    auto pop_heap = [&](const size_t size) -> void {
        if (size <= 1)
            return;
        callback(1, 0, size - 1); // swap root with last
        swap(array[0], array[size - 1]);
        callback(2, size - 1, 0); // mark sorted
        sift_down(0, size - 1);
    };

    // Repeatedly extract max to the end
    for (size_t heap_size = n; heap_size > 1; --heap_size)
        pop_heap(heap_size);

    callback(2, 0, 0);
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
void BinSort(DynamicArray<Type>& array, const size_t universe_size) {
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

    for (size_t b = 0; b < universe_size; ++b) {
        heads.addLast(nullptr);
        tails.addLast(nullptr);
    }

    // Phase 1: distribute elements into bins
    try {
        for (size_t i = 0; i < array.size(); ++i) {
            const Type v = array[i];
            const auto bin = static_cast<size_t>(v);
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
        for (size_t b = 0; b < universe_size; ++b) {
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
    size_t write = 0;
    for (size_t b = 0; b < universe_size; ++b) {
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
 * m = static_cast<size_t>(max_value - min_value + 1).
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
    if (span >= static_cast<U>(std::numeric_limits<size_t>::max()))
        throw std::length_error("BinSort: universe too large to index");

    const size_t m = static_cast<size_t>(span) + 1;

    struct Node {
        Type value;
        Node* next;
    };

    DynamicArray<Node*> heads(m);
    DynamicArray<Node*> tails(m);

    for (size_t b = 0; b < m; ++b) {
        heads.addLast(nullptr);
        tails.addLast(nullptr);
    }

    // Phase 1: distribute elements into bins
    try {
        for (size_t i = 0; i < array.size(); ++i) {
            const Type v = array[i];
            if (v < min_value || v > max_value)
                throw std::out_of_range(
                    "BinSort: value out of [min,max] universe");

            // Compute bin index in unsigned domain without narrowing
            const auto bin = static_cast<size_t>(static_cast<U>(v) - umin);

            Node* node = new Node{v, nullptr};
            if (!heads[bin]) {
                heads[bin] = tails[bin] = node;
            } else {
                tails[bin]->next = node;
                tails[bin] = node;
            }
        }
    } catch (...) {
        for (size_t b = 0; b < m; ++b) {
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
    size_t write = 0;
    for (size_t b = 0; b < m; ++b) {
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

    const size_t n = array.size();
    if (n <= 1 || isSorted(array))
        return;

    using U = std::make_unsigned_t<Type>;

    constexpr size_t RADIX = 256;
    constexpr size_t BYTE_MASK = 0xFFu;
    const size_t bytes = sizeof(Type);

    // Temporary buffer for stable distribution
    DynamicArray<Type> temp(n);
    for (size_t i = 0; i < n; ++i)
        temp.addLast(Type());

    for (size_t pass = 0; pass < bytes; ++pass) {
        size_t count[RADIX] = {};
        const bool msb_pass = (pass + 1 == bytes);
        const bool use_array_as_source = (pass % 2 == 0);

        // Counting occurrences from the correct source
        if (use_array_as_source) {
            for (size_t i = 0; i < n; ++i) {
                U u = static_cast<U>(array[i]);
                auto digit = static_cast<size_t>((u >> (8 * pass)) & BYTE_MASK);
                if constexpr (std::numeric_limits<Type>::is_signed)
                    if (msb_pass)
                        digit ^= 0x80u;
                ++count[digit];
            }
        } else {
            for (size_t i = 0; i < n; ++i) {
                U u = static_cast<U>(temp[i]);
                auto digit = static_cast<size_t>((u >> (8 * pass)) & BYTE_MASK);
                if constexpr (std::numeric_limits<Type>::is_signed)
                    if (msb_pass)
                        digit ^= 0x80u;
                ++count[digit];
            }
        }

        // Prefix sums to get starting positions
        size_t pos[RADIX];
        pos[0] = 0;
        for (size_t d = 1; d < RADIX; ++d)
            pos[d] = pos[d - 1] + count[d - 1];

        // Stable distribution: even passes write to temp, odd passes back to
        // array
        if (use_array_as_source) {
            for (size_t i = 0; i < n; ++i) {
                U u = static_cast<U>(array[i]);
                auto digit = static_cast<size_t>((u >> (8 * pass)) & BYTE_MASK);
                if constexpr (std::numeric_limits<Type>::is_signed)
                    if (msb_pass)
                        digit ^= 0x80u;
                temp[pos[digit]++] = array[i];
            }
        } else {
            for (size_t i = 0; i < n; ++i) {
                U u = static_cast<U>(temp[i]);
                auto digit = static_cast<size_t>((u >> (8 * pass)) & BYTE_MASK);
                if constexpr (std::numeric_limits<Type>::is_signed)
                    if (msb_pass)
                        digit ^= 0x80u;
                array[pos[digit]++] = temp[i];
            }
        }
    }

    // If number of passes is odd, last write was into temp -> copy back
    if ((bytes % 2) == 1)
        for (size_t i = 0; i < n; ++i)
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

    const size_t n = array.size();
    if (n <= 1 || isSorted(array))
        return;

    using U = std::make_unsigned_t<Type>;
    constexpr size_t RADIX = 256;

    // Helper to get the byte at position 'byte' (0-based from LSB)
    auto get_digit = [](U u, const int byte) -> size_t {
        return static_cast<size_t>((u >> (8 * byte)) & 0xFFu);
    };

    // Reusable temporary buffer for stable distribution
    DynamicArray<Type> temp(n);
    for (size_t i = 0; i < n; ++i)
        temp.addLast(Type());

    // Self-recursive lambda via fixpoint pattern: pass 'self' explicitly
    auto msd_rec = [&](auto&& self, const size_t lo, const size_t hi,
                       const int byte, const bool flip_msb) -> void {
        if (const size_t len = hi - lo; len <= 1 || byte < 0)
            return;

        size_t count[RADIX] = {};

        // Count digits
        for (size_t i = lo; i < hi; ++i) {
            U u = static_cast<U>(array[i]);
            size_t d = get_digit(u, byte);

            if (flip_msb)
                if constexpr (std::numeric_limits<Type>::is_signed)
                    d ^= 0x80u; // map sign bit so negatives come first

            ++count[d];
        }

        // Compute starting positions in [lo, hi)
        size_t pos[RADIX];
        pos[0] = lo;
        for (size_t d = 1; d < RADIX; ++d)
            pos[d] = pos[d - 1] + count[d - 1];

        // Stable distribute into temp
        for (size_t i = lo; i < hi; ++i) {
            U u = static_cast<U>(array[i]);
            size_t d = get_digit(u, byte);

            if (flip_msb)
                if constexpr (std::numeric_limits<Type>::is_signed)
                    d ^= 0x80u;

            temp[pos[d]++] = array[i];
        }

        // Copy back to array
        for (size_t i = lo; i < hi; ++i)
            array[i] = temp[i];

        // Recurse on each non-empty bucket with next byte (no sign flip below
        // MSB)
        size_t start = lo;
        for (const size_t cnt : count) {
            if (cnt > 1)
                self(self, start, start + cnt, byte - 1, false);
            start += cnt;
        }
    };

    // Start from the MSB (byte index sizeof(Type)-1), flip sign bit at MSB
    msd_rec(msd_rec, 0, n, static_cast<int>(sizeof(Type)) - 1, true);
}


} // namespace array_algorithms


#endif // DYNAMIC_ARRAY_ALGORITHMS_HPP
