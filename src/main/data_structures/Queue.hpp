#ifndef QUEUE_HPP
#define QUEUE_HPP


#include <iostream>
#include <stdexcept>

#include "DynamicArray.hpp"


/**
 * @class Queue
 *
 * A generic queue data structure that provides functionalities for adding,
 * removing, and accessing elements in a first-in-first-out (FIFO) manner.
 * The queue is implemented using a dynamic array to allow for efficient
 * resizing and management of elements.
 *
 * @tparam Type The type of elements stored in the queue.
 */
template <typename Type>
class Queue {

  private:
    DynamicArray<Type> array_;
    std::size_t front_idx_;
    std::size_t size_;
    std::size_t shrink_check_counter_ = 0;

    static constexpr std::size_t SHRINK_CHECK_INTERVAL = 16;
    static constexpr std::size_t MIN_SHRINK_CAPACITY = 10;
    static constexpr std::size_t SHRINK_THRESHOLD_DIVISOR = 4;


    /**
     * @brief Helper method to get the circular index for a logical index.
     *
     * This method calculates the actual index in the underlying array
     * based on the logical index and the current front index.
     *
     * @param logical_index The logical index of the element in the queue.
     * @return The actual index in the underlying array.
     */
    [[nodiscard]]
    std::size_t getCircularIndex(const std::size_t logical_index) const {
        return (front_idx_ + logical_index) % array_.capacity();
    }


    /**
     * @brief Helper method to get the index of the back element in the queue.
     *
     * This method calculates the index of the back element based on the
     * current front index and size of the queue.
     *
     * @return The index of the back element in the underlying array.
     */
    [[nodiscard]]
    std::size_t getBackIndex() const {
        return (front_idx_ + size_) % array_.capacity();
    }


    /**
     * @brief Automatically manages the capacity of the queue.
     *
     * This method checks if the queue needs to shrink its capacity based on
     * the current size and performs the necessary resizing operations.
     * It is called after each dequeue operation to ensure efficient memory
     * usage.
     */
    void autoManageCapacity() {
        shrink_check_counter_++;

        if (shrink_check_counter_ >= SHRINK_CHECK_INTERVAL) {
            shrink_check_counter_ = 0;

            if (size_ <= array_.capacity() / SHRINK_THRESHOLD_DIVISOR &&
                array_.capacity() > MIN_SHRINK_CAPACITY) {

                DynamicArray<Type> new_array;
                new_array.reserve(std::max(size_, array_.capacity() / 2));

                for (std::size_t i = 0; i < size_; ++i) {
                    std::size_t circular_idx = getCircularIndex(i);
                    new_array.addLast(array_[circular_idx]);
                }

                array_ = std::move(new_array);
                front_idx_ = 0;
            }
        }
    }


  public:
    /// Default constructor
    Queue() : array_(), front_idx_(0), size_(0) {}

    /// Constructor with initial capacity
    explicit Queue(std::size_t initial_capacity)
        : array_(), front_idx_(0), size_(0) {
        array_.reserve(initial_capacity);
    }

    /**
     * Constructor with initial data and size.
     *
     * This constructor initializes the queue with a given array of initial
     * data and its size. It reserves enough space in the underlying dynamic
     * array to hold the initial elements.
     *
     * @param initial_data Pointer to the initial data array.
     * @param initial_size The number of elements in the initial data array.
     */
    Queue(const Type* initial_data, const std::size_t initial_size)
        : array_(initial_data, initial_size), front_idx_(0),
          size_(initial_size) {}

    // Copy constructor
    Queue(const Queue& other) : array_(), front_idx_(0), size_(0) {
        array_.reserve(other.size_);
        for (std::size_t i = 0; i < other.size_; ++i) {
            std::size_t src_idx = other.getCircularIndex(i);
            array_.addLast(other.array_[src_idx]);
        }
        size_ = other.size_;
    }

    /// Move constructor
    Queue(Queue&& other) noexcept
        : array_(std::move(other.array_)), front_idx_(other.front_idx_),
          size_(other.size_) {
        other.array_ = DynamicArray<Type>{};
        other.front_idx_ = 0;
        other.size_ = 0;
    }

    /// Copy assignment operator
    Queue& operator=(const Queue& other) {
        if (this == &other)
            return *this;

        array_.clear();
        front_idx_ = 0;
        size_ = 0;

        array_.reserve(other.size_);

        for (std::size_t i = 0; i < other.size_; ++i) {
            std::size_t src_idx = other.getCircularIndex(i);
            array_.addLast(other.array_[src_idx]);
        }
        size_ = other.size_;

        return *this;
    }

    /// Move assignment operator
    Queue& operator=(Queue&& other) noexcept {
        if (this == &other)
            return *this;

        array_ = std::move(other.array_);
        front_idx_ = other.front_idx_;
        size_ = other.size_;

        other.front_idx_ = 0;
        other.size_ = 0;

        return *this;
    }


    /// Returns the number of elements in the queue.
    [[nodiscard]]
    std::size_t size() const noexcept {
        return size_;
    }

    /// Returns the capacity of the underlying data storage.
    [[nodiscard]]
    std::size_t capacity() const noexcept {
        return array_.capacity();
    }

    /// Checks if the queue is empty.
    [[nodiscard]]
    bool isEmpty() const noexcept {
        return size_ == 0;
    }


