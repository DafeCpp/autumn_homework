#include <gtest/gtest.h>

#include <algorithm>
#include <random>

#include "bellman.hpp"
#include "dijkstra.hpp"
#include "johnson.hpp"

TEST(Johnson, NegativeEdges) {
  std::vector<std::vector<long long>> result;
  ASSERT_TRUE(johnson(5, {{0, 1, 3}, {0, 2, 8}, {0, 4, -4},
                            {1, 3, 1}, {1, 4, 7}, {2, 1, 4},
                            {3, 0, 2}, {3, 2, -5}, {4, 3, 6}}, result));
  EXPECT_EQ(result, (std::vector<std::vector<long long>>{{0, 1, -3, 2, -4},
                                     {3, 0, -4, 1, -1},
                                     {7, 4, 0, 5, 3},
                                     {2, -1, -5, 0, -2},
                                     {8, 5, 1, 6, 0}}));
}

TEST(Johnson, DisconnectedAndParallelEdges) {
  std::vector<std::vector<long long>> result;
  ASSERT_TRUE(johnson(4, {{0, 1, 9}, {0, 1, -2}, {2, 3, 5}}, result));
  EXPECT_EQ(result, (std::vector<std::vector<long long>>{{0, -2, INF, INF},
                                     {INF, 0, INF, INF},
                                     {INF, INF, 0, 5},
                                     {INF, INF, INF, 0}}));
}

TEST(Johnson, NegativeCycleInDisconnectedComponent) {
  std::vector<std::vector<long long>> result;
  EXPECT_FALSE(johnson(4, {{0, 1, 1}, {2, 3, -2}, {3, 2, 1}}, result));
  EXPECT_FALSE(johnson(1, {{0, 0, -1}}, result));
}

TEST(Johnson, EmptyAndSingleVertex) {
  std::vector<std::vector<long long>> result;
  ASSERT_TRUE(johnson(0, {}, result));
  EXPECT_TRUE(result.empty());
  ASSERT_TRUE(johnson(1, {{0, 0, 0}}, result));
  EXPECT_EQ(result, (std::vector<std::vector<long long>>{{0}}));
}

TEST(Johnson, LargeDistances) {
  std::vector<std::vector<long long>> result;
  ASSERT_TRUE(johnson(3, {{0, 1, 3000000000LL}, {1, 2, 3000000000LL}}, result));
  EXPECT_EQ(result[0][2], 6000000000LL);
}

TEST(Bellman, IgnoresUnreachableNegativeCycle) {
  std::vector<long long> distance;
  ASSERT_TRUE(bellman(3, {{1, 2, -1}, {2, 1, -1}}, 0, distance));
  EXPECT_EQ(distance, (std::vector<long long>{0, INF, INF}));
}

TEST(Dijkstra, BothVersionsHandleUnreachableVertices) {
  std::vector<std::vector<std::pair<int, long long>>> graph = {{{1, 4}, {2, 10}}, {{2, 2}}, {}, {}};
  std::vector<long long> expected = {0, 4, 6, INF};
  EXPECT_EQ(dijkstra_simple(graph, 0), expected);
  EXPECT_EQ(dijkstra(graph, 0), expected);
}

TEST(Johnson, MatchesFloydWarshallOnRandomGraphs) {
  std::mt19937 random(42);
  for (int trial = 0; trial < 200; ++trial) {
    int n = 1 + random() % 7;
    std::vector<Edge> edges;
    std::vector<std::vector<long long>> expected(n, std::vector<long long>(n, INF));
    for (int a = 0; a < n; ++a) {
      expected[a][a] = 0;
      for (int b = 0; b < n; ++b) {
        if (random() % 4 == 0) {
          long long weight = static_cast<int>(random() % 16) - 5;
          edges.push_back({a, b, weight});
          expected[a][b] = std::min(expected[a][b], weight);
        }
      }
    }
    for (int k = 0; k < n; ++k) {
      for (int a = 0; a < n; ++a) {
        for (int b = 0; b < n; ++b) {
          if (expected[a][k] != INF && expected[k][b] != INF) {
            expected[a][b] =
                std::min(expected[a][b], expected[a][k] + expected[k][b]);
          }
        }
      }
    }
    bool negativeCycle = false;
    for (int v = 0; v < n; ++v) {
      negativeCycle = negativeCycle || expected[v][v] < 0;
    }
    std::vector<std::vector<long long>> result;
    bool success = johnson(n, edges, result);
    ASSERT_EQ(success, !negativeCycle) << "trial " << trial;
    if (success) {
      EXPECT_EQ(result, expected) << "trial " << trial;
    }
  }
}
