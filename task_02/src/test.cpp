#include <gtest/gtest.h>

#include "additional_roads.hpp"

TEST(Test, Simple) {
  std::vector<std::vector<int>> graf{{1}, {}};
  AdditionalRoads new_roads(graf);
  ASSERT_EQ(new_roads.amount, 1);
}

TEST(Test, OneVertex) {
  std::vector<std::vector<int>> graf{{}};
  AdditionalRoads new_roads(graf);
  ASSERT_EQ(new_roads.amount, 0);
}

TEST(Test, EmptyTwoVertices) {
  std::vector<std::vector<int>> graf{{}, {}};
  AdditionalRoads new_roads(graf);
  ASSERT_EQ(new_roads.amount, 2);
}

TEST(Test, Cycle) {
  std::vector<std::vector<int>> graf{{1}, {2}, {0}};
  AdditionalRoads new_roads(graf);
  ASSERT_EQ(new_roads.amount, 0);
}

TEST(Test, TwoVertexCycle) {
  std::vector<std::vector<int>> graf{{1}, {0}};
  AdditionalRoads new_roads(graf);
  ASSERT_EQ(new_roads.amount, 0);
}

TEST(Test, SelfLoop) {
  std::vector<std::vector<int>> graf{{0}};
  AdditionalRoads new_roads(graf);
  ASSERT_EQ(new_roads.amount, 0);
}

TEST(Test, SelfLoops) {
  std::vector<std::vector<int>> graf{{0}, {1}, {2}};
  AdditionalRoads new_roads(graf);
  ASSERT_EQ(new_roads.amount, 3);
}

TEST(Test, Chain) {
  std::vector<std::vector<int>> graf{{1}, {2}, {}};
  AdditionalRoads new_roads(graf);
  ASSERT_EQ(new_roads.amount, 1);
}

TEST(Test, Star) {
  std::vector<std::vector<int>> graf{{3}, {3}, {3}, {}};
  AdditionalRoads new_roads(graf);
  ASSERT_EQ(new_roads.amount, 3);
}

TEST(Test, TwoSCCCycle) {
  std::vector<std::vector<int>> graf{{1}, {0, 2}, {3}, {2}};
  AdditionalRoads new_roads(graf);
  ASSERT_EQ(new_roads.amount, 1);
}

TEST(Test, LongChain) {
  std::vector<std::vector<int>> graf(10);
  for (int i = 0; i < 9; ++i) graf[i].push_back(i + 1);
  AdditionalRoads new_roads(graf);
  ASSERT_EQ(new_roads.amount, 1);
}