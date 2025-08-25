#ifndef DYNAMIC_ARRAY_ALGORITHMS_HPP
#define DYNAMIC_ARRAY_ALGORITHMS_HPP


#include <DynamicArray.hpp>


namespace algo {

using data_structs::DynamicArray;

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

    // Temporary merged buffer
    Type* temp = new Type[n];

    std::size_t i = left;
    std::size_t j = mid + 1;
    std::size_t k = 0;

    // Merge into temp
    while (i <= mid && j <= right) {
        if (array[i] <= array[j])
            temp[k++] = array[i++];
        else
            temp[k++] = array[j++];
    }

    while (i <= mid)
        temp[k++] = array[i++];
    while (j <= right)
        temp[k++] = array[j++];

    // Copy back
    for (std::size_t t = 0; t < n; ++t)
        array[left + t] = temp[t];

    delete[] temp;
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


/// Checks if the array is sorted in ascending order.
template <typename Type>
bool isSorted(const DynamicArray<Type>& array) noexcept {
    for (std::size_t i = 1; i < array.size(); ++i)
        if (array[i] < array[i - 1])
            return false;

    return true;
}


/*** Sorting Algorithms ***/


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

    // Allocate bin heads and tails (ensure no leak if tails allocation throws)
    Node** heads = new Node*[universe_size];
    try {
        Node** tails = new Node*[universe_size];
        for (std::size_t b = 0; b < universe_size; ++b) {
            heads[b] = nullptr;
            tails[b] = nullptr;
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
            delete[] heads;
            delete[] tails;
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

        delete[] heads;
        delete[] tails;
    } catch (...) {
        delete[] heads;
        throw;
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

    // Allocate bin heads and tails (ensure no leak if tails allocation throws)
    Node** heads = new Node*[m];
    try {
        Node** tails = new Node*[m];
        for (std::size_t b = 0; b < m; ++b) {
            heads[b] = nullptr;
            tails[b] = nullptr;
        }

        // Phase 1: distribute elements into bins
        try {
            for (std::size_t i = 0; i < array.size(); ++i) {
                const Type v = array[i];
                if (v < min_value || v > max_value)
                    throw std::out_of_range(
                        "BinSort: value out of [min,max] universe");

                // Compute bin index in unsigned domain without narrowing
                const auto bin =
                    static_cast<std::size_t>(static_cast<U>(v) - umin);

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
            delete[] heads;
            delete[] tails;
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

        delete[] heads;
        delete[] tails;
    } catch (...) {
        delete[] heads;
        throw;
    }
}


/*** Searching Algorithms ***/


/**
 * @brief Performs a linear search in the array for the given element.
 *
 * Scans the array from start to end, comparing each element to the target.
 * Returns the index of the first occurrence of the element, or size_ if not
 * found.
 *
 * @param array The array to search in.
 * @param element The element to search for.
 * @return The index of the found element, or size_ if not found.
 *
 * @par Complexity
 * - O(n) time.
 * - O(1) space.
 */
template <typename Type>
std::size_t LinearSearch(const DynamicArray<Type>& array,
                         const Type& element) noexcept {
    return array.contains(element);
}


/**
 * @brief Performs a binary search for the given element.
 *
 * Requires the array to be sorted in non-decreasing (ascending) order.
 * Returns the index of the first position where element could be inserted
 * without violating the order among equals (lower_bound semantics).
 *
 * Note for float/double: Arrays containing NaN are unsupported for ordering;
 * results are unspecified.
 *
 * @param array The array to search in.
 * @param element The element to search for.
 * @return The index of the found element, or size_ if not found.
 *
 * @par Complexity
 * - O(log n) time.
 * - O(1) space.
 */
template <typename Type>
std::size_t BinarySearch(const DynamicArray<Type>& array,
                         const Type& element) noexcept {
    // left: inclusive lower bound, right: exclusive upper bound
    std::size_t left = 0;
    std::size_t right = array.size();

    while (left < right) {
        std::size_t middle = left + (right - left) / 2;
        if (array[middle] < element)
            left = middle + 1;
        else
            right = middle;
    }

    // left is the first index not less than element
    if (left < array.size() && array[left] == element)
        return left;

    return array.size();
}

} // namespace algo

#endif // DYNAMIC_ARRAY_ALGORITHMS_HPP