    /**
     * @brief Adds an element to the back of the queue.
     *
     * This method adds an element to the back of the queue. If the queue is
     * full, it automatically resizes the underlying dynamic array to
     * accommodate the new element. The element must be of the same type as the
     * queue's type.
     *
     * @complexity O(1) on average, O(n) in the worst case when resizing is
     * needed.
     *
     * @tparam U The type of the element to be added. It must be the same as the
     * queue's type.
     *
     * @param element The element to be added to the queue.
     */
    template <typename U>
    void enqueue(U&& element) {
        static_assert(std::is_constructible_v<Type, U&&>,
                      "Element must be constructible into Type");

        if (size_ == array_.capacity()) {
            std::size_t new_capacity = array_.capacity() > SIZE_MAX / 2
                                           ? SIZE_MAX
                                           : array_.capacity() * 2;

            DynamicArray<Type> new_array;
            new_array.reserve(new_capacity);

            for (std::size_t i = 0; i < size_; ++i) {
                std::size_t circular_idx = getCircularIndex(i);
                new_array.addLast(array_[circular_idx]);
            }

            new_array.addLast(element);
            array_ = std::move(new_array);
            front_idx_ = 0;
            size_++;
        } else {
            std::size_t back_idx = getBackIndex();
            if (back_idx >= array_.size())
                array_.addLast(element);
            else
                array_[back_idx] = std::forward<U>(element);
            size_++;
        }
    }


    /**
     * @brief Removes and returns the front element of the queue.
     *
     * If the queue is empty, it throws an out_of_range exception.
     *
     * @complexity O(1) for removing an element.
     *
     * @return The front element of the queue.
     * @throws std::out_of_range if the queue is empty.
     */
    Type dequeue() {
        if (isEmpty())
            throw std::out_of_range("Queue is empty");

        Type element = array_[front_idx_];
        front_idx_ = (front_idx_ + 1) % array_.capacity();
        size_--;

        autoManageCapacity();
        return element;
    }


    /**
     * @brief Returns the front element of the queue without removing it.
     *
     * @complexity O(1) for accessing the front element.
     *
     * @return A reference to the front element of the queue.
     * @throws std::out_of_range if the queue is empty.
     */
    Type& front() {
        if (isEmpty())
            throw std::out_of_range("Queue is empty");

        return array_[front_idx_];
    }


    /**
     * @brief Returns the front element of the queue without removing it.
     *
     * @complexity O(1) for accessing the front element.
     *
     * @return A const reference to the front element of the queue.
     * @throws std::out_of_range if the queue is empty.

     */
    const Type& front() const {
        if (isEmpty())
            throw std::out_of_range("Queue is empty");

        return array_[front_idx_];
    }


    /**
     * @brief Returns the back element of the queue without removing it.
     *
     * @complexity O(1) for accessing the back element.
     *
     * @return A reference to the back element of the queue.
     * @throws std::out_of_range if the queue is empty.

     */
    Type& back() {
        if (isEmpty())
            throw std::out_of_range("Queue is empty");

        std::size_t back_idx = (front_idx_ + size_ - 1) % array_.capacity();
        return array_[back_idx];
    }


    /**
     * @brief Returns the back element of the queue without removing it.
     *
     * @complexity O(1) for accessing the back element.
     *
     * @return A const reference to the back element of the queue.
     * @throws std::out_of_range if the queue is empty.
     */
    const Type& back() const {
        if (isEmpty())
            throw std::out_of_range("Queue is empty");

        std::size_t back_idx = (front_idx_ + size_ - 1) % array_.capacity();
        return array_[back_idx];
    }


    /**
     * @brief Adds a new element to the back of the queue, constructing it in
     * place.
     *
     * This method forwards the provided arguments to the constructor of Type
     * and constructs the new element at the back of the queue. If the queue is
     * full, it automatically resizes the underlying dynamic array to
     * accommodate the new element.
     *
     * @complexity O(1) on average, O(n) in the worst case when resizing is
     * needed.
     *
     * @tparam Args Types of arguments to be forwarded to the constructor of
     * Type.
     * @param args Arguments to be forwarded to the constructor of Type.
     */
    template <typename... Args>
    void emplaceBack(Args&&... args) {
        if (size_ == array_.capacity()) {
            std::size_t new_capacity = array_.capacity() > SIZE_MAX / 2
                                           ? SIZE_MAX
                                           : array_.capacity() * 2;

            DynamicArray<Type> new_array;
            new_array.reserve(new_capacity);

            for (std::size_t i = 0; i < size_; ++i) {
                std::size_t circular_idx = getCircularIndex(i);
                new_array.addLast(array_[circular_idx]);
            }

            new_array.emplaceLast(std::forward<Args>(args)...);
            array_ = std::move(new_array);
            front_idx_ = 0;
            size_++;
        } else {
            std::size_t back_idx = getBackIndex();
            if (back_idx >= array_.size()) {
                array_.emplaceLast(std::forward<Args>(args)...);
            } else {
                array_[back_idx].~Type();
                new (&array_[back_idx]) Type(std::forward<Args>(args)...);
            }
            size_++;
        }
    }


    /// Clears the queue, removing all elements.
    void clear() {
        array_.clear();
        array_.shrinkToFit();
        front_idx_ = 0;
        size_ = 0;
    }

    /// Destructor
    ~Queue() = default;
};

#endif // QUEUE_HPP
