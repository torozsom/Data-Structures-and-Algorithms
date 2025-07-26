

#ifndef THROWING_TYPE_HPP
#define THROWING_TYPE_HPP


#include <stdexcept>


// Test utility class for testing exception safety and memory management
class ThrowingType {
  public:
    static int construction_count;
    static bool should_throw;

    int value;

    explicit ThrowingType(int v = 0) : value(v) {
        if (should_throw && construction_count >= 2) {
            throw std::runtime_error("Throwing constructor");
        }
        construction_count++;
    }

    ThrowingType(const ThrowingType& other) : value(other.value) {
        if (should_throw && construction_count >= 2) {
            throw std::runtime_error("Throwing copy constructor");
        }
        construction_count++;
    }

    ThrowingType& operator=(const ThrowingType& other) {
        if (this != &other) {
            value = other.value;
        }
        return *this;
    }

    ~ThrowingType() { construction_count--; }

    static void reset() {
        should_throw = false;
        construction_count = 0;
    }
};

// Define static members using inline to avoid multiple definition errors
inline int ThrowingType::construction_count = 0;
inline bool ThrowingType::should_throw = false;

#endif // THROWING_TYPE_HPP
