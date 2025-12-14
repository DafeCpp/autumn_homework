#include <gtest/gtest.h>
#include "../../lib/src/util.hpp"

TEST(RMQTest, SimpleArray) {
    std::vector<int> data = {1, 3, 2, 7, 9, 11};
    RMQ rmq(data);
    ASSERT_EQ(rmq.Query(0, 2), 1);
    ASSERT_EQ(rmq.Query(1, 3), 2);
    ASSERT_EQ(rmq.Query(3, 5), 7);
    ASSERT_EQ(rmq.Query(2, 4), 2);
}

TEST(RMQTest, SingleElement) {
    std::vector<int> data = {42};
    RMQ rmq(data);
    ASSERT_EQ(rmq.Query(0, 0), 42);
}

TEST(RMQTest, FullRange) {
    std::vector<int> data = {5, 1, 6, 2, 0, 4};
    RMQ rmq(data);
    ASSERT_EQ(rmq.Query(0, 5), 0);
}

TEST(RMQTest, RepeatedElements) {
    std::vector<int> data = {2, 2, 2, 2, 2};
    RMQ rmq(data);
    ASSERT_EQ(rmq.Query(0, 4), 2);
    ASSERT_EQ(rmq.Query(1, 3), 2);
    ASSERT_EQ(rmq.Query(2, 2), 2);
}

TEST(RMQTest, LargeRange) {
    std::vector<int> data(1000);
    for (int i = 0; i < 1000; ++i) data[i] = 1000 - i;
    RMQ rmq(data);
    ASSERT_EQ(rmq.Query(0, 999), 1);
    ASSERT_EQ(rmq.Query(100, 900), 100);
}