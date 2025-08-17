#ifndef DYNAMICARRAY_HPP
#define DYNAMICARRAY_HPP


#include <algorithm>
#include <cassert>
#include <limits>
#include <new>
#include <stdexcept>
#include <type_traits>


/**
 * @class DynamicArray
 * @brief A vector-like, resizable, contiguous container with explicit lifetime
 * and alignment control.
 *
 * This template implements a growable array that owns a single contiguous block
 * of raw storage and constructs elements in-place via placement new. It is
 * designed to:
 *   - Provide O(1) indexed access and cache-friendly iteration.
 *   - Support appends, prepends, middle inserts, and removals.
 *   - Offer strong exception safety for the operations that change capacity or
 * rebuild storage.
 *   - Correctly handle over-aligned types by using the aligned forms of
 * ::operator new/delete.
 *
 * Elements are only constructed for the logical size; capacity refers to the
 * amount of raw storage available to hold additional elements without
 * reallocation. The container manually manages object lifetimes
 * (construct/destroy) independently from raw storage (allocate/deallocate).
 *
 * @tparam Type The element type stored by the container.
 *
 * @par Design Highlights
 * - Contiguous storage for cache locality and pointer/pointer-range iteration.
 * - Amortized O(1) append via doubling growth policy (up to MAX_CAPACITY).
 * - Strong exception safety for capacity-changing operations using the
 * allocate+construct+commit pattern.
 * - Over-alignment correctness by passing `std::align_val_t(alignof(Type))` to
 * allocation/deallocation.
 *
 * @par Growth Policy
 * - Capacity doubles on demand until MAX_CAPACITY. When near the limit, it
 * clamps to MAX_CAPACITY.
 *
 * @par Exception Safety
 * - insert / emplaceAt: strong guarantee (the array is unchanged on failure).
 * - removeAt: basic guarantee if assignment throws during shifting; strong
 * during relocation path.
 * - reserve / resize / clone: strong guarantee (build a fresh buffer, then
 * commit).
 *
 * @par Invalidation
 * - Any insert/remove/resize operation invalidates all pointers, references,
 * and iterators.
 *
 * @par Moved-from State
 * - A moved-from DynamicArray is valid and empty (begin()==end()).
 */
template <typename Type>
class DynamicArray {

    Type* data_;
    std::size_t size_;
    std::size_t capacity_;

    static constexpr std::size_t DEFAULT_CAPACITY = 5;

    static constexpr std::size_t MAX_CAPACITY =
        std::numeric_limits<std::size_t>::max() / sizeof(Type);

    static constexpr std::size_t MAX_SAFE_CAPACITY =
        std::numeric_limits<std::size_t>::max() / sizeof(Type) / 2;


    /**
     * @brief Allocate raw, uninitialized storage for a given number of elements
     * with correct alignment.
     *
     * Allocates a single contiguous block of bytes sufficient to hold the
     * requested number of objects of Type, aligned to alignof(Type). No
     * constructors are run here; the caller is responsible for constructing
     * elements via placement new.
     *
     * @param storage_size Number of elements' worth of storage to allocate. May
     * be zero.
     * @return Pointer to uninitialized storage; returns nullptr when
     * storage_size == 0.
     *
     * @par Complexity
     * - O(1) aside from the cost of the underlying allocation call.
     *
     * @par Exception Safety
     * - Strong: if an exception is thrown, no storage is leaked.
     *
     * @throws std::bad_alloc If storage_size > MAX_CAPACITY or the allocation
     * fails.
     */
    static Type* allocate(const std::size_t storage_size) {
        if (storage_size == 0)
            return nullptr;

        if (storage_size > MAX_CAPACITY)
            throw std::bad_alloc();

        return static_cast<Type*>(operator new(
            sizeof(Type) * storage_size,
            static_cast<std::align_val_t>(alignof(Type))));
    }


