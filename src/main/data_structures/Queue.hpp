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

  public:
    Queue() : array_() {};

    Queue(const Queue& other) : array_(other.array_) {}

    Queue(Queue&& other) noexcept : array_(std::move(other.array_)) {}

    Queue& operator=(const Queue& other) {
        if (this == &other)
            return *this;
        array_ = other.array_;
        return *this;
    }

    Queue& operator=(Queue&& other) noexcept {
        if (this == &other)
            return *this;
        array_ = std::move(other.array_);
        return *this;
    }


    /// Checks if the queue is empty.
    bool isEmpty() const { return array_.isEmpty(); }

    /// Returns the number of elements in the queue.
    std::size_t size() const { return array_.getSize(); }

    /// Returns the current capacity of the queue.
    std::size_t capacity() const { return array_.getCapacity(); }

    /// Clears all elements from the queue.
    void clear() { array_.clear(); }


    /**
     * Adds an element to the end of the queue.
     *
     * @param element The element to be added to the queue.
     */
    void enqueue(const Type& element) { array_.addLast(element); }


    /**
     * Removes and returns the first element of the queue.
     *
     * @return The first element of the queue.
     * @throws std::out_of_range If the queue is empty.
     */
    Type dequeue() {
        if (array_.isEmpty())
            throw std::out_of_range("Queue is empty");
        return array_.removeFirst();
    }


    /**
     * Retrieves the first element of the queue without removing it.
     *
     * @return A reference to the first element of the queue.
     * @throws std::out_of_range If the queue is empty.
     */
    Type& front() {
        if (array_.isEmpty())
            throw std::out_of_range("Queue is empty");
        return array_.getFirst();
    }


    /**
     * Retrieves the first element of the queue without removing it.
     *
     * @return A constant reference to the first element of the queue.
     * @throws std::out_of_range If the queue is empty.
     */
    const Type& front() const {
        if (array_.isEmpty())
            throw std::out_of_range("Queue is empty");
        return array_.getFirst();
    }


    /**
     * Retrieves the last element of the queue without removing it.
     *
     * @return A reference to the last element of the queue.
     * @throws std::out_of_range If the queue is empty.
     */
    Type& back() {
        if (array_.isEmpty())
            throw std::out_of_range("Queue is empty");
        return array_.getLast();
    }


    /**
     * Retrieves the last element of the queue without removing it.
     *
     * @return A constant reference to the last element of the queue.
     * @throws std::out_of_range If the queue is empty.
     */
    const Type& back() const {
        if (array_.isEmpty())
            throw std::out_of_range("Queue is empty");
        return array_.getLast();
    }


    /**
     * Prints all elements in the queue from front to back.
     */
    void print() const {
        if (array_.isEmpty()) {
            std::cout << "Queue is empty" << std::endl;
            return;
        }

        std::cout << "Queue (front to back): ";
        for (std::size_t i = 0; i < array_.getSize(); ++i)
            std::cout << array_[i] << " ";

        std::cout << std::endl;
    }


    ~Queue() = default;
};

#endif // QUEUE_HPP
