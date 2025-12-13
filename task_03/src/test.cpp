#include <gtest/gtest.h>

#include "../../lib/src/util.hpp"

TEST(Test, Seminar) {
  std ::vector<std ::vector<int>> list_smezhnosti = {
      {1, 3}, {2, 3}, {6}, {2, 4}, {}, {0}, {}};

  Graph g(list_smezhnosti);
  std :: vector<int>  ans = g.TopologySort(5);
  std :: vector<int>  real_ans = {5, 0, 1, 2, 6, 3, 4};
  ASSERT_EQ(real_ans, ans);
}