    /**
     * @brief Deallocate raw storage previously obtained via allocate.
     *
     * Calls the aligned form of ::operator delete. Passing nullptr is allowed
     * and is a no-op. The caller must ensure that all constructed objects in
     * the storage have been destroyed prior to deallocation.
     *
     * @param storage Pointer returned by allocate. May be nullptr.
     *
     * @par Exception Safety
     * - No-throw.
     */
    static void deallocate(Type* storage) noexcept {
        if (storage != nullptr)
            ::operator delete(storage,
                              static_cast<std::align_val_t>(alignof(Type)));
    }


    /**
     * @brief Destroy all constructed elements in the current buffer.
     *
     * Calls the destructor for each element in the range [data_, data_ +
     * size_). Raw storage remains allocated; this function only ends element
     * lifetimes.
     *
     * @par Complexity
     * - O(size()) destructor invocations.
     *
     * @par Exception Safety
     * - If Type's destructor throws (discouraged), the exception propagates.
     */
    void destroyArrayElements() {
        for (std::size_t i = 0; i < size_; ++i)
            data_[i].~Type();
    }


    /**
     * @brief Copy-construct elements from a source range into a destination
     * buffer.
     *
     * Constructs each element in order using Type's copy constructor. If
     * construction throws partway, already-constructed destination elements are
     * destroyed and the exception is rethrown.
     *
     * @param source_begin Pointer to the first source element.
     * @param source_end Pointer one past the last source element.
     * @param destination Destination buffer where elements will be
     * placement-new constructed.
     * @return Pointer to one past the last constructed destination element.
     *
     * @par Precondition
     * - The source and destination ranges must not overlap.
     *
     * @par Exception Safety
     * - Strong: partial constructions are cleaned up before rethrow.
     */
    Type* copyConstructElements(const Type* source_begin,
                                const Type* source_end, Type* destination) const
        noexcept(std::is_nothrow_copy_constructible_v<Type>) {
        assert(destination != nullptr);
        Type* current = destination;
        try {
            for (const Type* it = source_begin; it != source_end;
                 ++it, ++current)
                new (current) Type(*it); // copy-construct
            return current;
        } catch (...) {
            for (Type* it = destination; it != current; ++it)
                it->~Type();
            throw;
        }
    }


    /**
     * @brief Move-construct elements from a source range into a destination
     * buffer.
     *
     * Constructs each element in order using Type's move constructor. If
     * construction throws partway, already-constructed destination elements are
     * destroyed and the exception is rethrown. The sources remain valid but are
     * moved-from.
     *
     * @param source_begin Pointer to the first source element.
     * @param source_end Pointer one past the last source element.
     * @param destination Destination buffer where elements will be
     * placement-new constructed.
     *
     * @return Pointer to one past the last constructed destination element.
     *
     * @par Precondition
     * - The source and destination ranges must not overlap.
     *
     * @par Exception Safety
     * - Strong: partial constructions are cleaned up before rethrow.
     */
    Type* moveConstructElements(Type* source_begin, Type* source_end,
                                Type* destination) const
        noexcept(std::is_nothrow_move_constructible_v<Type>) {
        assert(destination != nullptr);
        Type* current = destination;
        try {
            for (Type* it = source_begin; it != source_end; ++it, ++current)
                new (current) Type(std::move(*it)); // move-construct
            return current;
        } catch (...) {
            for (Type* it = destination; it != current; ++it)
                it->~Type();
            throw;
        }
    }


