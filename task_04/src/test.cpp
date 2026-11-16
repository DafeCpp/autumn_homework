#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "task4.h"

using namespace std;

TEST(JohnsonTest, Example1_NegativeEdgesNoCycle) {
  int n = 4;
  int m = 5;
  vector<Edge> edges = {{1, 2, 2}, {1, 3, 5}, {3, 2, -4}, {2, 4, 3}, {3, 4, 6}};
  vector<string> expected = {"0 1 5 4", "INF 0 INF 3", "INF -4 0 -1",
                             "INF INF INF 0"};
  EXPECT_EQ(task4(n, m, edges), expected);
}

TEST(JohnsonTest, Example2_NegativeCycle) {
  int n = 3;
  int m = 3;
  vector<Edge> edges = {{1, 2, 1}, {2, 3, -2}, {3, 1, -1}};
  vector<string> expected = {"NEGATIVE CYCLE"};
  EXPECT_EQ(task4(n, m, edges), expected);
}

TEST(JohnsonTest, IsolatedNegativeCycle) {
  int n = 4;
  int m = 3;
  vector<Edge> edges = {{1, 2, 5}, {3, 4, -5}, {4, 3, -5}};
  vector<string> expected = {"NEGATIVE CYCLE"};
  EXPECT_EQ(task4(n, m, edges), expected);
}

TEST(JohnsonTest, NegativeLoop) {
  int n = 2;
  int m = 2;
  vector<Edge> edges = {{1, 2, 10}, {2, 2, -1}};
  vector<string> expected = {"NEGATIVE CYCLE"};
  EXPECT_EQ(task4(n, m, edges), expected);
}

TEST(JohnsonTest, ZeroWeightCycle) {
  int n = 3;
  int m = 3;
  vector<Edge> edges = {{1, 2, 0}, {2, 3, 0}, {3, 1, 0}};
  vector<string> expected = {"0 0 0", "0 0 0", "0 0 0"};
  EXPECT_EQ(task4(n, m, edges), expected);
}

TEST(JohnsonTest, MultipleEdgesAndPositiveLoops) {
  int n = 2;
  int m = 4;
  vector<Edge> edges = {{1, 2, 10}, {1, 2, 3}, {1, 1, 5}, {2, 1, 7}};
  vector<string> expected = {"0 3", "7 0"};
  EXPECT_EQ(task4(n, m, edges), expected);
}

TEST(JohnsonTest, UnreachablePairs) {
  int n = 4;
  int m = 2;
  vector<Edge> edges = {{1, 2, 5}, {3, 4, 10}};
  vector<string> expected = {"0 5 INF INF", "INF 0 INF INF", "INF INF 0 10",
                             "INF INF INF 0"};
  EXPECT_EQ(task4(n, m, edges), expected);
}

TEST(JohnsonTest, SingleVertex) {
  int n = 1;
  int m = 0;
  vector<Edge> edges = {};
  vector<string> expected = {"0"};
  EXPECT_EQ(task4(n, m, edges), expected);
}

TEST(JohnsonTest, SingleVertexWithPositiveLoop) {
  int n = 1;
  int m = 1;
  vector<Edge> edges = {{1, 1, 10}};
  vector<string> expected = {"0"};
  EXPECT_EQ(task4(n, m, edges), expected);
}