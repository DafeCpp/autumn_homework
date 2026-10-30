#include <gtest/gtest.h>

#include "task4.hpp"

TEST(JohnsonTests, SimpleGraph) {
  int n = 3;

  std::vector<Edge> edges(3);
  edges[0] = {0, 1, -2};
  edges[1] = {1, 2, 3};
  edges[2] = {0, 2, 2};

  std::vector<std::vector<long long>> res = Johnson(n, edges);

  ASSERT_EQ(res.empty(), false);

  ASSERT_EQ(res[0][0], 0);
  ASSERT_EQ(res[0][1], -2);
  ASSERT_EQ(res[0][2], 1);

  ASSERT_EQ(res[1][1], 0);
  ASSERT_EQ(res[1][2], 3);

  ASSERT_EQ(res[2][2], 0);
}

TEST(JohnsonTests, NegativeCycle) {
  int n = 2;

  std::vector<Edge> edges(2);
  edges[0] = {0, 1, 1};
  edges[1] = {1, 0, -2};

  std::vector<std::vector<long long>> res = Johnson(n, edges);

  ASSERT_EQ(res.empty(), true);
}
