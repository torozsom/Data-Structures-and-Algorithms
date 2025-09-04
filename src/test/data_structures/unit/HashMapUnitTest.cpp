#include <gtest/gtest.h>
#include <stdexcept>

#include "HashMap.hpp"


using containers::HashMap;


class HashMapUnitTest : public testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};


TEST_F(HashMapUnitTest, DefaultConstructor) {
    const HashMap<int, int> map;
    EXPECT_TRUE(map.isEmpty());
    EXPECT_EQ(map.size(), 0);
}


TEST_F(HashMapUnitTest, InsertAndRetrieve) {
    HashMap<int, int> map;
    map.insert(1, 10);
    map.insert(2, 20);
    EXPECT_EQ(map.at(1), 10);
    EXPECT_EQ(map.at(2), 20);
}


TEST_F(HashMapUnitTest, OverwriteValue) {
    HashMap<int, int> map;
    map.insert(1, 10);
    map.insert(1, 42);
    EXPECT_EQ(map.at(1), 42);
}


TEST_F(HashMapUnitTest, RemoveKey) {
    HashMap<int, int> map;
    map.insert(5, 50);
    EXPECT_TRUE(map.remove(5));
    EXPECT_FALSE(map.contains(5));
    EXPECT_THROW(map.at(5), std::out_of_range);
}


TEST_F(HashMapUnitTest, RehashKeepsValues) {
    HashMap<int, int> map;
    for (int i = 0; i < 100; ++i)
        map.insert(i, i * 2);
    for (int i = 0; i < 100; ++i)
        EXPECT_EQ(map.at(i), i * 2);
}


TEST_F(HashMapUnitTest, CopyConstructor) {
    HashMap<int, int> map;
    for (int i = 0; i < 10; ++i)
        map.insert(i, i);
    HashMap<int, int> copy(map);
    for (int i = 0; i < 10; ++i)
        EXPECT_EQ(copy.at(i), i);
}


TEST_F(HashMapUnitTest, MoveConstructor) {
    HashMap<int, int> map;
    map.insert(1, 1);
    const size_t original_size = map.size();
    HashMap<int, int> moved(std::move(map));
    EXPECT_EQ(moved.size(), original_size);
    EXPECT_TRUE(moved.contains(1));
    EXPECT_TRUE(map.isEmpty());
}
