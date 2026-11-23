#include <gtest/gtest.h>

#include <sstream>

#include "solution.hpp"

TEST(Test, SingleVertex) {
  Graph graph(1, {{0, 0, 5}}, 0, 0);
  EXPECT_EQ(get_min_weight(graph), 0);
}

TEST(Test, LongChain) {
  const int n = 1000;
  std::vector<Edge> edges;
  for (int i = 0; i + 1 < n; ++i) {
    edges.push_back({i, i + 1, 1});
  }
  Graph graph(n, edges, n / 2, 2);
  ASSERT_EQ(get_min_weight(graph), n - 1);
}

TEST(Test, HighDegreeStar) {
  const int n = 1000;
  std::vector<Edge> edges;
  for (int i = 1; i < n; ++i) {
    edges.push_back({0, i, 1});
  }
  EXPECT_EQ(get_min_weight(Graph(n, edges, 0, n - 2)), -1);
  EXPECT_EQ(get_min_weight(Graph(n, edges, 1, 1)), n - 1);
}

TEST(Test, TwoVertices) {
  Graph graph(2, {{0, 1, 7}}, 1, 1);
  EXPECT_EQ(get_min_weight(graph), 7);
}

TEST(Test, ZeroDegree) {
  Graph graph(2, {{0, 1, 7}}, 1, 0);
  EXPECT_EQ(get_min_weight(graph), -1);
}

TEST(Test, DegreeLimitDoesNotMatter) {
  Graph graph(3, {{0, 1, 1}, {1, 2, 2}, {0, 2, 10}}, 1, 2);
  EXPECT_EQ(get_min_weight(graph), 3);
}

TEST(Test, EqualWeightsAllowSeveralDegrees) {
  Graph graph(
      4, {{0, 1, 1}, {0, 2, 1}, {0, 3, 1}, {1, 2, 5}, {2, 3, 5}, {1, 3, 5}}, 0,
      2);
  EXPECT_EQ(get_min_weight(graph), 7);
}

TEST(Test, OnlyOneVertexIsLimited) {
  Graph graph(4, {{0, 1, 1}, {0, 2, 1}, {0, 3, 1}}, 1, 1);
  EXPECT_EQ(get_min_weight(graph), 3);
}

TEST(Test, ImpossibleStar) {
  Graph graph(4, {{0, 1, 1}, {1, 2, 2}, {1, 3, 3}}, 1, 2);
  EXPECT_EQ(get_min_weight(graph), -1);
}

TEST(Test, DisconnectedGraph) {
  Graph graph(4, {{0, 1, 1}, {2, 3, 1}}, 0, 3);
  EXPECT_EQ(get_min_weight(graph), -1);
}

TEST(Test, LoopsAndMultiEdges) {
  Graph graph(3, {{0, 0, 1}, {0, 1, 9}, {0, 1, 2}, {1, 2, 3}, {2, 1, 4}}, 0, 1);
  EXPECT_EQ(get_min_weight(graph), 5);
}

TEST(Test, ForcedDetour) {
  Graph graph(5,
              {{0, 1, 1},
               {0, 2, 1},
               {0, 3, 1},
               {0, 4, 1},
               {1, 2, 3},
               {2, 3, 3},
               {3, 4, 3}},
              0, 2);
  EXPECT_EQ(get_min_weight(graph), 8);
}

TEST(Test, MinDegreeAtTieIsBelowLimit) {
  Graph graph(5,
              {{0, 1, 4},
               {1, 2, 4},
               {2, 3, 4},
               {3, 4, 4},
               {0, 2, 1},
               {0, 3, 1},
               {0, 4, 1},
               {0, 1, 4}},
              0, 2);
  EXPECT_EQ(get_min_weight(graph), 10);
}