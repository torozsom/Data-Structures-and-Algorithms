/**
 * @file DynamicArray.hpp
 * A generic dynamic array class that provides functionalities for adding,
 * inserting, removing, and accessing elements. The internal storage is
 * dynamically resized as needed to accommodate changes in size.
 *
 * @version 1.0
 * @date 2024-12-23
 */


#ifndef DYNAMICARRAY_HPP
#define DYNAMICARRAY_HPP


#include <iostream>


/**
 * @class DynamicArray
 * A generic dynamic array class that provides functionalities for adding,
 * inserting, removing, and accessing elements. The internal storage is
 * dynamically resized as needed to accommodate changes in size.
 */
template<typename Type>
class DynamicArray {

private:
    Type* data_;
    unsigned int size_;
    unsigned int capacity_;
    static constexpr unsigned int DEFAULT_CAPACITY = 5;


    /**
     * Copies a specific number of elements from the source array to the destination array.
     *
     * @param source Pointer to the source array from which elements will be copied.
     * @param destination Pointer to the destination array where elements will be copied to.
     * @param count The number of elements to copy from the source to the destination.
     */
    void copyData(const Type* source, Type* destination, unsigned int count) {
        for (unsigned int i = 0; i < count; ++i)
            destination[i] = source[i];
    }


    /**
     * Resizes the dynamic array to the specified capacity. If the new capacity is less than
     * the default capacity, it is set to the default capacity instead. Copies existing data
     * to the newly allocated memory and updates the capacity.
     *
     * @param new_capacity The new desired capacity for the dynamic array.
     */
    void resize(unsigned int new_capacity) {
        if (new_capacity < DEFAULT_CAPACITY)
            new_capacity = DEFAULT_CAPACITY;

        Type* new_data = nullptr;
        try {
            new_data = new Type[new_capacity];
            copyData(data_, new_data, size_);
        } catch (...) {
            delete[] new_data;
            throw;
        }

        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }



public:
    /// Default constructor
    DynamicArray() : data_(nullptr), size_(0), capacity_(DEFAULT_CAPACITY) {
        data_ = new Type[capacity_];
    }

    /// Constructor with initial size
    DynamicArray(const Type* initial_data, unsigned int initial_size)
        : data_(new Type[initial_size]), size_(initial_size), capacity_(initial_size) {
        copyData(initial_data, data_, initial_size);
    }

    /// Copy constructor
    DynamicArray(const DynamicArray& other)
        : data_(new Type[other.capacity_]), size_(other.size_), capacity_(other.capacity_) {
        copyData(other.data_, data_, size_);
    }

    /// Move constructor
    DynamicArray(DynamicArray&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }


    unsigned int getSize() const { return size_; }

    unsigned int getCapacity() const { return capacity_; }

    bool isEmpty() const { return size_ == 0; }


    /// Copy assignment operator
    DynamicArray& operator=(const DynamicArray& other) {
        if (this == &other)
            return *this;

        delete[] data_;
        data_ = new Type[other.capacity_];
        size_ = other.size_;
        capacity_ = other.capacity_;

        copyData(other.data_, data_, size_);
        return *this;
    }

    /// Move assignment operator
    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this == &other)
            return *this;

        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;

        return *this;
    }


    /**
     * Adds an element to the end of the dynamic array. If the array is at full capacity,
     * it resizes to accommodate the new element.
     *
     * @param element The element to be added to the array.
     */
    void add(const Type& element) {
        if (size_ == capacity_)
            resize(capacity_ * 2);

        data_[size_++] = element;
    }


    /**
     * Inserts an element at the specified index in the dynamic array.
     * Shifts all subsequent elements one position to the right. If the array
     * is at full capacity, it resizes to accommodate the new element.
     *
     * @param index The position at which the element is to be inserted. Must be within the range [0, size_].
     * @param element The element to be inserted into the array.
     * @throws std::out_of_range If the specified index is greater than the size of the array.
     */
    void insert(unsigned int index, const Type& element) {
        if (index > size_)
            throw std::out_of_range("Index out of range");

        if (size_ == capacity_)
            resize(capacity_ * 2);

        for (unsigned int i = size_; i > index; --i)
            data_[i] = data_[i - 1];

        data_[index] = element;
        ++size_;
    }


    /**
     * Removes an element at the specified index from the dynamic array.
     * Shifts all subsequent elements one position to the left. Reduces the
     * size of the array. If the size falls below one-fourth of the capacity,
     * the array is resized to half of its current capacity.
     *
     * @param index The position of the element to be removed. Must be within the range [0, size_ - 1].
     * @throws std::out_of_range If the specified index is greater than or equal to the size of the array.
     */
    void remove(unsigned int index) {
        if (index >= size_)
            throw std::out_of_range("Index out of range");

        for (unsigned int i = index; i < size_ - 1; ++i)
            data_[i] = data_[i + 1];

        --size_;

        if (size_ > 0 && size_ <= capacity_ / 4)
            resize(capacity_ / 2);
    }


    /**
     * Retrieves the element at the specified index in the dynamic array.
     *
     * @param index The position of the element to retrieve. Must be within the range [0, size_ - 1].
     * @return A reference to the element at the specified index in the dynamic array.
     * @throws std::out_of_range If the specified index is greater than or equal to the size of the array.
     */
    Type& get(unsigned int index) const {
        if (index >= size_)
            throw std::out_of_range("Index out of range");

        return data_[index];
    }


    /**
     * Accesses the element at the specified index in the dynamic array.
     *
     * @param index The position of the element to access. Must be within the range [0, size_ - 1].
     * @return A reference to the element at the specified index in the dynamic array.
     * @throws std::out_of_range If the specified index is greater than or equal to the size of the array.
     */
    Type& operator[](unsigned int index) {
        return get(index);
    }


    /**
     * Provides read-only access to the element at the specified index in the dynamic array.
     *
     * @param index The position of the element to access. Must be within the range [0, size_ - 1].
     * @return A constant reference to the element at the specified index in the dynamic array.
     * @throws std::out_of_range If the specified index is greater than or equal to the size of the array.
     */
    const Type& operator[](unsigned int index) const {
        return get(index);
    }


    /**
     * Clears the contents of the dynamic array, deallocating the current memory.
     * Resets the array to its default capacity, size to zero, and reinitializes
     * the underlying data.
     */
    void clear() {
        delete[] data_;
        data_ = new Type[DEFAULT_CAPACITY];
        size_ = 0;
        capacity_ = DEFAULT_CAPACITY;
    }


    /// Destructor
    ~DynamicArray() { delete[] data_; }

};


#endif  // DYNAMICARRAY_HPP
