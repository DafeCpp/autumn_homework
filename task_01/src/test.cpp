#include <gtest/gtest.h>

#include "task1.hpp"

TEST(Task1Tests, Example1) {
  int n = 3;
  std::vector<std::pair<int, int>> edges = {{0, 1}, {1, 2}};
  std::vector<int> expected = {2, 1, 0};
  ASSERT_EQ(Solve(n, edges), expected);
}

TEST(Task1Tests, Example2) {
  int n = 4;
  std::vector<std::pair<int, int>> edges = {{0, 1}, {1, 2}, {2, 0}, {3, 1}};
  std::vector<int> expected = {-1};
  ASSERT_EQ(Solve(n, edges), expected);
}