    /**
     * @brief Resize the underlying capacity to a new value (size unchanged).
     *
     * If the requested capacity differs from the current capacity, this
     * function allocates a new buffer, move-/copy-constructs all existing
     * elements into it (preferring nothrow-move where available), destroys the
     * old elements, and then replaces the old storage. The logical size is
     * preserved.
     *
     * @param new_capacity Requested capacity in elements.
     *
     * @par Complexity
     * - O(size()) constructions + O(size()) destructions when a resize occurs.
     *
     * @par Exception Safety
     * - Strong: elements are constructed into fresh storage and committed only
     * on success.
     *
     * @throws std::invalid_argument If new_capacity < size().
     * @throws std::bad_alloc If new_capacity > MAX_CAPACITY or allocation
     * fails.
     */
    void resize(std::size_t new_capacity) {
        if (new_capacity == capacity_)
            return;

        if (new_capacity < DEFAULT_CAPACITY)
            new_capacity = DEFAULT_CAPACITY;

        if (new_capacity < size_)
            throw std::invalid_argument("New capacity is too small");

        if (new_capacity > MAX_CAPACITY)
            throw std::bad_alloc();

        Type* new_data = allocate(new_capacity);
        Type* new_data_end = new_data;
        try {
            if constexpr (std::is_nothrow_move_constructible_v<Type> ||
                          !std::is_copy_constructible_v<Type>)
                new_data_end =
                    moveConstructElements(data_, data_ + size_, new_data);
            else
                new_data_end =
                    copyConstructElements(data_, data_ + size_, new_data);
        } catch (...) {
            for (Type* it = new_data; it != new_data_end; ++it)
                it->~Type();
            deallocate(new_data);
            throw;
        }

        destroyArrayElements();
        deallocate(data_);
        data_ = new_data;
        capacity_ = new_capacity;
    }


    /**
     * @brief Shrink capacity by half when the array becomes sparse.
     *
     * If size() <= capacity()/4 and capacity() > DEFAULT_CAPACITY, the capacity
     * is reduced by half (not below the default). This reallocation is
     * performed with the same strong exception-safety as resize.
     *
     * @par Complexity
     * - O(size()) constructions/destructions only when a shrink occurs.
     */
    void shrinkIfNecessary() {
        if (size_ <= capacity_ / 4 && capacity_ > DEFAULT_CAPACITY)
            resize(capacity_ / 2);
    }


    /**
     * @brief Ensure there is room for one more element (grow by doubling if
     * full).
     *
     * If size() == capacity(), this doubles capacity (clamped to MAX_CAPACITY).
     * The growth uses resize, preserving the strong exception guarantee.
     *
     * @par Exception Safety
     * - Strong (via resize).
     *
     * @throws std::length_error If already at MAX_CAPACITY.
     */
    void ensureCapacity() {
        if (capacity_ == 0) {
            resize(DEFAULT_CAPACITY);
            return;
        }

        if (size_ == capacity_) {
            if (capacity_ == MAX_CAPACITY)
                throw std::length_error("DynamicArray capacity limit");
            resize(capacity_ > MAX_SAFE_CAPACITY ? MAX_CAPACITY
                                                 : capacity_ * 2);
        }
    }


