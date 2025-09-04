#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <new>
#include <random>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <functional>


namespace containers {

using std::size_t;


/** @class DefaultHash
 *
 * @brief A universal hash functor that works with any hashable type.
 *
 * This hash functor provides automatic compile-time dispatch to appropriate
 * hashing strategies based on the key type:
 *
 * - **Integral types** (int, char, bool, etc.): Uses SplitMix64 mixing for excellent distribution
 * - **Pointer types**: Removes alignment bits and applies SplitMix64 mixing
 * - **Enumeration types**: Hashes via underlying integral type
 * - **Standard library types**: Delegates to std::hash<T> with additional mixing
 * - **User-defined types**: Requires std::hash<T> specialization
 *
 * The hash function incorporates an optional random seed to provide protection
 * against hash collision attacks while maintaining deterministic behavior when needed.
 *
 * @tparam Key The type of the key to be hashed.
 * @tparam UseRandomSeed Whether to use a random seed (true) or deterministic seed (false).
 *                       Default is true for security, set to false for reproducible testing.
 *
 * @par Thread Safety:
 * This class is thread-safe. The random seed is initialized once per template
 * instantiation and shared among all instances of the same type.
 *
 * @par Performance:
 * - Zero runtime overhead for type dispatch (compile-time selection)
 * - High-quality mixing prevents clustering for sequential keys
 * - Optimal performance for all supported type categories
 */
template <typename Key, bool UseRandomSeed = true>
struct DefaultHash {
    /**
     * @brief Computes the hash value for the given key.
     *
     * This operator automatically selects the most appropriate hashing strategy
     * for the key type at compile time. The hash value incorporates mixing to
     * ensure good distribution properties.
     *
     * @param key The key to be hashed.
     * @return size_t A well-distributed hash value for the key.
     *
     * @par Complexity: O(1) for all supported types.
     * @par Exception Safety: Strong guarantee - either returns a valid hash or fails to compile.
     */
    [[nodiscard]]
    constexpr size_t operator()(const Key& key) const noexcept {
        return hash_dispatch(key);
    }


private:
    /**
     * @brief Dispatches to the appropriate hash function based on the key type.
     *
     * This method uses compile-time type traits and constexpr if to select
     * the optimal hashing strategy for each type category. The dispatch is
     * resolved at compile time with zero runtime overhead.
     *
     * @param key The key to be hashed.
     * @return size_t The computed hash value.
     */
    [[nodiscard]]
    constexpr size_t hash_dispatch(const Key& key) const noexcept {
        if constexpr (std::is_integral_v<Key>) {
            return hash_integral(static_cast<size_t>(key));
        }
        else if constexpr (std::is_pointer_v<Key>) {
            return hash_pointer(reinterpret_cast<uintptr_t>(key));
        }
        else if constexpr (std::is_enum_v<Key>) {
            return hash_enum(key);
        }
        else if constexpr (has_std_hash_v<Key>) {
            return hash_with_std_hash(key);
        }
        else {
            static_assert(always_false_v<Key>,
                         "No hash function available for this type. "
                         "Please specialize std::hash<YourType> or provide a custom Hash functor. "
                         "See documentation for examples.");
        }
        return 0; // Unreachable
    }


    /// SFINAE-based detection of std::hash availability
    template <typename T>
    class has_std_hash {
        template <typename U>
        static auto test(int) -> decltype(std::hash<U>{}(std::declval<const U&>()), std::true_type{});

        template <typename>
        static std::false_type test(...);

    public:
        static constexpr bool value = decltype(test<T>(0))::value;
    };


    /// Helper variable template for std::hash detection
    template <typename T>
    static constexpr bool has_std_hash_v = has_std_hash<T>::value;

    /// Helper for static_assert in template else branch
    template <typename T>
    static constexpr bool always_false_v = false;


    /**
     * @brief Hashes integral types using SplitMix64 mixing.
     *
     * Applies high-quality bit mixing to prevent clustering that occurs
     * with identity hashing of sequential integers. Uses the SplitMix64
     * finalizer which provides excellent avalanche properties.
     *
     * @param key The integral key cast to size_t.
     * @return size_t Well-distributed hash value.
     */
    [[nodiscard]]
    static constexpr size_t hash_integral(size_t key) noexcept {
        return splitmix64(key ^ seed_);
    }


    /**
     * @brief Hashes pointer types with alignment-aware processing.
     *
     * Removes the lower bits that are typically zero due to memory alignment,
     * then applies SplitMix64 mixing. This approach works well for both
     * heap pointers and stack pointers.
     *
     * @param ptr The pointer address as uintptr_t.
     * @return size_t Well-distributed hash value.
     */
    [[nodiscard]]
    static constexpr size_t hash_pointer(uintptr_t ptr) noexcept {
        // Remove lower 3 bits (assumes 8-byte alignment)
        ptr >>= 3;
        return splitmix64(ptr ^ seed_);
    }


