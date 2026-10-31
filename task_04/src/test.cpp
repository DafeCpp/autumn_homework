#include "johnson.h"

#include <gtest/gtest.h>

#include <tuple>
#include <vector>

namespace {

Graph CreateGraph(
    int n, const std::vector<std::tuple<int, int, long long>>& edges) {
  Graph g;
  g.n = n;
  g.adj.resize(n + 1);

  for (const auto& [u, v, w] : edges) {
    g.adj[u].push_back({v, w});
  }

  return g;
}

}  // namespace

TEST(JohnsonTest, NegativeEdgesNoCycle) {
  Graph g = CreateGraph(4, {{1, 2, 2},
                            {1, 3, 5},
                            {3, 2, -4},
                            {2, 4, 3},
                            {3, 4, 6}});

  bool has_negative_cycle = false;
  const auto dist = Johnson(g, has_negative_cycle);

  ASSERT_FALSE(has_negative_cycle);

  EXPECT_EQ(dist[1][1], 0);
  EXPECT_EQ(dist[1][2], 1);
  EXPECT_EQ(dist[1][3], 5);
  EXPECT_EQ(dist[1][4], 4);

  EXPECT_EQ(dist[2][1], kInf);
  EXPECT_EQ(dist[2][2], 0);
  EXPECT_EQ(dist[2][3], kInf);
  EXPECT_EQ(dist[2][4], 3);

  EXPECT_EQ(dist[3][1], kInf);
  EXPECT_EQ(dist[3][2], -4);
  EXPECT_EQ(dist[3][3], 0);
  EXPECT_EQ(dist[3][4], -1);

  EXPECT_EQ(dist[4][1], kInf);
  EXPECT_EQ(dist[4][2], kInf);
  EXPECT_EQ(dist[4][3], kInf);
  EXPECT_EQ(dist[4][4], 0);
}

TEST(JohnsonTest, NegativeCycle) {
  Graph g = CreateGraph(3, {{1, 2, 1}, {2, 3, -2}, {3, 1, -1}});

  bool has_negative_cycle = false;
  Johnson(g, has_negative_cycle);

  ASSERT_TRUE(has_negative_cycle);
}

TEST(JohnsonTest, NegativeCycleIsolated) {
  Graph g = CreateGraph(5, {{1, 2, 1}, {2, 3, 1}, {4, 5, -1}, {5, 4, -1}});

  bool has_negative_cycle = false;
  Johnson(g, has_negative_cycle);

  ASSERT_TRUE(has_negative_cycle);
}

TEST(JohnsonTest, NegativeSelfLoop) {
  Graph g = CreateGraph(3, {{1, 2, 1}, {2, 2, -1}, {2, 3, 1}});

  bool has_negative_cycle = false;
  Johnson(g, has_negative_cycle);

  ASSERT_TRUE(has_negative_cycle);
}

TEST(JohnsonTest, ZeroWeightCycle) {
  Graph g = CreateGraph(3, {{1, 2, 1}, {2, 3, 1}, {3, 1, -2}});

  bool has_negative_cycle = false;
  const auto dist = Johnson(g, has_negative_cycle);

  ASSERT_FALSE(has_negative_cycle);
  EXPECT_EQ(dist[1][1], 0);
  EXPECT_EQ(dist[1][2], 1);
  EXPECT_EQ(dist[1][3], 2);
}

TEST(JohnsonTest, MultipleEdges) {
  Graph g = CreateGraph(2, {{1, 2, 5}, {1, 2, 3}, {1, 2, 7}});

  bool has_negative_cycle = false;
  const auto dist = Johnson(g, has_negative_cycle);

  ASSERT_FALSE(has_negative_cycle);
  EXPECT_EQ(dist[1][2], 3);
}

TEST(JohnsonTest, NonNegativeSelfLoop) {
  Graph g = CreateGraph(2, {{1, 1, 5}, {1, 2, 3}});

  bool has_negative_cycle = false;
  const auto dist = Johnson(g, has_negative_cycle);

  ASSERT_FALSE(has_negative_cycle);
  EXPECT_EQ(dist[1][1], 0);
  EXPECT_EQ(dist[1][2], 3);
}

TEST(JohnsonTest, UnreachablePairs) {
  Graph g = CreateGraph(3, {{1, 2, 1}});

  bool has_negative_cycle = false;
  const auto dist = Johnson(g, has_negative_cycle);

  ASSERT_FALSE(has_negative_cycle);
  EXPECT_EQ(dist[1][2], 1);
  EXPECT_EQ(dist[1][3], kInf);
  EXPECT_EQ(dist[2][1], kInf);
  EXPECT_EQ(dist[3][1], kInf);
}

TEST(JohnsonTest, SingleVertex) {
  Graph g = CreateGraph(1, {});

  bool has_negative_cycle = false;
  const auto dist = Johnson(g, has_negative_cycle);

  ASSERT_FALSE(has_negative_cycle);
  EXPECT_EQ(dist[1][1], 0);
}

TEST(JohnsonTest, NoEdges) {
  Graph g = CreateGraph(3, {});

  bool has_negative_cycle = false;
  const auto dist = Johnson(g, has_negative_cycle);

  ASSERT_FALSE(has_negative_cycle);

  for (int i = 1; i <= 3; ++i) {
    for (int j = 1; j <= 3; ++j) {
      if (i == j) {
        EXPECT_EQ(dist[i][j], 0);
      } else {
        EXPECT_EQ(dist[i][j], kInf);
      }
    }
  }
}
