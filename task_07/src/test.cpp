#include <gtest/gtest.h>

#include <vector>

#include "task7.h"

using namespace std;

TEST(SegmentTreeTest, ProblemExample) {
  vector<int> a = {0, 5, 3, 7, 3, 2};
  SegmentTree st(a);

  EXPECT_EQ(st.query(2, 5), 2);

  st.update(3, 1);

  EXPECT_EQ(st.query(1, 3), 1);

  EXPECT_EQ(st.query(3, 3), 1);

  st.update(5, 10);

  EXPECT_EQ(st.query(4, 5), 3);
}

TEST(SegmentTreeTest, SingleElement) {
  vector<int> a = {0, 42};
  SegmentTree st(a);

  EXPECT_EQ(st.query(1, 1), 42);

  st.update(1, -10);
  EXPECT_EQ(st.query(1, 1), -10);
}

TEST(SegmentTreeTest, NegativeNumbers) {
  vector<int> a = {0, -5, -2, -10, 0, 4};
  SegmentTree st(a);

  EXPECT_EQ(st.query(1, 5), -10);
  EXPECT_EQ(st.query(1, 2), -5);
  EXPECT_EQ(st.query(4, 5), 0);

  st.update(3, 100);
  EXPECT_EQ(st.query(1, 5), -5);
}

TEST(SegmentTreeTest, SingleIndexQueries) {
  vector<int> a = {0, 10, 20, 30, 40, 50};
  SegmentTree st(a);

  EXPECT_EQ(st.query(1, 1), 10);
  EXPECT_EQ(st.query(3, 3), 30);
  EXPECT_EQ(st.query(5, 5), 50);

  st.update(3, 25);
  EXPECT_EQ(st.query(3, 3), 25);
}

TEST(SegmentTreeTest, UpdateToSameValues) {
  vector<int> a = {0, 1, 2, 3, 4, 5};
  SegmentTree st(a);

  EXPECT_EQ(st.query(1, 5), 1);

  st.update(1, 10);
  EXPECT_EQ(st.query(1, 5), 2);

  st.update(2, 10);
  EXPECT_EQ(st.query(1, 5), 3);

  st.update(3, 10);
  st.update(4, 10);
  st.update(5, 10);
  EXPECT_EQ(st.query(1, 5), 10);
}

TEST(SegmentTreeTest, UpdateNotAffectingMin) {
  vector<int> a = {0, 10, 20, 5, 30};
  SegmentTree st(a);

  EXPECT_EQ(st.query(1, 4), 5);

  st.update(2, 50);

  EXPECT_EQ(st.query(1, 4), 5);
}