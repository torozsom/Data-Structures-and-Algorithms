#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <new>
#include <stdexcept>
#include <type_traits>
#include <utility>


namespace containers {

using std::size_t;


/**
 * @struct DefaultHash
 *
 * @brief A default hash functor for integral and pointer types with improved distribution.
 *
 * This functor provides hash functions that work for integral types using mixing
 * functions to reduce clustering, and for pointer types by hashing the address.
 * For other types, it triggers a static assertion failure.
 *
 * @tparam Key The type of the key to be hashed.
 */
template <typename Key>
struct DefaultHash {
    /**
     * @brief Computes the hash value for the given key.
     *
     * @param key The key to be hashed.
     * @return size_t The computed hash value.
     *
     * @throws std::logic_error If the key type is not supported.
     */
    size_t operator()(const Key& key) const {
        if constexpr (std::is_integral_v<Key>)
            return hash_integral(static_cast<size_t>(key));
        else if constexpr (std::is_pointer_v<Key>)
            return hash_pointer(reinterpret_cast<uintptr_t>(key));
        else
            static_assert(std::is_integral_v<Key> || std::is_pointer_v<Key>,
                          "DefaultHash only supports integral or pointer keys.");
        return 0; // Unreachable, but avoids compiler warnings
    }

private:
    /**
     * @brief Hash function for integral types using bit mixing.
     *
     * Uses a simple but effective mixing function to reduce clustering
     * that can occur with identity hashing of sequential integers.
     */
    static size_t hash_integral(size_t key) {
        // MurmurHash3 finalizer - simple but effective mixing
        key ^= key >> 16;
        key *= 0x85ebca6b;
        key ^= key >> 13;
        key *= 0xc2b2ae35;
        key ^= key >> 16;
        return key;
    }

    /**
     * @brief Hash function for pointer types.
     *
     * Pointers often have patterns (alignment, heap structure),
     * so we apply mixing to get better distribution.
     */
    static size_t hash_pointer(uintptr_t ptr) {
        // Remove lower bits that are often zero due to alignment
        ptr >>= 3;  // Assume 8-byte alignment for most allocators
        return hash_integral(ptr);
    }
};


/** @class HashMap
 *
 * @brief A simple hash map implementation using open addressing with linear probing.
 *
 * This class provides a hash map that supports insertion, deletion, and lookup
 * operations. It uses open addressing with linear probing for collision resolution.
 * The map automatically resizes when the load factor exceeds a predefined threshold.
 *
 * @tparam Key The type of the keys in the map.
 * @tparam Value The type of the values in the map.
 * @tparam Hash A hash functor for the key type. Defaults to DefaultHash<Key>.
 */
template <typename Key, typename Value, typename Hash = DefaultHash<Key>>
class HashMap {

    /// State of a bucket in the hash map.
    enum class State : unsigned char { Empty, Occupied, Tombstone };


    /** @struct Bucket
     *
     * @brief Represents a single bucket in the hash map.
     *
     * Each bucket can be in one of three states: Empty, Occupied, or Tombstone.
     * It uses aligned storage to hold the key and value, allowing for proper
     * construction and destruction of non-trivially constructible types.
     */
    struct Bucket {
        alignas(Key)
        unsigned char key_storage[sizeof(Key)]{};

        alignas(Value)
        unsigned char value_storage[sizeof(Value)]{};

        State state;

        /// Default constructor initializes the bucket to Empty state.
        Bucket() : state(State::Empty) {}

        /// Returns a pointer to the key stored in the bucket.
        Key* key() { return std::launder(reinterpret_cast<Key*>(key_storage)); }

        /// Returns a const pointer to the key stored in the bucket.
        const Key* key() const { return std::launder(reinterpret_cast<const Key*>(key_storage)); }

        /// Returns a pointer to the value stored in the bucket.
        Value* value() { return std::launder(reinterpret_cast<Value*>(value_storage)); }

        /// Returns a const pointer to the value stored in the bucket.
        const Value* value() const { return std::launder(reinterpret_cast<const Value*>(value_storage)); }


        /**
         * @brief Constructs a key-value pair in the bucket.
         *
         * This method uses placement new to construct the key and value
         * in the aligned storage. It also sets the bucket state to Occupied.
         *
         * @tparam K The type of the key (can be a reference or rvalue).
         * @tparam V The type of the value (can be a reference or rvalue).
         * @param k The key to be constructed.
         * @param v The value to be constructed.
         */
        template <typename K, typename V>
        void construct(K&& k, V&& v) {
            new (key_storage) Key(std::forward<K>(k));
            new (value_storage) Value(std::forward<V>(v));
            state = State::Occupied;
        }

