#ifndef RECORD_HPP
#define RECORD_HPP

struct Record {
    int& value;

    explicit Record(int& v) : value(v) {}
    Record(const Record& other) : value(other.value) {}

    bool operator<(const Record& other) const { return value < other.value; }
    bool operator>(const Record& other) const { return value > other.value; }
    bool operator==(const Record& other) const { return value == other.value; }
    bool operator<=(const Record& other) const { return value <= other.value; }
    bool operator>=(const Record& other) const { return value >= other.value; }


    Record& operator=(const Record& other) {
        if (this != &other)
            value = other.value;
        return *this;
    }

    Record& operator=(Record&& other) noexcept {
        if (this != &other)
            value = other.value;
        return *this;
    }
};

#endif // RECORD_HPP
