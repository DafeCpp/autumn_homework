#include <gtest/gtest.h>

#include <sstream>

#include "solution.hpp"

typedef std::vector<std::vector<long long>> Matrix;

TEST(Test, SingleVertex) {
  Graph graph(1, {});
  EXPECT_EQ(*johnson(graph), Matrix({{0}}));
}

TEST(Test, TwoVerticesOneEdge) {
  Graph graph(2, {{0, 1, 5}});
  EXPECT_EQ(*johnson(graph), Matrix({{0, 5}, {INF, 0}}));
}

TEST(Test, NegativeEdge) {
  Graph graph(3, {{0, 1, 4}, {1, 2, -3}, {0, 2, 5}});
  EXPECT_EQ(*johnson(graph), Matrix({{0, 4, 1}, {INF, 0, -3}, {INF, INF, 0}}));
}

TEST(Test, NegativeEdgeInCycle) {
  Graph graph(3, {{0, 1, 2}, {1, 2, -1}, {2, 0, 2}});
  EXPECT_EQ(*johnson(graph), Matrix({{0, 2, 1}, {1, 0, -1}, {2, 4, 0}}));
}

TEST(Test, NegativeCycle) {
  Graph graph(3, {{0, 1, 1}, {1, 2, -3}, {2, 0, 1}});
  EXPECT_FALSE(johnson(graph).has_value());
}

TEST(Test, NegativeSelfLoop) {
  Graph graph(2, {{0, 1, 1}, {1, 1, -1}});
  EXPECT_FALSE(johnson(graph).has_value());
}

TEST(Test, NegativeCycleUnreachableFromStart) {
  Graph graph(4, {{0, 1, 1}, {2, 3, -2}, {3, 2, 1}});
  EXPECT_FALSE(johnson(graph).has_value());
}

TEST(Test, PositiveSelfLoopIgnored) {
  Graph graph(2, {{0, 0, 3}, {0, 1, 1}});
  EXPECT_EQ(*johnson(graph), Matrix({{0, 1}, {INF, 0}}));
}

TEST(Test, ZeroWeightCycle) {
  Graph graph(2, {{0, 1, -2}, {1, 0, 2}});
  EXPECT_EQ(*johnson(graph), Matrix({{0, -2}, {2, 0}}));
}

TEST(Test, ParallelEdgesTakeMinimum) {
  Graph graph(2, {{0, 1, 7}, {0, 1, 3}, {0, 1, 5}});
  EXPECT_EQ(*johnson(graph), Matrix({{0, 3}, {INF, 0}}));
}

TEST(Test, DisconnectedVertices) {
  Graph graph(4, {{0, 1, 1}, {2, 3, 2}});
  EXPECT_EQ(*johnson(graph), Matrix({{0, 1, INF, INF}, {INF, 0, INF, INF}, {INF, INF, 0, 2}, {INF, INF, INF, 0}}));
}

TEST(Test, Clrs) {
  Graph graph(5, {{0, 1, 3}, {0, 2, 8}, {0, 4, -4}, {1, 3, 1}, {1, 4, 7}, {2, 1, 4}, {3, 0, 2}, {3, 2, -5}, {4, 3, 6}});
  Matrix expected = {{0, 1, -3, 2, -4}, {3, 0, -4, 1, -1}, {7, 4, 0, 5, 3}, {2, -1, -5, 0, -2}, {8, 5, 1, 6, 0}};
  EXPECT_EQ(*johnson(graph), expected);
}