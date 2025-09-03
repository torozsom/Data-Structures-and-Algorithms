# 📘 Array Algorithms Library

This directory contains implementations of fundamental searching and sorting algorithms used throughout the project.  All algorithms operate on the custom `DynamicArray` container and are written in modern C++23.  This README explains the idea behind each algorithm, outlines its time and space complexity, and highlights typical scenarios where it shines.

## Table of Contents
- [Searching Algorithms](#searching-algorithms)
    - [Linear Search](#linear-search)
    - [Binary Search](#binary-search)
- [Sorting Algorithms](#sorting-algorithms)
    - [Bubble Sort](#bubble-sort)
    - [Improved Bubble Sort](#improved-bubble-sort)
    - [Linear Insertion Sort](#linear-insertion-sort)
    - [Binary Insertion Sort](#binary-insertion-sort)
    - [Quick Sort](#quick-sort)
    - [Merge Sort](#merge-sort)
    - [In‑Place Merge Sort](#in-place-merge-sort)
    - [Heap Sort](#heap-sort)
    - [Bin Sort (Fixed Universe)](#bin-sort-fixed-universe)
    - [Bin Sort (Range Universe)](#bin-sort-range-universe)
    - [Radix Sort – Least Significant Digit](#radix-sort--least-significant-digit)
    - [Radix Sort – Most Significant Digit](#radix-sort--most-significant-digit)
- [Choosing an Algorithm](#choosing-an-algorithm)

---

## Searching Algorithms

### Linear Search
**Idea.** Scan elements sequentially from the beginning of the array until the target value is found or the end is reached.

**Steps.**
1. Start at index `0`.
2. Compare the current element with the target.
3. If it matches, return the index; otherwise move to the next element.
4. Continue until the element is found or the array ends.

**Complexity.**
- **Time:** `O(n)` – each of the `n` elements may need to be inspected.
- **Space:** `O(1)` – only a few loop variables are required.

**Use When.**
- The array is small or unsorted.
- Only a few searches are performed, making preprocessing (e.g., sorting) unnecessary.

### Binary Search
**Idea.** Repeatedly divide a sorted array in half to locate a target value.

**Steps.**
1. Maintain `left` and `right` bounds for the current search range.
2. Probe the middle element and compare it to the target.
3. If equal, return the index.  If the middle is smaller, discard the left half; otherwise discard the right half.
4. Repeat until the range is empty.

**Complexity.**
- **Time:** `O(log n)` – the search range halves with each iteration.
- **Space:** `O(1)` – bounds and indices occupy constant memory.

**Use When.**
- Data is already sorted or can be kept sorted.
- Fast repeated lookups are required.

---

## Sorting Algorithms

### Bubble Sort
**Idea.** Repeatedly sweep through the array, swapping adjacent elements that are out of order so larger values "bubble" to the end.

**Steps.**
1. Iterate from the start to the penultimate element.
2. Compare each pair of adjacent elements and swap if needed.
3. After each pass, the largest unsorted element is placed at its final position.
4. Repeat `n-1` passes until the array is sorted.

**Complexity.**
- **Time:** Worst and average `O(n²)` comparisons and swaps; best `O(n)` when already sorted.
- **Space:** `O(1)` – sorting occurs in-place.

**Use When.**
- Teaching purposes and simple visualisations.
- Arrays are tiny and nearly sorted; otherwise it is inefficient.

### Improved Bubble Sort
**Idea.** Enhances Bubble Sort by stopping early if a pass makes no swaps and by shrinking the portion that still needs checking.

**Steps.**
1. Perform a standard bubble sweep.
2. Record whether any swap occurred and the position of the last swap.
3. If no swaps happened, the array is sorted and the algorithm terminates.
4. The next pass only processes elements before the last swap index.

**Complexity.**
- **Time:** Still `O(n²)` in the worst case but faster for nearly sorted data; best case `O(n)`.
- **Space:** `O(1)`.

**Use When.**
- Arrays are almost sorted and you want a simple improvement over plain Bubble Sort.

### Linear Insertion Sort
**Idea.** Build a sorted prefix by inserting each new element into the correct location using linear search.

**Steps.**
1. Treat the first element as a sorted subarray.
2. Take the next element (the "key") and scan leftward until a smaller element is found.
3. Shift larger elements to the right and insert the key.
4. Repeat for all elements.

**Complexity.**
- **Time:** `O(n²)` comparisons and shifts; best case `O(n)` when array already sorted.
- **Space:** `O(1)`.

**Use When.**
- Very small collections.
- Arrays that are mostly sorted with only a few elements out of place.

### Binary Insertion Sort
**Idea.** Same as insertion sort but uses binary search to locate the insertion position inside the sorted prefix, reducing comparisons.

**Steps.**
1. For each new element, perform binary search on the sorted portion to find the insertion index.
2. Shift larger elements to the right to make room.
3. Insert the element and continue.

**Complexity.**
- **Time:** `O(n²)` due to shifting, though only `O(n log n)` comparisons are required.
- **Space:** `O(1)`.

**Use When.**
- Slightly faster than linear insertion sort for medium-sized arrays.
- Useful where comparison cost is high but shifts are cheap.

### Quick Sort
**Idea.** Divide-and-conquer: partition the array around a pivot so that smaller elements go left and larger elements go right, then recursively sort the partitions.

**Steps.**
1. Choose a pivot (here the last element).
2. Partition the array so elements `<=` pivot precede it.
3. Recursively sort the left and right partitions.
4. Tail recursion is converted to iteration on the larger partition to save stack space.

**Complexity.**
- **Time:** Average `O(n log n)`; worst `O(n²)` when pivots are consistently poor (e.g., already sorted data with last-element pivot).
- **Space:** `O(log n)` for the recursion stack on average.

**Use When.**
- General-purpose sorting where average performance matters more than worst-case guarantees.
- Not stable, so avoid when relative ordering of equal elements must be preserved.

### Merge Sort
**Idea.** Recursively split the array in half, sort each half, then merge the two sorted halves into a temporary buffer.

**Steps.**
1. Divide the array into two halves.
2. Recursively sort both halves.
3. Merge them into a temporary array while preserving order.
4. Copy the merged result back.

**Complexity.**
- **Time:** Always `O(n log n)` because the array is halved each level and merging is linear.
- **Space:** `O(n)` extra buffer plus `O(log n)` recursion stack.

**Use When.**
- Large datasets where stability is required.
- Good for external sorting and data stored on slower media.

### In‑Place Merge Sort
**Idea.** Maintains the divide-and-conquer structure of merge sort but performs the merge step by repeatedly swapping elements in place, avoiding an auxiliary buffer.

**Steps.**
1. Recursively sort the two halves.
2. Walk through both halves; whenever an element in the right half is smaller, move it leftwards into position via adjacent swaps.
3. Continue until all elements are in order.

**Complexity.**
- **Time:** Worst case `Θ(n²)` because elements may travel across the boundary one swap at a time.
- **Space:** `O(1)` auxiliary space beyond the recursion stack.

**Use When.**
- Memory is extremely constrained yet stability is still desired.
- Acceptable when datasets are small or nearly merged already.

### Heap Sort
**Idea.** Build a max‑heap in place, then repeatedly move the maximum element to the end and restore the heap property for the remaining elements.

**Steps.**
1. Transform the array into a max‑heap using sift‑down operations.
2. Swap the root with the last element, shrinking the heap by one.
3. Sift the new root down to maintain the heap.
4. Repeat until only one element remains.

**Complexity.**
- **Time:** `O(n log n)` – heap construction is `O(n)` and each of the `n` extractions costs `O(log n)`.
- **Space:** `O(1)` aside from variables; performed entirely in place.

**Use When.**
- Need guaranteed `O(n log n)` time with minimal memory.
- Useful in embedded environments; not stable.

### Bin Sort (Fixed Universe)
**Idea.** For integral keys in the known range `[0, m)`, maintain `m` linked‑list bins and distribute each element into its bin, then concatenate bins.

**Steps.**
1. Create `m` empty bins.
2. For each array element, append it to bin `B[value]`.
3. Traverse bins in increasing order and write elements back to the array.

**Complexity.**
- **Time:** `O(n + m)` for distributing and collecting.
- **Space:** `O(n + m)` for node storage and bin heads.

**Use When.**
- Keys are integers within a small, fixed universe.
- Essentially a bucket sort variant that preserves order (stable).

### Bin Sort (Range Universe)
**Idea.** Similar to the fixed‑universe version but bins cover a contiguous range `[min, max]` mapped to zero‑based indices.

**Steps.**
1. Compute `m = max - min + 1` bins.
2. Map each value to index `value - min` and append to that bin.
3. Concatenate bins in order back into the array.

**Complexity.**
- **Time:** `O(n + m)`.
- **Space:** `O(n + m)`.

**Use When.**
- The value range is contiguous but does not start at zero.
- Useful for small ranges where allocating bins is inexpensive.

### Radix Sort – Least Significant Digit
**Idea.** Perform a stable counting sort on each byte from least significant to most significant, flipping the sign bit on the final pass to handle negatives.

**Steps.**
1. Allocate a temporary buffer and counting array of size `256`.
2. For each byte position:
    - Count digit occurrences.
    - Compute prefix sums for positions.
    - Distribute elements to the destination based on the current byte.
    - Alternate between the original array and the buffer each pass.
3. Copy back if an odd number of passes were performed.

**Complexity.**
- **Time:** `O(k · n)` where `k` is number of bytes in the key type.
- **Space:** `O(n + k)` for the buffer and counting array.

**Use When.**
- Sorting fixed‑width integers or data with few bytes.
- Stable and good when comparison sorts are too slow.

### Radix Sort – Most Significant Digit
**Idea.** Recursively sort on the most significant byte first, partitioning into buckets and refining each bucket on the next byte.

**Steps.**
1. Count digits of the current byte to determine bucket sizes.
2. Distribute elements to a temporary buffer according to their digit.
3. Copy back and recursively process each bucket on the next less significant byte.
4. The most significant pass flips the sign bit for signed types.

**Complexity.**
- **Time:** `O(k · n)` where `k` is the number of bytes.
- **Space:** `O(n + k)` using a reusable buffer plus recursion stack.

**Use When.**
- Sorting large integers where most significant differences appear early.
- Efficient when prefixes quickly distinguish numbers.

---

## Choosing an Algorithm
Selecting the right algorithm depends on array size, existing order, memory limits, and stability requirements:

- **Small or nearly sorted arrays:** insertion sorts or improved bubble sort.
- **General purpose:** quick sort for speed, merge sort for guaranteed `O(n log n)` and stability, heap sort when memory is tight and worst‑case guarantees are needed.
- **Integers in known ranges:** bin sort or radix sort provide linear performance.
- **Single lookups:** linear search; **frequent lookups over sorted data:** binary search.

The implementations here emphasize clarity and educational value while providing realistic performance characteristics.  They serve both as production‑ready utilities and as a basis for visualising algorithm behaviour.