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


    /// Helper method to get the actual index in the circular buffer
    std::size_t getCircularIndex(std::size_t logical_index) const {
        return (front_idx_ + logical_index) % array_.capacity();
    }

    /// Helper method to get the back index where next element will be inserted
    std::size_t getBackIndex() const {
        return (front_idx_ + size_) % array_.capacity();
    }


  public:
    /// Default constructor
    Queue() : array_(), front_idx_(0), size_(0) {}

    // Copy constructor
    Queue(const Queue& other) : array_(), front_idx_(0), size_(0) {
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
        other.front_idx_ = 0;
        other.size_ = 0;
    }

    /// Assignment operator
    Queue& operator=(const Queue& other) {
        if (this == &other)
            return *this;

        array_.clear();
        front_idx_ = 0;
        size_ = 0;

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
    std::size_t size() const noexcept { return size_; }

    /// Returns the capacity of the underlying data storage.
    std::size_t capacity() const noexcept { return array_.capacity(); }

    /// Checks if the queue is empty.
    bool isEmpty() const noexcept { return size_ == 0; }


    /**
     * @brief Adds an element to the back of the queue.
     *
     * If the queue is full, it resizes the underlying array to accommodate
     * more elements.
     *
     * @param element The element to be added to the queue.
     * @complexity O(1) amortized for adding an element, O(n) for resizing.
     */
    void enqueue(const Type& element) {
        // Check if we need to resize
        if (size_ == array_.capacity()) {
            DynamicArray<Type> new_array;

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
                array_[back_idx] = element;
            size_++;
        }
    }


    /**
     * @brief Removes and returns the front element of the queue.
     *
     * If the queue is empty, it throws an out_of_range exception.
     *
     * @return The front element of the queue.
     * @throws std::out_of_range if the queue is empty.
     * @complexity O(1) for removing an element.
     */
    Type dequeue() {
        if (isEmpty())
            throw std::out_of_range("Queue is empty");

        Type element = array_[front_idx_];
        front_idx_ = (front_idx_ + 1) % array_.capacity();
        size_--;

        return element;
    }


    /**
     * @brief Returns the front element of the queue without removing it.
     *
     * If the queue is empty, it throws an out_of_range exception.
     *
     * @return A reference to the front element of the queue.
     * @throws std::out_of_range if the queue is empty.
     * @complexity O(1) for accessing the front element.
     */
    Type& front() {
        if (isEmpty())
            throw std::out_of_range("Queue is empty");

        return array_[front_idx_];
    }


    /**
     * @brief Returns the front element of the queue without removing it.
     *
     * If the queue is empty, it throws an out_of_range exception.
     *
     * @return A const reference to the front element of the queue.
     * @throws std::out_of_range if the queue is empty.
     * @complexity O(1) for accessing the front element.
     */
    const Type& front() const {
        if (isEmpty())
            throw std::out_of_range("Queue is empty");

        return array_[front_idx_];
    }


    /**
     * @brief Returns the back element of the queue without removing it.
     *
     * If the queue is empty, it throws an out_of_range exception.
     *
     * @return A reference to the back element of the queue.
     * @throws std::out_of_range if the queue is empty.
     * @complexity O(1) for accessing the back element.
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
     * If the queue is empty, it throws an out_of_range exception.
     *
     * @return A const reference to the back element of the queue.
     * @throws std::out_of_range if the queue is empty.
     * @complexity O(1) for accessing the back element.
     */
    const Type& back() const {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty");
        }
        std::size_t back_idx = (front_idx_ + size_ - 1) % array_.capacity();
        return array_[back_idx];
    }


    /// Clears the queue, removing all elements.
    void clear() {
        array_.clear();
        front_idx_ = 0;
        size_ = 0;
    }

    /// Destructor
    ~Queue() { clear(); }
};

#endif // QUEUE_HPP
