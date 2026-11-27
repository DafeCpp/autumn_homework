#include <gtest/gtest.h>

#include <random>
#include <utility>
#include <vector>

#include "task8.h"

static int naive_lca(int u, int v, const std::vector<int>& parent,
                     const std::vector<int>& depth) {
  while (depth[u] > depth[v]) {
    u = parent[u];
  }
  while (depth[v] > depth[u]) {
    v = parent[v];
  }
  while (u != v) {
    u = parent[u];
    v = parent[v];
  }
  return u;
}

TEST(LCATest, SingleNode) {
  LCA lca(1, {}, 1);
  EXPECT_EQ(lca.get_lca(1, 1), 1);
}

TEST(LCATest, SimpleTree) {
  std::vector<std::pair<int, int>> edges = {{1, 2}, {1, 3}, {2, 4}, {2, 5},
                                            {3, 6}, {5, 7}, {5, 8}};
  LCA lca(8, edges, 1);

  EXPECT_EQ(lca.get_lca(1, 1), 1);
  EXPECT_EQ(lca.get_lca(7, 7), 7);

  EXPECT_EQ(lca.get_lca(2, 7), 2);
  EXPECT_EQ(lca.get_lca(7, 2), 2);
  EXPECT_EQ(lca.get_lca(1, 6), 1);

  EXPECT_EQ(lca.get_lca(7, 8), 5);
  EXPECT_EQ(lca.get_lca(4, 8), 2);
  EXPECT_EQ(lca.get_lca(8, 4), 2);
  EXPECT_EQ(lca.get_lca(7, 6), 1);
  EXPECT_EQ(lca.get_lca(4, 3), 1);
}

TEST(LCATest, CustomRootAndReversedEdges) {
  std::vector<std::pair<int, int>> edges = {{8, 5}, {6, 3}, {2, 1}, {5, 2},
                                            {3, 1}, {4, 2}, {7, 5}};
  LCA lca(8, edges, 2);

  EXPECT_EQ(lca.get_lca(1, 6), 1);
  EXPECT_EQ(lca.get_lca(4, 6), 2);
  EXPECT_EQ(lca.get_lca(7, 8), 5);
  EXPECT_EQ(lca.get_lca(1, 5), 2);
  EXPECT_EQ(lca.get_lca(3, 6), 3);
}

TEST(LCATest, ZeroBasedIndexing) {
  std::vector<std::pair<int, int>> edges = {{0, 1}, {0, 2}, {1, 3}, {1, 4}};
  LCA lca(5, edges, 0);

  EXPECT_EQ(lca.get_lca(3, 4), 1);
  EXPECT_EQ(lca.get_lca(3, 2), 0);
  EXPECT_EQ(lca.get_lca(0, 4), 0);
}

TEST(LCATest, StarTree) {
  const int n = 1000;
  std::vector<std::pair<int, int>> edges;
  edges.reserve(n - 1);
  for (int i = 2; i <= n; ++i) {
    edges.push_back({1, i});
  }

  LCA lca(n, edges, 1);

  EXPECT_EQ(lca.get_lca(1, 1), 1);
  EXPECT_EQ(lca.get_lca(2, 2), 2);
  EXPECT_EQ(lca.get_lca(2, n), 1);
  EXPECT_EQ(lca.get_lca(123, 456), 1);
}

TEST(LCATest, DeepBambooNoStackOverflow) {
  const int n = 100000;
  std::vector<std::pair<int, int>> edges;
  edges.reserve(n - 1);
  for (int i = 1; i < n; ++i) {
    edges.push_back({i, i + 1});
  }

  LCA lca(n, edges, 1);

  EXPECT_EQ(lca.get_lca(1, n), 1);
  EXPECT_EQ(lca.get_lca(n, 1), 1);
  EXPECT_EQ(lca.get_lca(50000, n), 50000);
  EXPECT_EQ(lca.get_lca(n - 1, n), n - 1);
  EXPECT_EQ(lca.get_lca(n, n), n);
}

TEST(LCATest, RandomTreesAgainstNaive) {
  std::mt19937 rng(42);

  for (int test = 0; test < 20; ++test) {
    int n = 50 + static_cast<int>(rng() % 100);
    std::vector<std::pair<int, int>> edges;
    std::vector<int> parent(n + 1, 0);
    std::vector<int> depth(n + 1, 0);

    parent[1] = 1;
    for (int i = 2; i <= n; ++i) {
      int p = 1 + static_cast<int>(rng() % (i - 1));
      parent[i] = p;
      depth[i] = depth[p] + 1;
      if (rng() % 2 == 0) {
        edges.push_back({p, i});
      } else {
        edges.push_back({i, p});
      }
    }

    LCA lca(n, edges, 1);

    for (int q = 0; q < 500; ++q) {
      int u = 1 + static_cast<int>(rng() % n);
      int v = 1 + static_cast<int>(rng() % n);
      ASSERT_EQ(lca.get_lca(u, v), naive_lca(u, v, parent, depth))
          << "Failed on u=" << u << ", v=" << v;
    }
  }
}

TEST(LCATest, LargeBinaryTreePerformance) {
  const int n = 100000;
  std::vector<std::pair<int, int>> edges;
  edges.reserve(n - 1);
  for (int i = 2; i <= n; ++i) {
    edges.push_back({i / 2, i});
  }

  LCA lca(n, edges, 1);
  std::mt19937 rng(12345);

  for (int q = 0; q < 100000; ++q) {
    int u = 1 + static_cast<int>(rng() % n);
    int v = 1 + static_cast<int>(rng() % n);

    int expected_u = u;
    int expected_v = v;
    while (expected_u != expected_v) {
      if (expected_u > expected_v) {
        expected_u /= 2;
      } else {
        expected_v /= 2;
      }
    }

    ASSERT_EQ(lca.get_lca(u, v), expected_u);
  }
}