    /**
     * @brief Build a fresh buffer with a new element placed at a given index,
     * then commit.
     *
     * Implements the "fresh buffer then commit" insert/emplace strategy for
     * middle inserts. It allocates a new buffer (same capacity), constructs the
     * prefix [0, idx), then the new element at idx, then the suffix [idx,
     * size()). On any failure, all partially constructed elements in the new
     * buffer are destroyed and the original array is left unchanged.
     *
     * @tparam CtorArgs Argument types forwarded to Type's constructor for the
     * inserted element.
     * @param idx Insertion index (must satisfy 0 <= idx < size() when called
     * from insert/emplace paths).
     * @param args Constructor arguments forwarded to Type for the new element.
     *
     * @par Complexity
     * - O(size()) constructions + O(size()) destructions.
     *
     * @par Exception Safety
     * - Strong: original buffer remains intact unless the entire rebuild
     * succeeds.
     */
    template <typename... CtorArgs>
    void rebuildBuffer(const std::size_t idx, CtorArgs&&... args) {
        static_assert(std::is_constructible_v<Type, CtorArgs&&...>);
        Type* new_data = allocate(capacity_);
        Type* constructed_end = new_data;

        try {
            if constexpr (std::is_nothrow_move_constructible_v<Type> ||
                          !std::is_copy_constructible_v<Type>)
                constructed_end =
                    moveConstructElements(data_, data_ + idx, new_data);
            else
                constructed_end =
                    copyConstructElements(data_, data_ + idx, new_data);

            new (new_data + idx) Type(std::forward<CtorArgs>(args)...);
            constructed_end = new_data + idx + 1;

            if constexpr (std::is_nothrow_move_constructible_v<Type> ||
                          !std::is_copy_constructible_v<Type>)
                constructed_end = moveConstructElements(
                    data_ + idx, data_ + size_, constructed_end);
            else
                constructed_end = copyConstructElements(
                    data_ + idx, data_ + size_, constructed_end);
        } catch (...) {
            for (Type* it = new_data; it != constructed_end; ++it)
                it->~Type();
            deallocate(new_data);
            throw;
        }

        destroyArrayElements();
        deallocate(data_);
        data_ = new_data;
        ++size_;
    }


    /**
     * @brief Erase by left-shifting using assignment (for assignable types).
     *
     * Overwrites the range [from_idx .. size()-2] by assigning from the
     * subsequent element, destroys the last element, and decrements the size.
     *
     * @param from_idx Index of the element to remove (0 <= from_idx < size()).
     *
     * @par Complexity
     * - O(n) assignments and one destruction.
     *
     * @par Exception Safety
     * - Basic: if an assignment throws, the invariant size() is left unchanged
     * and all elements remain valid (the container can be cleared).
     */
    void shiftLeftAssignables(const std::size_t from_idx) {
        for (std::size_t i = from_idx; i < size_ - 1; ++i)
            data_[i] = std::move(data_[i + 1]);
        data_[size_ - 1].~Type();
        --size_;
    }


    /**
     * @brief Erase by left-shifting via construct+destroy relocation (for
     * non-assignable types).
     *
     * Destroys the element at from_idx, then for each subsequent position
     * constructs a new object in-place from the next element (move if nothrow;
     * otherwise copy), and destroys the old source. If a construction throws,
     * partially constructed elements are destroyed and the size is reset to the
     * last successfully relocated index.
     *
     * @param from_idx Index of the element to remove (0 <= from_idx < size()).
     *
     * @par Complexity
     * - O(n) constructions/destructions.
     *
     * @par Exception Safety
     * - Strong during the relocation loop: partial progress is rolled back on
     * throw.
     */
    void shiftLeftNonassignables(std::size_t from_idx) {
        data_[from_idx].~Type();
        std::size_t j = from_idx;
        try {
            for (; j + 1 < size_; ++j) {
                Type* dst = data_ + j;
                Type* src = data_ + (j + 1);
                if constexpr (std::is_nothrow_move_constructible_v<Type> ||
                              !std::is_copy_constructible_v<Type>)
                    new (dst) Type(std::move(*src));
                else
                    new (dst) Type(*src);
                src->~Type();
            }
        } catch (...) {
            for (std::size_t k = j + 1; k < size_; ++k)
                data_[k].~Type();
            size_ = j;
            throw;
        }
        --size_;
    }


  public:
    /**
     * Default constructor that initializes the dynamic array with a default
     * capacity.
     */
    DynamicArray() : data_(nullptr), size_(0), capacity_(DEFAULT_CAPACITY) {
        data_ = allocate(capacity_);
    }

