#ifndef DYNAMICARRAY_HPP
#define DYNAMICARRAY_HPP


#include <algorithm>
#include <climits>
#include <iostream>
#include <stdexcept>
#include <type_traits>


/**
 * @class DynamicArray
 *
 * Dynamic Array is a template class that implements a dynamic array data
 * structure. It allows for automatic resizing, element insertion, and removal.
 * The array can grow or shrink as needed, and it provides methods to access
 * elements, add new elements, and remove existing ones. Memory management is
 * handled using placement new and operator delete to ensure proper construction
 * and destruction of elements. It also supports iterators for easy traversal
 * of the elements in the array (foreach).
 *
 * @tparam Type The type of elements stored in the dynamic array.
 */
template <typename Type>
class DynamicArray {

  private:
    Type* data_;
    std::size_t size_;
    std::size_t capacity_;

    static constexpr std::size_t DEFAULT_CAPACITY = 5;
    static constexpr std::size_t MAX_CAPACITY = SIZE_MAX / sizeof(Type);
    static constexpr std::size_t MAX_SAFE_CAPACITY =
        SIZE_MAX / sizeof(Type) / 2;


    /// Destroys all elements in the dynamic array by calling their destructors.
    void destroyArrayElements() {
        for (std::size_t i = 0; i < size_; ++i)
            data_[i].~Type();
    }