    /**
     * @brief Hashes enumeration types via their underlying integral type.
     *
     * Converts the enum to its underlying type and delegates to integral hashing.
     * This works for both scoped (enum class) and unscoped enums.
     *
     * @tparam Enum The enumeration type.
     * @param e The enumeration value to hash.
     * @return size_t Well-distributed hash value.
     */
    template <typename Enum>
    [[nodiscard]]
    constexpr size_t hash_enum(const Enum& e) const noexcept {
        using underlying_t = std::underlying_type_t<Enum>;
        return hash_integral(static_cast<size_t>(static_cast<underlying_t>(e)));
    }


    /**
     * @brief Hashes types with std::hash specialization.
     *
     * Delegates to std::hash<T> and then applies additional SplitMix64 mixing
     * to ensure consistent quality across different std::hash implementations.
     * This handles std::string, std::vector, and other standard library types.
     *
     * @tparam T The type to hash (must have std::hash specialization).
     * @param value The value to hash.
     * @return size_t Well-distributed hash value.
     */
    template<typename T>
    [[nodiscard]]
    constexpr size_t hash_with_std_hash(const T& value) const noexcept {
        const size_t base_hash = std::hash<T>{}(value);
        return splitmix64(base_hash ^ seed_);
    }


    /**
     * @brief SplitMix64 mixing function for high-quality hash distribution.
     *
     * This is the finalizer from the SplitMix64 algorithm, known for excellent
     * avalanche properties. Each input bit influences approximately half of the
     * output bits, providing strong mixing that eliminates patterns in input data.
     *
     * The constants used are:
     * - 0x9e3779b97f4a7c15: The golden ratio * 2^64, provides good stepping
     * - 0xbf58476d1ce4e5b9, 0x94d049bb133111eb: Large odd multipliers with good bit mixing
     *
     * @param x The input value to mix.
     * @return size_t The mixed hash value with excellent distribution properties.
     */
    [[nodiscard]]
    static constexpr size_t splitmix64(size_t x) noexcept {
        x += 0x9e3779b97f4a7c15ull; // Golden ratio * 2^64
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ull;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebull;
        return x ^ (x >> 31);
    }


    /**
     * @brief Initializes a cryptographically random seed.
     *
     * Uses std::random_device to generate a high-quality random seed.
     * The seed is combined from two 32-bit values to ensure full 64-bit entropy
     * even on platforms where std::random_device returns 32-bit values.
     *
     * @return size_t A random seed value.
     */
    static size_t init_random_seed() {
        std::random_device rd;
        return (static_cast<size_t>(rd()) << 32) ^ static_cast<size_t>(rd());
    }


    /**
     * @brief Returns the seed based on the UseRandomSeed template parameter.
     *
     * When UseRandomSeed is true, generates a random seed for hash collision resistance.
     * When false, uses a fixed seed for deterministic behavior (useful for testing).
     *
     * @return size_t The seed value to use for hashing.
     */
    static constexpr size_t get_seed() {
        if constexpr (UseRandomSeed) {
            return init_random_seed();
        } else {
            // Fixed seed derived from golden ratio for deterministic behavior
            return 0x9e3779b97f4a7c15ull;
        }
    }

    /// Static seed shared by all instances of this hash specialization
    inline static const size_t seed_ = get_seed();
};



/** @class HashMap
 *
 * @brief A simple hash map implementation using open addressing with linear
 * probing.
 *
 * This class provides a hash map that supports insertion, deletion, and lookup
 * operations. It uses open addressing with linear probing for collision
 * resolution. The map automatically resizes when the load factor exceeds a
 * predefined threshold.
 *
 * @tparam Key The type of the keys in the map.
 * @tparam Value The type of the values in the map.
 * @tparam Hash A hash functor for the key type. Defaults to DefaultHash<Key>.
 * @tparam LoadFactorPercent The maximum load factor percentage before resizing.
 * Defaults to 70.
 */
template <typename Key, typename Value, typename Hash = DefaultHash<Key>,
          size_t LoadFactorPercent = 70>
class HashMap {
    static_assert(LoadFactorPercent > 0 && LoadFactorPercent < 100,
                  "LoadFactorPercent must be between 1 and 99.");

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
        alignas(Key) unsigned char key_storage[sizeof(Key)]{};
        alignas(Value) unsigned char value_storage[sizeof(Value)]{};

        State state;

        /// Default constructor initializes the bucket to Empty state.
        Bucket() : state(State::Empty) {}

        /// Returns a pointer to the key stored in the bucket.
        Key* key() { return std::launder(reinterpret_cast<Key*>(key_storage)); }

