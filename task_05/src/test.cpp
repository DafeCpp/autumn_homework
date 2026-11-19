#include <gtest/gtest.h>

#include "kruskal.hpp"
#include "mst.hpp"

TEST(ConstrainedMst, UsesCheaperEdgesWithinDegreeLimit) {
  std::vector<Edge> edges = {{0, 1, 1}, {0, 2, 1}, {1, 2, 10}};
  EXPECT_EQ(constrainedMst(3, edges, 0, 1), 11);
  EXPECT_EQ(constrainedMst(3, edges, 0, 2), 2);
}

TEST(ConstrainedMst, RejectsTooSmallDegree) {
  EXPECT_EQ(constrainedMst(3, {{0, 1, 1}, {0, 2, 1}}, 0, 1), -1);
}

TEST(ConstrainedMst, RejectsDisconnectedGraph) {
  EXPECT_EQ(constrainedMst(3, {{1, 2, 1}}, 0, 2), -1);
}

TEST(ConstrainedMst, HandlesSingleVertexAndParallelEdges) {
  EXPECT_EQ(constrainedMst(1, {}, 0, 0), 0);
  EXPECT_EQ(constrainedMst(2, {{0, 1, 7}, {0, 1, 3}}, 0, 1), 3);
}

TEST(Kruskal, FindsMinimumTree) {
  EXPECT_EQ(kruskal(3, {{0, 1, 1}, {1, 2, 2}, {0, 2, 5}}), 3);
  EXPECT_EQ(kruskal(3, {{0, 1, 1}}), -1);
}
