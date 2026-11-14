#include <gtest/gtest.h>

#include <vector>

#include "rmq.hpp"

namespace {

TEST(RmqTest, ExampleFromPrompt) {
  std::vector<int> a = {5, 3, 7, 3, 2};
  SegmentTree st(a);

  EXPECT_EQ(st.Query(1, 4), 2);

  st.Update(2, 1);

  EXPECT_EQ(st.Query(0, 2), 1);

  EXPECT_EQ(st.Query(2, 2), 1);

  st.Update(4, 10);

  EXPECT_EQ(st.Query(3, 4), 3);
}

TEST(RmqTest, SingleElement) {
  std::vector<int> a = {42};
  SegmentTree st(a);

  EXPECT_EQ(st.Query(0, 0), 42);

  st.Update(0, 10);
  EXPECT_EQ(st.Query(0, 0), 10);
}

TEST(RmqTest, FullRangeQuery) {
  std::vector<int> a = {10, 5, 8, 2, 9};
  SegmentTree st(a);

  EXPECT_EQ(st.Query(0, 4), 2);

  st.Update(3, 15);
  EXPECT_EQ(st.Query(0, 4), 5);
}

TEST(RmqTest, DecreasingArray) {
  std::vector<int> a = {5, 4, 3, 2, 1};
  SegmentTree st(a);

  EXPECT_EQ(st.Query(0, 4), 1);
  EXPECT_EQ(st.Query(2, 4), 1);
  EXPECT_EQ(st.Query(0, 2), 3);
}

TEST(RmqTest, IncreasingArray) {
  std::vector<int> a = {1, 2, 3, 4, 5};
  SegmentTree st(a);

  EXPECT_EQ(st.Query(0, 4), 1);
  EXPECT_EQ(st.Query(2, 4), 3);
  EXPECT_EQ(st.Query(0, 2), 1);
}

TEST(RmqTest, LargeValues) {
  std::vector<int> a = {1'000'000'000, 1'000'000'000, 1'000'000'000};
  SegmentTree st(a);

  EXPECT_EQ(st.Query(0, 2), 1'000'000'000);
  st.Update(1, -1'000'000'000);
  EXPECT_EQ(st.Query(0, 2), -1'000'000'000);
}

}  // namespace