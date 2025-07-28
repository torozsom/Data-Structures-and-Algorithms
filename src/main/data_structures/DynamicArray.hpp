#ifndef DYNAMICARRAY_HPP
#define DYNAMICARRAY_HPP


#include <algorithm>
#include <climits>
#include <iostream>
#include <memory>
#include <stdexcept>


/**
 * @class DynamicArray
 *
 * A generic dynamic array class that provides functionalities for adding,
 * inserting, removing, and accessing elements. The internal storage is
 * dynamically resized as needed to accommodate changes in size.
 */
template <typename Type>
class DynamicArray {

  private:
    Type* data_;
    std::size_t size_;
    std::size_t capacity_;
    static constexpr std::size_t DEFAULT_CAPACITY = 5;


    /// Destroys all elements in the dynamic array by calling their destructors.
    void destroyElements() {
        for (std::size_t i = 0; i < size_; ++i)
            data_[i].~Type();
    }


    /**
     * Resizes the dynamic array to the specified capacity. If the new capacity
     * is less than the default capacity, it is set to the default capacity
     * instead. Copies existing data to the newly allocated memory and updates
     * the capacity.
     *
     * @param new_capacity The new desired capacity for the dynamic array.
     */
    void resize(std::size_t new_capacity) {
        if (new_capacity == capacity_)
            return;

        if (new_capacity < DEFAULT_CAPACITY)
            new_capacity = DEFAULT_CAPACITY;

        if (new_capacity < size_)
            throw std::invalid_argument("New capacity is too small");

        if (new_capacity > SIZE_MAX / sizeof(Type))
            throw std::bad_alloc();

        Type* new_data =
            static_cast<Type*>(::operator new(sizeof(Type) * new_capacity));
        try {
            std::uninitialized_copy(data_, data_ + size_, new_data);
        } catch (...) {
            ::operator delete(new_data);
            throw;
        }

        destroyElements();
        ::operator delete(data_);
        data_ = new_data;
        capacity_ = new_capacity;
    }


  public:
    /// Default constructor
    DynamicArray() : data_(nullptr), size_(0), capacity_(DEFAULT_CAPACITY) {
        data_ = static_cast<Type*>(::operator new(sizeof(Type) * capacity_));
    }

    /// Constructor with initial size
    DynamicArray(const Type* initial_data, const std::size_t initial_size)
        : data_(nullptr), size_(initial_size),
          capacity_(initial_size < DEFAULT_CAPACITY ? DEFAULT_CAPACITY
                                                    : initial_size) {
        if (initial_size > 0 && initial_data == nullptr)
            throw std::invalid_argument("Initial data cannot be null if "
                                        "initial size is greater than zero");

        data_ = static_cast<Type*>(::operator new(sizeof(Type) * capacity_));
        try {
            for (std::size_t i = 0; i < size_; ++i)
                new (data_ + i) Type(initial_data[i]);
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
            std::uninitialized_copy(other.data_, other.data_ + size_, data_);
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
            std::uninitialized_copy(other.data_, other.data_ + other.size_,
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
        if (this == &other)
            return *this;

        ::operator delete(data_);
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;

        return *this;
    }


    std::size_t size() const noexcept { return size_; }

    std::size_t capacity() const noexcept { return capacity_; }

    bool isEmpty() const noexcept { return size_ == 0; }


    /**
     * Adds an element to the end of the dynamic array. If the array is at full
     * capacity, it resizes to accommodate the new element.
     *
     * @param element The element to be added to the array.
     */
    void addLast(const Type& element) { insert(size_, element); }


    /**
     * Adds an element to the beginning of the dynamic array. All existing
     * elements are shifted one position to the right to make space for the new
     * element. If the array is at full capacity, it resizes to accommodate the
     * new element.
     *
     * @param element The element to be added at the beginning of the array.
     */
    void addFirst(const Type& element) { insert(0, element); }


    /**
     * Inserts an element at the specified index in the dynamic array.
     * Shifts all subsequent elements one position to the right. If the array
     * is at full capacity, it resizes to accommodate the new element.
     *
     * @param idx The position at which the element is to be inserted. Must be
     * within the range [0, size_].
     * @param element The element to be inserted into the array.
     * @throws std::out_of_range If the specified index is greater than the size
     * of the array.
     */
    void insert(const std::size_t idx, const Type& element) {
        if (idx > size_)
            throw std::out_of_range("Index out of range");

        if (size_ == capacity_)
            resize(capacity_ > SIZE_MAX / 2 ? SIZE_MAX : capacity_ * 2);

        for (std::size_t i = size_; i > idx; --i) {
            new (data_ + i) Type(std::move(data_[i - 1]));
            data_[i - 1].~Type();
        }

        new (data_ + idx) Type(element);
        ++size_;
    }


    /**
     * Removes an element at the specified index from the dynamic array.
     * Shifts all subsequent elements one position to the left. Reduces the
     * size of the array. If the size falls below one-fourth of the capacity,
     * the array is resized to half of its current capacity.
     *
     * @param idx The position of the element to be removed. Must be within the
     * range [0, size_ - 1].
     * @throws std::out_of_range If the specified index is greater than or equal
     * to the size of the array.
     * @return The removed element
     */
    Type removeAt(const std::size_t idx) {
        if (idx >= size_)
            throw std::out_of_range("Index out of range");

        Type element = data_[idx];

        std::copy(data_ + idx + 1, data_ + size_, data_ + idx);
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
     * @return The removed first element.
     */
    Type removeFirst() { return removeAt(0); }


    /**
     * Removes the last element from the dynamic array. Reduces the size of the
     * array. If the size falls below one-fourth of the capacity, the array is
     * resized to half of its current capacity.
     *
     * @return The removed last element.
     */
    Type removeLast() { return removeAt(size_ - 1); }


    /**
     * Retrieves the element at the specified index in the dynamic array.
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
     * Clears the contents of the dynamic array, deallocating the current
     * memory. Resets the array to its default capacity, size to zero, and
     * reinitializes the underlying data.
     */
    void clear() {
        destroyElements();
        ::operator delete(data_);

        data_ =
            static_cast<Type*>(::operator new(sizeof(Type) * DEFAULT_CAPACITY));
        size_ = 0;
        capacity_ = DEFAULT_CAPACITY;
    }


    /// Destructor
    ~DynamicArray() noexcept {
        if (data_) {
            destroyElements();
            ::operator delete(data_);
        }
    }
};


#endif // DYNAMICARRAY_HPP
