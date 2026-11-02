#include <gtest/gtest.h>

#include <sstream>

#include "solution.hpp"

TEST(Test, Example) {
  Graph graph(2, {{0, 1}});
  EXPECT_EQ(get_critical(graph), "0\n-\n1\n1 2");
}

TEST(Test, SingleVertex) {
  Graph graph(1, {});
  EXPECT_EQ(get_critical(graph), "0\n-\n0\n-");
}

TEST(Test, Path) {
  Graph graph(3, {{0, 1}, {1, 2}});
  EXPECT_EQ(get_critical(graph), "1\n2\n2\n1 2; 2 3");
}

TEST(Test, Triangle) {
  Graph graph(3, {{0, 1}, {1, 2}, {2, 0}});
  EXPECT_EQ(get_critical(graph), "0\n-\n0\n-");
}

TEST(Test, TriangleWithTail) {
  Graph graph(4, {{0, 1}, {1, 2}, {2, 0}, {2, 3}});
  EXPECT_EQ(get_critical(graph), "1\n3\n1\n3 4");
}

TEST(Test, TwoTrianglesSharingVertex) {
  Graph graph(5, {{0, 1}, {1, 2}, {2, 0}, {2, 3}, {3, 4}, {4, 2}});
  EXPECT_EQ(get_critical(graph), "1\n3\n0\n-");
}

TEST(Test, Star) {
  Graph graph(4, {{0, 1}, {0, 2}, {0, 3}});
  EXPECT_EQ(get_critical(graph), "1\n1\n3\n1 2; 1 3; 1 4");
}

TEST(Test, DisconnectedGraph) {
  Graph graph(6, {{0, 1}, {1, 2}, {3, 4}});
  EXPECT_EQ(get_critical(graph), "1\n2\n3\n1 2; 2 3; 4 5");
}

TEST(Test, NoEdges) {
  Graph graph(3, {});
  EXPECT_EQ(get_critical(graph), "0\n-\n0\n-");
}

TEST(Test, MultiEdgeIsNotBridge) {
  Graph graph(2, {{0, 1}, {0, 1}});
  EXPECT_EQ(get_critical(graph), "0\n-\n0\n-");
}

TEST(Test, SelfLoopIgnored) {
  Graph graph(2, {{0, 0}, {0, 1}});
  EXPECT_EQ(get_critical(graph), "0\n-\n1\n1 2");
}

TEST(Test, ReversedEdgesAreSorted) {
  Graph graph(3, {{2, 1}, {1, 0}});
  EXPECT_EQ(get_critical(graph), "1\n2\n2\n1 2; 2 3");
}

TEST(Test, ReadFromInput) {
  std::istringstream in("5 4\n1 2\n2 3\n3 1\n3 4\n");
  std::streambuf* old = std::cin.rdbuf(in.rdbuf());
  Graph graph;
  std::cin.rdbuf(old);
  EXPECT_EQ(get_critical(graph), "1\n3\n1\n3 4");
}

TEST(Test, LongPath) {
  size_t n = 10000;
  std::vector<std::pair<int, int>> edges;
  for (size_t i = 0; i + 1 < n; ++i) {
    edges.push_back({(int)i, (int)i + 1});
  }
  Graph graph((int)n, edges);
  EXPECT_EQ(graph.get_cut_vertices().size(), n - 2);
  EXPECT_EQ(graph.get_bridges().size(), n - 1);
}

TEST(Test, GetCriticalIsRepeatable) {
  Graph graph(3, {{0, 1}, {1, 2}});
  EXPECT_EQ(get_critical(graph), get_critical(graph));
  EXPECT_EQ(graph.get_bridges().size(), graph.get_bridges().size());
}
