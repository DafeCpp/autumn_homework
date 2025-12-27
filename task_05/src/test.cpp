#include <gtest/gtest.h>

#include <vector>

#include "MST.hpp"

TEST(TestDegreeConstrainedMST, SimpleTree) {
  std::vector<Edge> edges = {{1, 2, 1}};
  ASSERT_EQ(DegreeConstrainedMST(2, 1, 1, edges), 1);
}

TEST(TestDegreeConstrainedMST, SmallGraphPossible) {
  std::vector<Edge> edges = {{1, 2, 1}, {2, 3, 2}, {1, 3, 3}};
  ASSERT_EQ(DegreeConstrainedMST(3, 3, 2, edges), 3);
}

TEST(TestDegreeConstrainedMST, SmallGraphImpossible) {
  std::vector<Edge> edges = {{1, 2, 1}, {2, 3, 2}, {1, 3, 3}};
  ASSERT_EQ(DegreeConstrainedMST(3, 3, 1, edges), -1);
}

TEST(TestDegreeConstrainedMST, MediumGraph) {
  std::vector<Edge> edges = {
      {1, 2, 1}, {1, 3, 2}, {2, 3, 3}, {2, 4, 4}, {3, 4, 1}};
  ASSERT_EQ(DegreeConstrainedMST(4, 5, 2, edges), 4);
}

TEST(TestDegreeConstrainedMST, SingleVertex) {
  std::vector<Edge> edges;
  ASSERT_EQ(DegreeConstrainedMST(1, 0, 1, edges), 0);
}

TEST(TestDegreeConstrainedMST, LinearChain) {
  std::vector<Edge> edges = {{1, 2, 1}, {2, 3, 1}, {3, 4, 1}};
  ASSERT_EQ(DegreeConstrainedMST(4, 3, 1, edges), -1);
  ASSERT_EQ(DegreeConstrainedMST(4, 3, 2, edges), 3);
}

TEST(TestDegreeConstrainedMST, Last_Test) {
  std::vector<Edge> edges = {
      {1, 2, 3}, {2, 4, 4}, {1, 3, 1}, {3, 4, 2}, {2, 3, 1}};

  ASSERT_EQ(DegreeConstrainedMST(4, 4, 3, edges), 4);
}