    /**
     * Constructs elements in the destination array by copying or moving from
     * the source array. If an exception occurs during construction, it cleans
     * up any successfully constructed elements in the destination array.
     *
     * @param source_begin Pointer to the beginning of the source array.
     * @param source_end Pointer to the end of the source array.
     * @param destination Pointer to the destination array where elements will
     * be constructed.
     *
     * @return Pointer to the end of the constructed elements in the destination
     * array.
     */
    Type* copy_construct_elements(const Type* source_begin,
                                  const Type* source_end,
                                  Type* destination) const
        noexcept(std::is_nothrow_copy_constructible_v<Type>) {
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
     * Constructs elements in the destination array by moving from the source
     * array. If an exception occurs during construction, it cleans up any
     * successfully constructed elements in the destination array.
     *
     * @param source_begin Pointer to the beginning of the source array.
     * @param source_end Pointer to the end of the source array.
     * @param destination Pointer to the destination array where elements will
     * be constructed.
     *
     * @return Pointer to the end of the constructed elements in the destination
     * array.
     */
    Type* move_construct_elements(Type* source_begin, Type* source_end,
                                  Type* destination) const
        noexcept(std::is_nothrow_move_constructible_v<Type>) {
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
     * @brief Resizes the dynamic array to a new capacity.
     *
     * If the new capacity equals the current capacity, no action is taken.
     * If the new capacity is less than the default capacity, it is set
     * to the default capacity. Then it allocates new memory for the
     * elements, moves or copies the existing elements to the new memory,
     * and destroys the old elements.
     *
     * @param new_capacity The new capacity for the dynamic array.
     *
     * @throws std::invalid_argument If the new capacity is less than the
     * current size of the array.
     * @throws std::bad_alloc If the new capacity exceeds the maximum size that
     * can be allocated.
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

        Type* new_data =
            static_cast<Type*>(::operator new(sizeof(Type) * new_capacity));

        if constexpr (std::is_nothrow_move_constructible_v<Type> ||
                      !std::is_copy_constructible_v<Type>)
            move_construct_elements(data_, data_ + size_, new_data);
        else
            copy_construct_elements(data_, data_ + size_, new_data);

        destroyArrayElements();
        ::operator delete(data_);
        data_ = new_data;
        capacity_ = new_capacity;
    }


  public:
    /**
     * Default constructor that initializes the dynamic array with a default
     * capacity.
     */
    DynamicArray() : data_(nullptr), size_(0), capacity_(DEFAULT_CAPACITY) {
        data_ = static_cast<Type*>(::operator new(sizeof(Type) * capacity_));
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

        data_ = static_cast<Type*>(::operator new(sizeof(Type) * capacity_));
        try {
            copy_construct_elements(initial_data, initial_data + size_, data_);
        } catch (...) {
            ::operator delete(data_);
            throw;
        }
    }

    /// Copy constructor
    DynamicArray(const DynamicArray& other)
        : data_(static_cast<Type*>(
              ::operator new(sizeof(Type) * other.capacity_))),
          size_(other.size_), capacity_(other.capacity_) {
        try {
            copy_construct_elements(other.data_, other.data_ + size_, data_);
        } catch (...) {
            ::operator delete(data_);
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

        Type* new_data =
            static_cast<Type*>(::operator new(sizeof(Type) * other.capacity_));
        try {
            copy_construct_elements(other.data_, other.data_ + other.size_,
                                    new_data);
        } catch (...) {
            ::operator delete(new_data);
            throw;
        }

        ::operator delete(data_);
        data_ = new_data;
        size_ = other.size_;
        capacity_ = other.capacity_;

        return *this;
    }

    /// Move assignment operator
    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this != &other) {
            destroyArrayElements();
            ::operator delete(data_);

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
     * Inserts an element at the specified index in the dynamic array. All
     * existing elements from the specified index onward are shifted one
     * position to the right to make space for the new element. If the array is
     * at full capacity, it resizes to accommodate the new element.
     *
     * @complexity Average and worst case insertion is O(n).
     *
     * @tparam U The type of the element to be inserted. Must be the same as
     * Type or convertible to it.
     *
     * @param element The element to be inserted into the array.
     * @param idx The index at which to insert the new element. Must be within
     * the range [0, size_].
     *
     * @throws std::out_of_range If the specified index is greater than the
     * current size of the array.
     */
    template <typename U>
    void insert(U&& element, const std::size_t idx) {
        static_assert(std::is_constructible_v<Type, U&&>,
                      "Element must be constructible into Type");

        if (idx > size_)
            throw std::out_of_range("Index out of range");

        if (size_ == capacity_)
            resize(capacity_ > MAX_SAFE_CAPACITY ? MAX_CAPACITY
                                                 : capacity_ * 2);

        for (std::size_t i = size_; i > idx; --i) {
            new (data_ + i) Type(std::move(data_[i - 1]));
            data_[i - 1].~Type();
        }

        new (data_ + idx) Type(std::forward<U>(element));
        ++size_;
    }


    /**
     * Adds an element to the end of the dynamic array. If the array is at full
     * capacity, it resizes to accommodate the new element.
     *
     * @complexity O(1) on average, O(n) in the worst case when resizing is
     * needed.
     *
     * @tparam U The type of the element to be added. Must be the same as Type
     * or convertible to it.
     *
     * @param element The element to be added at the end of the array.
     */
    template <typename U>
    void addLast(U&& element) {
        insert(std::forward<U>(element), size_);
    }


    /**
     * Adds an element to the beginning of the dynamic array. All existing
     * elements are shifted one position to the right to make space for the new
     * element. If the array is at full capacity, it resizes to accommodate the
     * new element.
     *
     * @complexity O(n) on average.
     *
     * @tparam U The type of the element to be added. Must be the same as Type
     * or convertible to it.
     *
     * @param element The element to be added at the beginning of the array.
     */
    template <typename U>
    void addFirst(U&& element) {
        insert(std::forward<U>(element), 0);
    }


    /**
     * Removes the element at the specified index from the dynamic array. All
     * subsequent elements are shifted one position to the left. Reduces the
     * size of the array. If the size falls below one-fourth of the capacity,
     * the array is resized to half of its current capacity.
     *
     * @complexity O(n) on average.
     *
     * @param idx The index of the element to be removed. Must be within the
     * range [0, size_ - 1].
     * @return The element that was removed.
     * @throws std::out_of_range If the specified index is greater than or equal
     * to the size of the array.
     */
    Type removeAt(const std::size_t idx) {
        if (idx >= size_)
            throw std::out_of_range("Index out of range");

        Type element = data_[idx];
        data_[idx].~Type();

        copy_construct_elements(data_ + idx + 1, data_ + size_, data_ + idx);
        --size_;

        if (size_ <= capacity_ / 4 && capacity_ > DEFAULT_CAPACITY)
            resize(std::max(DEFAULT_CAPACITY, capacity_ / 2));

        return element;
    }


    /**
     * Removes the first element from the dynamic array. All subsequent
     * elements are shifted one position to the left. Reduces the size of the
     * array. If the size falls below one-fourth of the capacity, the array is
     * resized to half of its current capacity.
     *
     * @complexity O(n) each time.
     *
     * @return The removed first element.
     */
    Type removeFirst() { return removeAt(0); }


    /**
     * Removes the last element from the dynamic array. Reduces the size of the
     * array. If the size falls below one-fourth of the capacity, the array is
     * resized to half of its current capacity.
     *
     * @complexity O(1) on average, O(n) in the worst case when resizing is
     * needed.
     *
     * @return The removed last element.
     */
    Type removeLast() { return removeAt(size_ - 1); }


    /**
     * Retrieves the element at the specified index in the dynamic array.
     *
     * @complexity O(1) for direct access.
     *
     * @param idx The position of the element to retrieve. Must be within the
     * range [0, size_ - 1].
     * @return A reference to the element at the specified index in the dynamic
     * array.
     * @throws std::out_of_range If the specified index is greater than or equal
     * to the size of the array.
     */
    Type& get(const std::size_t idx) {
        if (idx >= size_)
            throw std::out_of_range("Index out of range");

        return data_[idx];
    }


    /**
     * Retrieves the element at the specified index in the dynamic array.
     *
     * @complexity O(1) for direct access.
     *
     * @param idx The position of the element to retrieve. Must be within the
     * range [0, size_ - 1].
     * @return A reference to the element at the specified index in the dynamic
     * array.
     * @throws std::out_of_range If the specified index is greater than or equal
     * to the size of the array.
     */
    const Type& get(const std::size_t idx) const {
        if (idx >= size_)
            throw std::out_of_range("Index out of range");

        return data_[idx];
    }


    /**
     * Accesses the element at the specified index in the dynamic array.
     *
     * @param idx The position of the element to access. Must be within the
     * range [0, size_ - 1].
     * @return A reference to the element at the specified index in the dynamic
     * array.
     * @throws std::out_of_range If the specified index is greater than or equal
     * to the size of the array.
     */
    Type& operator[](const std::size_t idx) { return get(idx); }


    /**
     * Provides read-only access to the element at the specified index in the
     * dynamic array.
     *
     * @param idx The position of the element to access. Must be within the
     * range [0, size_ - 1].
     * @return A constant reference to the element at the specified index in the
     * dynamic array.
     * @throws std::out_of_range If the specified index is greater than or equal
     * to the size of the array.
     */
    const Type& operator[](const std::size_t idx) const { return get(idx); }


    /**
     * Retrieves the first element of the dynamic array.
     *
     * @return The first element of the dynamic array.
     * @throws std::out_of_range If the array is empty.
     */
    Type& getFirst() {
        if (isEmpty())
            throw std::out_of_range("Array is empty");

        return data_[0];
    }


    /**
     * Retrieves the first element of the dynamic array.
     *
     * @return A constant reference to the first element of the dynamic array.
     * @throws std::out_of_range If the array is empty.
     */
    const Type& getFirst() const {
        if (isEmpty())
            throw std::out_of_range("Array is empty");

        return data_[0];
    }


    /**
     * Retrieves the last element of the dynamic array.
     *
     * @return The last element of the dynamic array.
     * @throws std::out_of_range If the array is empty.
     */
    Type& getLast() {
        if (isEmpty())
            throw std::out_of_range("Array is empty");

        return data_[size_ - 1];
    }


    /**
     * Retrieves the last element of the dynamic array.
     *
     * @return A constant reference to the last element of the dynamic array.
     * @throws std::out_of_range If the array is empty.
     */
    const Type& getLast() const {
        if (isEmpty())
            throw std::out_of_range("Array is empty");

        return data_[size_ - 1];
    }


    /**
     * Emplaces a new element at the specified index in the dynamic array,
     * constructing it in place using the provided arguments. All existing
     * elements from the specified index onward are shifted one position to the
     * right to make space for the new element. If the array is at full
     * capacity, it resizes to accommodate the new element.
     *
     * @complexity Average and worst case insertion is O(n).
     *
     * @tparam Args Types of arguments to be forwarded to the constructor of
     * Type.
     *
     * @param idx The index at which to emplace the new element. Must be within
     * the range [0, size_].
     * @param args Arguments to be forwarded to the constructor of Type.
     *
     * @throws std::out_of_range If the specified index is greater than the
     * current size of the array.
     */
    template <typename... Args>
    void emplaceAt(std::size_t idx, Args&&... args) {
        static_assert(std::is_constructible_v<Type, Args&&...>,
                      "Arguments must be constructible into Type");

        if (idx > size_)
            throw std::out_of_range("Index out of range");

        if (size_ == capacity_)
            resize(capacity_ > MAX_SAFE_CAPACITY ? MAX_CAPACITY
                                                 : capacity_ * 2);

        for (std::size_t i = size_; i > idx; --i) {
            new (data_ + i) Type(std::move(data_[i - 1]));
            data_[i - 1].~Type();
        }

        new (data_ + idx) Type(std::forward<Args>(args)...);
        ++size_;
    }


    /**
     * Emplaces a new element at the end of the dynamic array, constructing it
     * in place using the provided arguments. If the array is at full capacity,
     * it resizes to accommodate the new element.
     *
     * @complexity O(1) on average, O(n) in the worst case when resizing is
     * needed.
     *
     * @tparam Args Types of arguments to be forwarded to the constructor of
     * Type.
     * @param args Arguments to be forwarded to the constructor of Type.
     */
    template <typename... Args>
    void emplaceLast(Args&&... args) {
        emplaceAt(size_, std::forward<Args>(args)...);
    }


    /**
     * Emplaces a new element at the beginning of the dynamic array,
     * constructing it in place using the provided arguments. All existing
     * elements are shifted one position to the right to make space for the new
     * element. If the array is at full capacity, it resizes to accommodate the
     * new element.
     *
     * @complexity O(n) each time.
     *
     * @tparam Args Types of arguments to be forwarded to the constructor of
     * Type.
     * @param args Arguments to be forwarded to the constructor of Type.
     */
    template <typename... Args>
    void emplaceFirst(Args&&... args) {
        emplaceAt(0, std::forward<Args>(args)...);
    }


    /**
     * @brief Ensures the capacity of the array is at least the given amount.
     *
     * If the current capacity is less than new_capacity, the array is resized
     * but no new elements are added (size_ remains unchanged).
     *
     * @param new_capacity The desired minimum capacity.
     * @throws std::bad_alloc If allocation fails or the new capacity is
     * invalid.
     */
    void reserve(const std::size_t new_capacity) {
        if (new_capacity > capacity_)
            resize(new_capacity);
    }


    /**
     * @brief Shrinks the capacity of the dynamic array to fit its current size.
     *
     * If the current size is less than the default capacity, it resizes to the
     * default capacity. Otherwise, it resizes to the current size.
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
     * @brief Creates a copy of the dynamic array.
     *
     * This method creates a new DynamicArray instance that is a copy of the
     * current instance, including all elements. The new instance has its own
     * memory allocation for the elements.
     *
     * @return A new DynamicArray instance that is a copy of the current one.
     */
    DynamicArray clone() const {
        DynamicArray copy;
        copy.reserve(size_);
        copy_construct_elements(data_, data_ + size_, copy.data_);
        copy.size_ = size_;
        return copy;
    }


    /// **** Iterators **** ///

    // --- Iterator support for range-based for loops ---

    Type* begin() noexcept { return data_; }
    const Type* begin() const noexcept { return data_; }
    Type* end() noexcept { return data_ + size_; }
    const Type* end() const noexcept { return data_ + size_; }

    // --- C++11 range-based for loop support ---

    const Type* cbegin() const noexcept { return data_; }
    const Type* cend() const noexcept { return data_ + size_; }

    /// **** ///


    /**
     * Clears the dynamic array by destroying all elements and resetting the
     * size and capacity to their initial values. The memory allocated for
     * the array is also released.
     */
    void clear() {
        destroyArrayElements();
        ::operator delete(data_);

        data_ =
            static_cast<Type*>(::operator new(sizeof(Type) * DEFAULT_CAPACITY));
        size_ = 0;
        capacity_ = DEFAULT_CAPACITY;
    }


    /// Destructor that deallocates the memory used by the dynamic array and
    /// destroys all elements.
    ~DynamicArray() noexcept {
        if (data_) {
            destroyArrayElements();
            ::operator delete(data_);
        }
    }
};


#endif // DYNAMICARRAY_HPP
