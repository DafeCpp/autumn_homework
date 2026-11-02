#include <gtest/gtest.h>

#include <sstream>

#include "solution.hpp"

TEST(Test, SingleVertex) {
  Graph graph(1, {{0, 0, 5}}, 1);
  EXPECT_EQ(get_min_weight(graph), 0);
}

TEST(Test, TwoVertices) {
  Graph graph(2, {{0, 1, 7}}, 1);
  EXPECT_EQ(get_min_weight(graph), 7);
}

TEST(Test, DegreeLimitDoesNotMatter) {
  Graph graph(3, {{0, 1, 1}, {1, 2, 2}, {0, 2, 10}}, 2);
  EXPECT_EQ(get_min_weight(graph), 3);
}

TEST(Test, StarIsForbidden) {
  Graph graph(4, {{0, 1, 1}, {0, 2, 1}, {0, 3, 1}, {1, 2, 5}, {2, 3, 5}, {1, 3, 5}}, 2);
  EXPECT_EQ(get_min_weight(graph), 7);
}

TEST(Test, StarIsAllowed) {
  Graph graph(4, {{0, 1, 1}, {0, 2, 1}, {0, 3, 1}, {1, 2, 5}, {2, 3, 5}, {1, 3, 5}}, 3);
  EXPECT_EQ(get_min_weight(graph), 3);
}

TEST(Test, ImpossibleStar) {
  Graph graph(4, {{0, 1, 1}, {0, 2, 1}, {0, 3, 1}}, 2);
  EXPECT_EQ(get_min_weight(graph), -1);
}

TEST(Test, ImpossibleDegreeOne) {
  Graph graph(3, {{0, 1, 1}, {1, 2, 1}, {0, 2, 1}}, 1);
  EXPECT_EQ(get_min_weight(graph), -1);
}

TEST(Test, DisconnectedGraph) {
  Graph graph(4, {{0, 1, 1}, {2, 3, 1}}, 3);
  EXPECT_EQ(get_min_weight(graph), -1);
}

TEST(Test, LoopsAndMultiEdges) {
  Graph graph(3, {{0, 0, 1}, {0, 1, 9}, {0, 1, 2}, {1, 2, 3}, {2, 1, 4}}, 2);
  EXPECT_EQ(get_min_weight(graph), 5);
}

TEST(Test, ForcedDetour) {
  Graph graph(5, {{0, 1, 1}, {0, 2, 1}, {0, 3, 1}, {0, 4, 1}, {1, 2, 3}, {2, 3, 3}, {3, 4, 3}}, 2);
  EXPECT_EQ(get_min_weight(graph), 8);
}

TEST(Test, PathWhenDegreeTwo) {
  Graph graph(5, {{0, 1, 4}, {1, 2, 4}, {2, 3, 4}, {3, 4, 4}, {0, 2, 1}, {0, 3, 1}, {0, 4, 1}, {0, 1, 4}}, 2);
  EXPECT_EQ(get_min_weight(graph), 10);
}

TEST(Test, ReadFromInput) {
  std::istringstream in("4 6 2\n1 2 1\n1 3 1\n1 4 1\n2 3 5\n3 4 5\n2 4 5\n");
  std::streambuf* old = std::cin.rdbuf(in.rdbuf());
  Graph graph;
  std::cin.rdbuf(old);
  EXPECT_EQ(get_min_weight(graph), 7);
}
