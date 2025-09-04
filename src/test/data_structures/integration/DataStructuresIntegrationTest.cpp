#include <gtest/gtest.h>

#include "DynamicArray.hpp"
#include "LinkedList.hpp"
#include "Queue.hpp"
#include "Stack.hpp"
#include "BinarySearchTree.hpp"
#include "HashMap.hpp"
#include "MaxHeap.hpp"
#include "MinHeap.hpp"

#include <string>
#include <functional>
#include <utility>


using namespace containers;


TEST(DataStructuresIntegrationTest, LinkedListFeedsBinarySearchTree) {
    LinkedList<int> list;
    list.addLast(5);
    list.addLast(3);
    list.addLast(7);
    list.addLast(1);
    list.addLast(4);
    list.addLast(6);
    list.addLast(8);

    BinarySearchTree<int> tree;
    for (int value : list)
        tree.insert(value);

    EXPECT_EQ(tree.size(), list.size());
    EXPECT_TRUE(tree.isValidBST());
    for (int value : list)
        EXPECT_TRUE(tree.contains(value));
}


TEST(DataStructuresIntegrationTest, QueueToStackRoundTrip) {
    Queue<int> queue;
    for (int i = 1; i <= 5; ++i)
        queue.enqueue(i);

    Stack<int> stack;
    DynamicArray<int> array;

    while (!queue.isEmpty())
        stack.push(queue.dequeue());

    while (!stack.isEmpty())
        array.addLast(stack.pop());

    ASSERT_EQ(array.size(), 5u);
    for (size_t i = 0; i < array.size(); ++i)
        EXPECT_EQ(array.get(i), 5 - static_cast<int>(i));
}


TEST(DataStructuresIntegrationTest, BinarySearchTreeExtractsToLinkedListInOrder) {
    const std::vector values = {50, 30, 70, 20, 40, 60, 80, 10, 35, 45, 55, 65, 75, 85};
    BinarySearchTree<int> tree;
    for (int v : values)
        tree.insert(v);

    LinkedList<int> list;

    std::function<void(const Node<int>*)> inorder = [&](const Node<int>* node) {
        if (!node)
            return;
        inorder(node->left);
        list.addLast(node->data);
        inorder(node->right);
    };

    inorder(tree.getRoot());

    ASSERT_EQ(list.size(), values.size());
    EXPECT_TRUE(tree.isValidBST());
    for (size_t i = 1; i < list.size(); ++i) {
        EXPECT_LE(list.get(i - 1), list.get(i));
    }
}


TEST(DataStructuresIntegrationTest, HashMapQueueStackDynamicArrayWorkflow) {
    std::vector<std::pair<int, std::string>> pairs = {
        {1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}, {5, "five"},
        {6, "six"}, {7, "seven"}, {8, "eight"}, {9, "nine"}, {10, "ten"}
    };

    HashMap<int, std::string> map;
    Queue<int> keys;
    for (const auto& [k, v] : pairs) {
        map.insert(k, v);
        keys.enqueue(k);
    }

    Stack<std::string> values;
    while (!keys.isEmpty()) {
        int key = keys.dequeue();
        ASSERT_TRUE(map.contains(key));
        values.push(map.at(key));
        EXPECT_TRUE(map.remove(key));
    }

    EXPECT_TRUE(map.isEmpty());

    DynamicArray<std::string> array;
    while (!values.isEmpty())
        array.addLast(values.pop());

    ASSERT_EQ(array.size(), pairs.size());
    for (size_t i = 0; i < array.size(); ++i) {
        EXPECT_EQ(array.get(i), pairs[pairs.size() - 1 - i].second);
    }
}


TEST(DataStructuresIntegrationTest, HeapsQueueAndLinkedListRoundTrip) {
    MaxHeap<int> maxHeap;
    const DynamicArray numbers = {5, 3, 17, 10, 84, 19, 6, 22, 9};
    for (int n : numbers)
        maxHeap.insert(n);

    ASSERT_TRUE(maxHeap.isValidHeap());
    EXPECT_EQ(maxHeap.size(), numbers.size());

    Queue<int> queue;
    while (!maxHeap.isEmpty())
        queue.enqueue(maxHeap.extractRoot());

    EXPECT_TRUE(maxHeap.isEmpty());
    EXPECT_EQ(queue.size(), numbers.size());

    MinHeap<int> minHeap;
    while (!queue.isEmpty()) {
        int value = queue.dequeue();
        minHeap.insert(value);
        ASSERT_TRUE(minHeap.isValidHeap());
    }

    LinkedList<int> list;
    while (!minHeap.isEmpty())
        list.addLast(minHeap.extractRoot());

    ASSERT_EQ(list.size(), numbers.size());
    for (size_t i = 1; i < list.size(); ++i)
        EXPECT_LE(list.get(i - 1), list.get(i));
}
