#include <gtest/gtest.h>
#include "solution.hpp"

TEST(Test, Chain) {
  Graph graph({{1}, {2}, {}});
  EXPECT_EQ(get_requirements(graph), std::vector<int>({2, 1, 0}));
}

TEST(Test, CycleWithTail) {
  Graph graph({{1}, {2}, {0}, {1}});
  EXPECT_EQ(get_requirements(graph), std::vector<int>({-1}));
}

TEST(Test, NoDependencies) {
  Graph graph({{}, {}, {}});
  EXPECT_EQ(get_requirements(graph), std::vector<int>({0, 1, 2}));
}

TEST(Test, SelfLoop) {
  Graph graph({{}, {1}});
  EXPECT_EQ(get_requirements(graph), std::vector<int>({-1}));
}

TEST(Test, Diamond) {
  Graph graph({{1, 2}, {3}, {3}, {}});
  EXPECT_EQ(get_requirements(graph), std::vector<int>({3, 1, 2, 0}));
}

TEST(Test, CycleInSeparateComponent) {
  Graph graph({{}, {0}, {3}, {4}, {2}});
  EXPECT_EQ(get_requirements(graph), std::vector<int>({-1}));
}

TEST(Test, SingleVertex) {
  std::vector<std::vector<int>> adj(1);
  Graph graph(adj);
  EXPECT_EQ(get_requirements(graph), std::vector<int>({0}));
}

TEST(Test, DuplicateDependencies) {
  Graph graph({{1, 1}, {}});
  EXPECT_EQ(get_requirements(graph), std::vector<int>({1, 0}));
}

TEST(Test, TenGraph) {
  Graph graph({{1, 2}, {3, 4}, {4, 5}, {6}, {6, 7}, {7}, {8}, {8, 9}, {}, {}});
  EXPECT_EQ(get_requirements(graph), std::vector<int>({8, 6, 3, 9, 7, 4, 1, 5, 2, 0}));
}