    /**
     * Constructor that initializes the dynamic array with a specified initial
     * size and capacity. If the initial size is greater than zero, the initial
     * data must not be null.
     *
     * @param initial_data Pointer to the initial data to be copied into the
     * dynamic array.
     * @param initial_size The number of elements in the initial data.
     */
    DynamicArray(const Type* initial_data, const std::size_t initial_size)
        : data_(nullptr), size_(initial_size),
          capacity_(initial_size < DEFAULT_CAPACITY ? DEFAULT_CAPACITY
                                                    : initial_size) {
        if (initial_size > 0 && initial_data == nullptr)
            throw std::invalid_argument("Initial data cannot be null if "
                                        "initial size is greater than zero");

        data_ = allocate(capacity_);
        try {
            if (size_ > 0)
                copyConstructElements(initial_data, initial_data + size_,
                                      data_);
        } catch (...) {
            deallocate(data_);
            throw;
        }
    }

    /// Copy constructor
    DynamicArray(const DynamicArray& other)
        : data_(allocate(other.capacity_)), size_(other.size_),
          capacity_(other.capacity_) {
        try {
            copyConstructElements(other.data_, other.data_ + size_, data_);
        } catch (...) {
            deallocate(data_);
            throw;
        }
    }

    /// Move constructor
    DynamicArray(DynamicArray&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    /// Copy assignment operator
    DynamicArray& operator=(const DynamicArray& other) {
        if (this == &other)
            return *this;

        Type* new_data = allocate(other.capacity_);
        try {
            copyConstructElements(other.data_, other.data_ + other.size_,
                                  new_data);
        } catch (...) {
            deallocate(new_data);
            throw;
        }

        destroyArrayElements();
        deallocate(data_);
        data_ = new_data;
        size_ = other.size_;
        capacity_ = other.capacity_;

        return *this;
    }

    /// Move assignment operator
    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this != &other) {
            destroyArrayElements();
            deallocate(data_);

            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;

            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }

