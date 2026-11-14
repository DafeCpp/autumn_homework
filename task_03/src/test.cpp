#include <gtest/gtest.h>

#include "task3.hpp"

TEST(NetworkTests, Example1) {
  int n = 2;
  std::vector<std::pair<int, int>> edges = {{1, 2}};
  NetworkResult res = FindWeakPoints(n, edges);

  ASSERT_EQ(res.critical_routers.size(), 0);
  ASSERT_EQ(res.critical_cables.size(), 1);
  ASSERT_EQ(res.critical_cables[0].first, 1);
  ASSERT_EQ(res.critical_cables[0].second, 2);
}

TEST(NetworkTests, RingGraph) {
  int n = 3;
  std::vector<std::pair<int, int>> edges = {{1, 2}, {2, 3}, {3, 1}};
  NetworkResult res = FindWeakPoints(n, edges);

  ASSERT_EQ(res.critical_routers.size(), 0);
  ASSERT_EQ(res.critical_cables.size(), 0);
}
