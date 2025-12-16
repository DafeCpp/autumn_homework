#include <gtest/gtest.h>

#include "../../lib/src/util.hpp"

TEST(LCATest, SimpleTree) {
  std::vector<std::vector<int>> tree = {{1, 2}, {3}, {}, {}};

  LCA lca(tree);

  EXPECT_EQ(lca.Query(1, 2), 0);
  EXPECT_EQ(lca.Query(3, 2), 0);
  EXPECT_EQ(lca.Query(3, 1), 1);
  EXPECT_EQ(lca.Query(2, 2), 2);
}

TEST(LCATest, ChainTree) {
  std::vector<std::vector<int>> tree = {{1}, {2}, {3}, {}};

  LCA lca(tree);

  EXPECT_EQ(lca.Query(3, 0), 0);
  EXPECT_EQ(lca.Query(2, 3), 2);
  EXPECT_EQ(lca.Query(1, 3), 1);
}

TEST(LCATest, BalancedTree) {
  std::vector<std::vector<int>> tree = {{1, 2}, {3, 4}, {5, 6}, {}, {}, {}, {}};

  LCA lca(tree);

  EXPECT_EQ(lca.Query(3, 4), 1);
  EXPECT_EQ(lca.Query(3, 5), 0);
  EXPECT_EQ(lca.Query(5, 6), 2);
  EXPECT_EQ(lca.Query(4, 6), 0);
}

TEST(LCATest, SameNode) {
  std::vector<std::vector<int>> tree = {{1, 2}, {3, 4}, {5, 6}, {}, {}, {}, {}};

  LCA lca(tree);

  EXPECT_EQ(lca.Query(3, 3), 3);
  EXPECT_EQ(lca.Query(0, 0), 0);
}

TEST(LCATest, RootWithChildren) {
  std::vector<std::vector<int>> tree = {{1, 2, 3}, {}, {}, {}};

  LCA lca(tree);

  EXPECT_EQ(lca.Query(1, 2), 0);
  EXPECT_EQ(lca.Query(1, 3), 0);
  EXPECT_EQ(lca.Query(2, 3), 0);
}