        return *this;
    }


    /// Returns the current size of the dynamic array.
    [[nodiscard]]
    std::size_t size() const noexcept {
        return size_;
    }

    /// Returns the current capacity of the dynamic array.
    [[nodiscard]]
    std::size_t capacity() const noexcept {
        return capacity_;
    }

    /// Checks if the dynamic array is empty.
    [[nodiscard]]
    bool isEmpty() const noexcept {
        return size_ == 0;
    }


    /**
     * @brief Insert an element at index idx.
     *
     * For middle inserts (idx < size()), this method constructs the result into
     * a fresh buffer and commits only after success (strong guarantee).
     * Appending at idx == size() constructs in-place at the end in O(1)
     * amortized time.
     *
     * @tparam U A type that can construct Type (perfect-forwarded).
     * @param element Value to insert.
     * @param idx Insertion position, 0 <= idx <= size().
     *
     * @par Complexity
     * - Append: amortized O(1); O(n) if growth occurs.
     * - Middle insert: O(n) time; O(n) temporary storage during rebuild.
     *
     * @par Exception Safety
     * - Strong: on failure (allocation or construction), the array is
     * unchanged.
     *
     * @throws std::out_of_range If idx > size().
     * @throws std::length_error If growth is needed and capacity() ==
     * MAX_CAPACITY.
     * @throws std::bad_alloc On allocation failure.
     */
    template <typename U>
    void insert(U&& element, const std::size_t idx) {
        static_assert(std::is_constructible_v<Type, U&&>);
        if (idx > size_)
            throw std::out_of_range("Index out of range");

        ensureCapacity();

        if (idx == size_) {
            new (data_ + size_) Type(std::forward<U>(element));
            ++size_;
            return;
        }

        rebuildBuffer(idx, std::forward<U>(element));
    }


    /**
     * @brief Append an element to the end.
     *
     * Delegates to insert(element, size()). This is typically O(1) amortized,
     * and O(n) when a growth reallocation occurs.
     *
     * @tparam U A type that can construct Type.
     * @param element Value to append.
     *
     * @par Invalidation
     * - On growth (reallocation), all pointers/references/iterators are
     * invalidated.
     */
    template <typename U>
    void addLast(U&& element) {
        insert(std::forward<U>(element), size_);
    }


    /**
     * @brief Prepend an element at index 0.
     *
     * Delegates to insert(element, 0). This is an O(n) middle insert that
     * rebuilds into a fresh buffer and commits on success (strong guarantee).
     *
     * @tparam U A type that can construct Type.
     * @param element Value to insert at the front.
     */
    template <typename U>
    void addFirst(U&& element) {
        insert(std::forward<U>(element), 0);
    }


    /**
     * @brief Remove and return the element at index idx.
     *
     * Removes one element and left-shifts the suffix. If Type is
     * move/copy-assignable, the fast assignment path is used (basic guarantee
     * if an assignment throws). Otherwise, the relocation path is used, which
     * constructs and destroys in sequence and rolls back on construction
     * failure (stronger behavior during the shift).
     *
     * @param idx Index to remove (must satisfy 0 <= idx < size()).
     * @return The removed element (moved out).
     *
     * @par Complexity
     * - O(n) due to shifting.
     *
     * @par Exception Safety
     * - Basic for the assignable path (if an assignment throws).
     * - Strong during relocation path (rollback of partial progress).
     *
     * @throws std::out_of_range If idx >= size().
     */
    Type removeAt(const std::size_t idx) {
        if (idx >= size_)
            throw std::out_of_range("Index out of range");

        Type element = std::move(data_[idx]);

        if (idx == size_ - 1) {
            data_[idx].~Type();
            --size_;
            shrinkIfNecessary();
            return element;
        }

        if constexpr (std::is_move_assignable_v<Type> ||
                      std::is_copy_assignable_v<Type>)
            shiftLeftAssignables(idx);
        else
            shiftLeftNonassignables(idx);

        shrinkIfNecessary();
        return element;
    }


    /**
     * @brief Remove and return the first element.
     *
     * Equivalent to removeAt(0). Shifts all remaining elements one position to
     * the left. Its time complexity is O(size()) each time.
     *
     * @return The removed element (moved out).
     * @throws std::out_of_range If the array is empty.
     */
    Type removeFirst() { return removeAt(0); }


    /**
     * @brief Remove and return the last element.
     *
     * Equivalent to removeAt(size()-1). This is O(1) for the erase itself and
     * may trigger a shrink (which is O(size()) due to reallocation).
     *
     * @return The removed element (moved out).
     * @throws std::out_of_range If the array is empty.
     */
    Type removeLast() { return removeAt(size_ - 1); }


    /**
     * @brief Destroy all elements (capacity unchanged).
     *
     * Ends the lifetime of each constructed element and sets size() to zero.
     * The underlying storage is retained so future appends can avoid
     * reallocation.
     *
     * @par Complexity
     * - O(size()) destructor calls.
     */
    void removeAll() noexcept {
        destroyArrayElements();
        size_ = 0;
    }

    /**
     * @brief Bounds-checked mutable access.
     *
     * Returns a reference to the element at index idx. Throws if idx >= size().
     *
     * @param idx Index in [0, size()).
     * @return Mutable reference to the element at idx.
     */
    Type& get(const std::size_t idx) {
        if (idx >= size_)
            throw std::out_of_range("Index out of range");

        return data_[idx];
    }


    /**
     * @brief Bounds-checked const access.
     *
     * Returns a const reference to the element at index idx. Throws if idx >=
     * size().
     *
     * @param idx Index in [0, size()).
     * @return Const reference to the element at idx.
     */
    const Type& get(const std::size_t idx) const {
        if (idx >= size_)
            throw std::out_of_range("Index out of range");

        return data_[idx];
    }


    /**
     * @brief Element access with bounds checking (mutable).
     *
     * Delegates to get(idx) to enforce bounds checking.
     *
     * @param idx Index in [0, size()).
     * @return Mutable reference to the element at idx.
     */
    Type& operator[](const std::size_t idx) { return get(idx); }


    /**
     * @brief Element access with bounds checking (const).
     *
     * Delegates to get(idx) const to enforce bounds checking.
     *
     * @param idx Index in [0, size()).
     * @return Const reference to the element at idx.
     */
    const Type& operator[](const std::size_t idx) const { return get(idx); }


    /**
     * @brief First element (mutable).
     *
     * @return Reference to the first element.
     * @throws std::out_of_range If the array is empty.
     */
    Type& getFirst() {
        if (isEmpty())
            throw std::out_of_range("Array is empty");

        return data_[0];
    }


    /**
     * @brief First element (const).
     *
     * @return Const reference to the first element.
     * @throws std::out_of_range If the array is empty.
     */
    const Type& getFirst() const {
        if (isEmpty())
            throw std::out_of_range("Array is empty");

        return data_[0];
    }


    /**
     * @brief Last element (mutable).
     *
     * @return Reference to the last element.
     * @throws std::out_of_range If the array is empty.
     */
    Type& getLast() {
        if (isEmpty())
            throw std::out_of_range("Array is empty");

        return data_[size_ - 1];
    }


    /**
     * @brief Last element (const).
     *
     * @return Reference to the last element.
     * @throws std::out_of_range If the array is empty.
     */
    const Type& getLast() const {
        if (isEmpty())
            throw std::out_of_range("Array is empty");

        return data_[size_ - 1];
    }


    /**
     * @brief Emplace-construct an element at index idx.
     *
     * For middle inserts (idx < size()), constructs the result into a fresh
     * buffer (prefix, new element, suffix) and commits only after success
     * (strong guarantee). Appending at idx == size() constructs in-place at the
     * end.
     *
     * @tparam Args Argument types forwarded to Type's constructor.
     * @param idx Insertion position, 0 <= idx <= size().
     * @param args Constructor arguments for the new element.
     *
     * @par Complexity
     * - Append: amortized O(1); O(n) when growth occurs.
     * - Middle insert: O(n) time; O(n) temporary storage during rebuild.
     *
     * @par Exception Safety
     * - Strong: on failure (allocation or construction), the array is
     * unchanged.
     *
     * @throws std::out_of_range If idx > size().
     * @throws std::length_error If growth is needed and capacity() ==
     * MAX_CAPACITY.
     * @throws std::bad_alloc On allocation failure.
     */
    template <typename... Args>
    void emplaceAt(const std::size_t idx, Args&&... args) {
        static_assert(std::is_constructible_v<Type, Args&&...>);
        if (idx > size_)
            throw std::out_of_range("Index out of range");

        ensureCapacity();

        if (idx == size_) {
            new (data_ + size_) Type(std::forward<Args>(args)...);
            ++size_;
            return;
        }

        rebuildBuffer(idx, std::forward<Args>(args)...);
    }


    /**
     * @brief Emplace-construct an element at the end (append).
     *
     * Delegates to emplaceAt(size(), args...).
     *
     * @tparam Args Argument types forwarded to Type's constructor.
     * @param args Constructor arguments.
     *
     * @par Complexity
     * - Amortized O(1); O(n) when growth occurs.
     */
    template <typename... Args>
    void emplaceLast(Args&&... args) {
        emplaceAt(size_, std::forward<Args>(args)...);
    }


    /**
     * @brief Emplace-construct an element at the front (index 0).
     *
     * Delegates to emplaceAt(0, args...). This is an O(n) middle insert that
     * rebuilds into a fresh buffer and commits on success (strong guarantee).
     *
     * @tparam Args Argument types forwarded to Type's constructor.
     * @param args Constructor arguments.
     */
    template <typename... Args>
    void emplaceFirst(Args&&... args) {
        emplaceAt(0, std::forward<Args>(args)...);
    }


    /**
     * @brief Ensure capacity() >= new_capacity (size unchanged).
     *
     * If new_capacity > capacity(), reallocate to exactly new_capacity and
     * move-/copy-construct the existing elements into the new buffer.
     *
     * @param new_capacity Desired minimum capacity in elements.
     *
     * @par Complexity
     * - O(size()) when a resize occurs; otherwise O(1).
     *
     * @par Exception Safety
     * - Strong (via resize).
     *
     * @throws std::bad_alloc If new_capacity > MAX_CAPACITY or allocation
     * fails.
     */
    void reserve(const std::size_t new_capacity) {
        if (new_capacity > capacity_)
            resize(new_capacity);
    }


    /**
     * @brief Shrink capacity to fit current size (not below DEFAULT_CAPACITY).
     *
     * If size() < DEFAULT_CAPACITY, capacity is set to DEFAULT_CAPACITY.
     * Otherwise capacity is set to size(). Elements are moved/copied into the
     * new buffer.
     *
     * @par Complexity
     * - O(size()) when a resize occurs; otherwise O(1).
     *
     * @par Exception Safety
     * - Strong (via resize).
     */
    void shrinkToFit() {
        if (capacity_ > size_) {
            if (size_ < DEFAULT_CAPACITY)
                resize(DEFAULT_CAPACITY);
            else
                resize(size_);
        }
    }


    /**
     * @brief Create a deep copy with independent storage.
     *
     * Returns a new DynamicArray containing copies of all elements. Subsequent
     * modifications to either array do not affect the other.
     *
     * @return A new array equal to *this.
     *
     * @par Complexity
     * - O(size()).
     *
     * @par Exception Safety
     * - Strong; exceptions from allocation and Type's copy constructor
     * propagate.
     */
    DynamicArray clone() const {
        DynamicArray copy;
        copy.reserve(size_);
        copyConstructElements(data_, data_ + size_, copy.data_);
        copy.size_ = size_;
        return copy;
    }


    // --- Iterator support for range-based for loops ---

    Type* begin() noexcept { return data_; }
    const Type* begin() const noexcept { return data_; }

    Type* end() noexcept { return data_ ? data_ + size_ : data_; }
    const Type* end() const noexcept { return data_ ? data_ + size_ : data_; }

    // --- C++11 range-based for loop support ---

    const Type* cbegin() const noexcept { return data_; }
    const Type* cend() const noexcept { return data_ ? data_ + size_ : data_; }

    // Note: All insert/remove/resize operations invalidate pointers/iterators.
    // A moved-from array has begin()==end() and yields zero elements.


    /**
     * @brief Destroy all elements and reset to an empty array with
     * DEFAULT_CAPACITY.
     *
     * Destroys all constructed elements, deallocates the current buffer, then
     * allocates a fresh buffer of DEFAULT_CAPACITY. After clear(), the array is
     * empty and ready to be reused.
     *
     * @par Postconditions
     * - size() == 0
     * - capacity() == DEFAULT_CAPACITY
     *
     * @par Complexity
     * - O(previous size) destructor calls, plus one allocation.
     *
     * @par Exception Safety
     * - Strong: if the re-allocation fails, the previous array remains
     * unchanged (elements are not destroyed).
     */
    void clear() {
        Type* new_data = allocate(DEFAULT_CAPACITY);

        destroyArrayElements();
        deallocate(data_);

        data_ = new_data;
        size_ = 0;
        capacity_ = DEFAULT_CAPACITY;
    }


    /**
     * @brief Destructor: destroys elements and deallocates storage.
     *
     * Destroys all constructed elements, then deallocates the raw storage using
     * the aligned delete counterpart. Safe to call on an already-moved-from
     * object.
     *
     * @par Exception Safety
     * - No-throw (assuming Type's destructor is noexcept; if not, exceptions
     * propagate during destruction).
     */
    ~DynamicArray() noexcept {
        if (data_) {
            destroyArrayElements();
            deallocate(data_);
        }
    }
};


#endif // DYNAMICARRAY_HPP
