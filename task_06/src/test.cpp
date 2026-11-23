#include <gtest/gtest.h>

#include <sstream>

#include "solution.hpp"

TEST(Test, SinglePipe) {
  Graph graph(2, {{0, 1, 7}});
  EXPECT_EQ(get_max_flow(graph), 7);
}

TEST(Test, Chain) {
  Graph graph(3, {{0, 1, 5}, {1, 2, 3}});
  EXPECT_EQ(get_max_flow(graph), 3);
}

TEST(Test, LongChain) {
  const int n = 200000;
  std::vector<std::vector<long long>> pipes;
  for (int i = 0; i + 1 < n; ++i) {
    pipes.push_back({i, i + 1, i == n / 2 ? 3 : 7});
  }
  ASSERT_EQ(get_max_flow(Graph(n, pipes)), 3);
}

TEST(Test, TwoParallelPaths) {
  Graph graph(4, {{0, 1, 3}, {1, 3, 3}, {0, 2, 4}, {2, 3, 2}});
  EXPECT_EQ(get_max_flow(graph), 5);
}

TEST(Test, ParallelPipesAreSummed) {
  Graph graph(2, {{0, 1, 2}, {0, 1, 3}, {0, 1, 4}});
  EXPECT_EQ(get_max_flow(graph), 9);
}

TEST(Test, NoPath) {
  Graph graph(3, {{0, 1, 5}, {2, 1, 5}});
  EXPECT_EQ(get_max_flow(graph), 0);
}

TEST(Test, NoPipes) {
  Graph graph(2, {});
  EXPECT_EQ(get_max_flow(graph), 0);
}

TEST(Test, SingleVertex) {
  Graph graph(1, {});
  EXPECT_EQ(get_max_flow(graph), 0);
}

TEST(Test, PipesAreOneWay) {
  Graph graph(2, {{1, 0, 5}});
  EXPECT_EQ(graph.get_max_flow(0, 1), 0);
  EXPECT_EQ(get_max_flow(graph), 5);
}

TEST(Test, SourceAndSinkInArbitraryOrder) {
  Graph graph(5, {{3, 0, 4}, {3, 4, 7}, {0, 1, 6}, {4, 1, 3}, {3, 3, 8}});
  EXPECT_EQ(get_max_flow(graph), 7);
}

TEST(Test, NeedsFlowCancellation) {
  Graph graph(4, {{0, 1, 1}, {0, 2, 1}, {1, 2, 1}, {1, 3, 1}, {2, 3, 1}});
  EXPECT_EQ(get_max_flow(graph), 2);
}

TEST(Test, DiamondWithBridge) {
  Graph graph(4, {{0, 1, 10}, {0, 2, 10}, {1, 2, 1}, {1, 3, 10}, {2, 3, 10}});
  EXPECT_EQ(get_max_flow(graph), 20);
}

TEST(Test, Clrs) {
  Graph graph(6, {{0, 1, 16},
                  {0, 2, 13},
                  {1, 2, 10},
                  {2, 1, 4},
                  {1, 3, 12},
                  {3, 2, 9},
                  {2, 4, 14},
                  {4, 3, 7},
                  {3, 5, 20},
                  {4, 5, 4}});
  EXPECT_EQ(get_max_flow(graph), 23);
}

TEST(Test, LoopsAreIgnored) {
  Graph graph(2, {{0, 0, 100}, {0, 1, 4}, {1, 1, 100}});
  EXPECT_EQ(get_max_flow(graph), 4);
}

TEST(Test, BigCapacities) {
  Graph graph(3, {{0, 1, 3000000000LL}, {1, 2, 5000000000LL}});
  EXPECT_EQ(get_max_flow(graph), 3000000000LL);
}