        /// Returns a const pointer to the key stored in the bucket.
        const Key* key() const {
            return std::launder(reinterpret_cast<const Key*>(key_storage));
        }

        /// Returns a pointer to the value stored in the bucket.
        Value* value() {
            return std::launder(reinterpret_cast<Value*>(value_storage));
        }

        /// Returns a const pointer to the value stored in the bucket.
        const Value* value() const {
            return std::launder(reinterpret_cast<const Value*>(value_storage));
        }


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

    static constexpr float LOAD_FACTOR =
        static_cast<float>(LoadFactorPercent) / 100.0f;
    static constexpr size_t DEFAULT_CAPACITY = 8;


    /// Computes the next capacity (double the current).
    size_t nextCapacity(const size_t current) { return current * 2; }

    /// Rounds up to the next power of two.
    static size_t roundUpToPowerOfTwo(const size_t n) {
        size_t cap = 1;
        while (cap < n)
            cap <<= 1;
        return cap;
    }

    /// Computes the index for a given key.
    size_t indexFor(const Key& key) const {
        return hasher_(key) & (capacity_ - 1);
    }


    /**
     * @brief Rehashes the hash map to a new capacity.
     *
     * This method allocates a new array of buckets with the specified
     * capacity and reinserts all occupied buckets from the old array.
     * It ensures that the load factor remains within acceptable limits.
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
            if (Bucket& bucket = old_buckets[i];
                bucket.state == State::Occupied) {
                size_t idx = hasher_(*bucket.key()) & (capacity_ - 1);
                while (buckets_[idx].state == State::Occupied)
                    idx = (idx + 1) & (capacity_ - 1);
                buckets_[idx].construct(std::move(*bucket.key()),
                                        std::move(*bucket.value()));
                ++size_;
                bucket.destroy();
            }
        }
        delete[] old_buckets;
    }


    /// Ensures the hash map has enough capacity, resizing if necessary.
    void ensureCapacity() {
        if ((size_ + 1) > static_cast<size_t>(capacity_ * LOAD_FACTOR))
            rehash(nextCapacity(capacity_));
    }


  public:
    /// Default constructor initializes the hash map with default capacity.
    HashMap()
        : buckets_(new Bucket[DEFAULT_CAPACITY]), size_(0),
          capacity_(DEFAULT_CAPACITY), hasher_() {}

    /// Constructor with specified initial capacity.
    explicit HashMap(const size_t capacity)
        : buckets_(nullptr), size_(0), capacity_(roundUpToPowerOfTwo(capacity)),
          hasher_() {
        buckets_ = new Bucket[capacity_];
    }

    /// Copy constructor.
    HashMap(const HashMap& other)
        : buckets_(new Bucket[other.capacity_]), size_(0),
          capacity_(other.capacity_), hasher_(other.hasher_) {
        for (size_t i = 0; i < other.capacity_; ++i)
            if (other.buckets_[i].state == State::Occupied)
                insert(*other.buckets_[i].key(), *other.buckets_[i].value());
    }

    /// Move constructor.
    HashMap(HashMap&& other) noexcept
        : buckets_(other.buckets_), size_(other.size_),
          capacity_(other.capacity_), hasher_(std::move(other.hasher_)) {
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
    bool isEmpty() const noexcept {
        return size_ == 0;
    }

    /// Returns the number of key-value pairs in the hash map.
    [[nodiscard]]
    size_t size() const noexcept {
        return size_;
    }

    /// Clears the hash map, removing all key-value pairs.
    void clear() {
        for (size_t i = 0; i < capacity_; ++i)
            buckets_[i].destroy();
        size_ = 0;
    }


    /** * @brief Inserts or updates a key-value pair in the hash map.
     *
     * If the key already exists, its value is updated. If the key does not
     * exist, a new key-value pair is inserted. The method ensures that the hash
     * map has enough capacity before insertion, resizing if necessary.
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
                size_t target =
                    first_tombstone != capacity_ ? first_tombstone : idx;
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
            idx = (idx + 1) & (capacity_ - 1);
        }
    }


    /**
     * @brief Accesses the value associated with the given key.
     *
     * This method returns a reference to the value associated with the
     * specified key. If the key does not exist in the hash map, it throws an
     * std::out_of_range exception.
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
            idx = (idx + 1) & (capacity_ - 1);
        }
    }


    /**
     * @brief Accesses the value associated with the given key (const version).
     *
     * This method returns a const reference to the value associated with the
     * specified key. If the key does not exist in the hash map, it throws an
     * std::out_of_range exception.
     *
     * @param key The key whose associated value is to be accessed.
     * @return const Value& A const reference to the value associated with the
     * key.
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
            idx = (idx + 1) & (capacity_ - 1);
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
            idx = (idx + 1) & (capacity_ - 1);
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
            idx = (idx + 1) & (capacity_ - 1);
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