        /// Destroys the key and value in the bucket if occupied.
        void destroy() {
            if (state == State::Occupied) {
                key()->~Key();
                value()->~Value();
                state = State::Empty;
            }
        }
    };


    Bucket* buckets_;
    size_t size_;
    size_t capacity_;
    Hash hasher_;

    static constexpr float LOAD_FACTOR = 0.7f;
    static constexpr size_t DEFAULT_CAPACITY = 8;


    /// Computes the next capacity (double the current).
    size_t nextCapacity(const size_t current) { return current * 2; }

    /// Computes the index for a given key.
    size_t indexFor(const Key& key) const { return hasher_(key) % capacity_; }


    /** * @brief Rehashes the hash map to a new capacity.
     *
     * This method allocates a new array of buckets with the specified new capacity,
     * re-inserts all occupied buckets from the old array into the new array, and
     * then deallocates the old array. It also resets the size to reflect the number
     * of occupied buckets.
     *
     * @param new_capacity The new capacity for the hash map.
     */
    void rehash(const size_t new_capacity) {
        Bucket* old_buckets = buckets_;
        const size_t old_capacity = capacity_;
        buckets_ = new Bucket[new_capacity];
        capacity_ = new_capacity;
        size_ = 0;

        for (size_t i = 0; i < old_capacity; ++i) {
            if (old_buckets[i].state == State::Occupied) {
                insert(std::move(*old_buckets[i].key()),
                       std::move(*old_buckets[i].value()));
                old_buckets[i].destroy();
            }
        }
        delete[] old_buckets;
    }


    void ensureCapacity() {
        if ((size_ + 1) > static_cast<size_t>(capacity_ * LOAD_FACTOR))
            rehash(nextCapacity(capacity_));
    }


  public:
    /// Default constructor initializes the hash map with default capacity.
    HashMap()
        : buckets_(new Bucket[DEFAULT_CAPACITY]),
          size_(0),
          capacity_(DEFAULT_CAPACITY),
          hasher_() {}

    /// Constructor with specified initial capacity.
    explicit HashMap(const size_t capacity)
        : buckets_(new Bucket[capacity]),
          size_(0),
          capacity_(capacity),
          hasher_() {}

    /// Copy constructor.
    HashMap(const HashMap& other)
        : buckets_(new Bucket[other.capacity_]),
          size_(0),
          capacity_(other.capacity_),
          hasher_(other.hasher_) {
        for (size_t i = 0; i < other.capacity_; ++i)
            if (other.buckets_[i].state == State::Occupied)
                insert(*other.buckets_[i].key(), *other.buckets_[i].value());
    }

