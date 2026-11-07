#include <gtest/gtest.h>
#include <sstream>
#include "solution.hpp"


TEST(Test, Example1) {
  Graph graph({{1}, {}});
  EXPECT_EQ(get_min_edges(graph), 1);
}

TEST(Test, Example2) {
  Graph graph({{1}, {3}, {1}, {}});
  EXPECT_EQ(get_min_edges(graph), 2);
}

TEST(Test, Example3) {
  Graph graph({{1}, {3, 4}, {1}, {}, {}});
  EXPECT_EQ(get_min_edges(graph), 2);
}

TEST(Test, SingleVertex) {
  std::vector<std::vector<int>> adj(1);
  Graph graph(adj);
  EXPECT_EQ(get_min_edges(graph), 0);
}

TEST(Test, SingleVertexWithSelfLoop) {
  std::vector<std::vector<int>> adj = {{0}};
  Graph graph(adj);
  EXPECT_EQ(get_min_edges(graph), 0);
}

TEST(Test, AlreadyOneCycle) {
  Graph graph({{1}, {2}, {0}});
  EXPECT_EQ(get_min_edges(graph), 0);
}

TEST(Test, IsolatedVertices) {
  std::vector<std::vector<int>> adj(3);
  Graph graph(adj);
  EXPECT_EQ(get_min_edges(graph), 3);
}

TEST(Test, Chain) {
  Graph graph({{1}, {2}, {3}, {4}, {}});
  EXPECT_EQ(get_min_edges(graph), 1);
}

TEST(Test, OutSt) {
  Graph graph({{1, 2, 3}, {}, {}, {}});
  EXPECT_EQ(get_min_edges(graph), 3);
}

TEST(Test, TwoCyclesConnected) {
  Graph graph({{1}, {0, 2}, {3}, {2}});
  EXPECT_EQ(get_min_edges(graph), 1);
}

TEST(Test, ReadFromInput) {
  std::istringstream in("4 3\n1 2\n3 2\n2 4\n");
  std::streambuf* old = std::cin.rdbuf(in.rdbuf());
  Graph graph;
  std::cin.rdbuf(old);
  EXPECT_EQ(get_min_edges(graph), 2);
}

TEST(Test, LongChain) {
  int n = 1000;
  std::vector<std::vector<int>> adj(n);
  for (int i = 0; i + 1 < n; ++i) {
    adj[i].push_back(i + 1);
  }
  Graph graph(adj);
  EXPECT_EQ(get_min_edges(graph), 1);
}

TEST(Test, LongCycle) {
  int n = 1000;
  std::vector<std::vector<int>> adj(n);
  for (int i = 0; i < n; ++i) {
    adj[i].push_back((i + 1) % n);
  }
  Graph graph(adj);
  EXPECT_EQ(get_min_edges(graph), 0);
}
