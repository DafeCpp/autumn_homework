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
  const int n = 200000;
  std::vector<std::pair<int, int>> edges;
  std::vector<int> cut_vertices;
  for (int i = 0; i + 1 < n; ++i) {
    edges.push_back({i, i + 1});
    if (i > 0) cut_vertices.push_back(i);
  }
  Graph graph(n, edges);
  ASSERT_EQ(graph.get_cut_vertices(), cut_vertices);
  ASSERT_EQ(graph.get_bridges(), edges);
}

TEST(Test, GetCriticalIsRepeatable) {
  Graph graph(3, {{0, 1}, {1, 2}});
  EXPECT_EQ(get_critical(graph), get_critical(graph));
  EXPECT_EQ(graph.get_bridges().size(), graph.get_bridges().size());
}