    /// Move constructor.
    HashMap(HashMap&& other) noexcept
        : buckets_(other.buckets_),
          size_(other.size_),
          capacity_(other.capacity_),
          hasher_(std::move(other.hasher_)) {
        other.buckets_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    /// Copy-and-swap assignment operator.
    HashMap& operator=(HashMap other) noexcept {
        swap(other);
        return *this;
    }


    /// Swaps the contents of this hash map with another.
    void swap(HashMap& other) noexcept {
        Bucket* tmp_b = buckets_;
        buckets_ = other.buckets_;
        other.buckets_ = tmp_b;

        size_t tmp_s = size_;
        size_ = other.size_;
        other.size_ = tmp_s;

        size_t tmp_c = capacity_;
        capacity_ = other.capacity_;
        other.capacity_ = tmp_c;

        std::swap(hasher_, other.hasher_);
    }

    /// Checks if the hash map is empty.
    [[nodiscard]]
    bool isEmpty() const noexcept { return size_ == 0; }

    /// Returns the number of key-value pairs in the hash map.
    [[nodiscard]]
    size_t size() const noexcept { return size_; }

    /// Clears the hash map, removing all key-value pairs.
    void clear() {
        for (size_t i = 0; i < capacity_; ++i)
            buckets_[i].destroy();
        size_ = 0;
    }


    /** * @brief Inserts or updates a key-value pair in the hash map.
     *
     * If the key already exists, its value is updated. If the key does not exist,
     * a new key-value pair is inserted. The method ensures that the hash map has
     * enough capacity before insertion, resizing if necessary.
     *
     * @tparam K The type of the key (can be a reference or rvalue).
     * @tparam V The type of the value (can be a reference or rvalue).
     *
     * @param key The key to be inserted or updated.
     * @param value The value to be associated with the key.
     */
    template <typename K, typename V>
    void insert(K&& key, V&& value) {
        ensureCapacity();
        size_t idx = indexFor(key);
        size_t first_tombstone = capacity_;

        while (true) {
            Bucket& bucket = buckets_[idx];
            if (bucket.state == State::Empty) {
                size_t target = first_tombstone != capacity_ ? first_tombstone : idx;
                buckets_[target].construct(std::forward<K>(key),
                                            std::forward<V>(value));
                ++size_;
                return;
            }
            if (bucket.state == State::Tombstone) {
                if (first_tombstone == capacity_)
                    first_tombstone = idx;
            } else if (*bucket.key() == key) {
                bucket.destroy();
                bucket.construct(std::forward<K>(key), std::forward<V>(value));
                return;
            }
            idx = (idx + 1) % capacity_;
        }
    }


    /**
     * @brief Accesses the value associated with the given key.
     *
     * This method returns a reference to the value associated with the specified key.
     * If the key does not exist in the hash map, it throws an std::out_of_range exception.
     *
     * @param key The key whose associated value is to be accessed.
     * @return Value& A reference to the value associated with the key.
     *
     * @throws std::out_of_range If the key is not found in the hash map.
     */
    Value& at(const Key& key) {
        size_t idx = indexFor(key);
        while (true) {
            Bucket& bucket = buckets_[idx];
            if (bucket.state == State::Empty)
                throw std::out_of_range("Key not found");
            if (bucket.state == State::Occupied && *bucket.key() == key)
                return *bucket.value();
            idx = (idx + 1) % capacity_;
        }
    }


    /**
     * @brief Accesses the value associated with the given key (const version).
     *
     * This method returns a const reference to the value associated with the specified key.
     * If the key does not exist in the hash map, it throws an std::out_of_range exception.
     *
     * @param key The key whose associated value is to be accessed.
     * @return const Value& A const reference to the value associated with the key.
     *
     * @throws std::out_of_range If the key is not found in the hash map.
     */
    const Value& at(const Key& key) const {
        size_t idx = indexFor(key);
        while (true) {
            const Bucket& bucket = buckets_[idx];
            if (bucket.state == State::Empty)
                throw std::out_of_range("Key not found");
            if (bucket.state == State::Occupied && *bucket.key() == key)
                return *bucket.value();
            idx = (idx + 1) % capacity_;
        }
    }


    /**
     * @brief Removes the key-value pair associated with the given key.
     *
     * This method searches for the specified key in the hash map and removes
     * the corresponding key-value pair if found. The bucket is marked as a
     * tombstone to indicate that it was previously occupied. If the key is not
     * found, the method returns false.
     *
     * @param key The key to be removed from the hash map.
     * @return true If the key was found and removed.
     * @return false If the key was not found in the hash map.
     */
    bool remove(const Key& key) {
        size_t idx = indexFor(key);
        while (true) {
            Bucket& bucket = buckets_[idx];
            if (bucket.state == State::Empty)
                return false;
            if (bucket.state == State::Occupied && *bucket.key() == key) {
                bucket.destroy();
                bucket.state = State::Tombstone;
                --size_;
                return true;
            }
            idx = (idx + 1) % capacity_;
        }
    }


    /**
     * @brief Checks if the hash map contains the given key.
     *
     * This method searches for the specified key in the hash map and returns
     * true if the key is found, or false otherwise.
     *
     * @param key The key to be checked for existence in the hash map.
     * @return true If the key exists in the hash map.
     * @return false If the key does not exist in the hash map.
     */
    bool contains(const Key& key) const {
        size_t idx = indexFor(key);
        while (true) {
            const Bucket& bucket = buckets_[idx];
            if (bucket.state == State::Empty)
                return false;
            if (bucket.state == State::Occupied && *bucket.key() == key)
                return true;
            idx = (idx + 1) % capacity_;
        }
    }

    /// Destructor cleans up allocated resources.
    ~HashMap() {
        clear();
        delete[] buckets_;
    }
};


} // namespace containers


#endif
