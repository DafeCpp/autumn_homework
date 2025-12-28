#include <gtest/gtest.h>

#include "MST.hpp"

TEST(DegreeBoundedMST, EmptyGraph) {
  WeightedGraph<int> g(false);
  ASSERT_EQ(DegreeBoundedMST(g, 1), 0);
}

TEST(DegreeBoundedMST, SingleVertex) {
  WeightedGraph<int> g(false);
  g.AddVertex(1);
  ASSERT_EQ(DegreeBoundedMST(g, 1), 0);
}

TEST(DegreeBoundedMST, TwoVerticesOneEdge) {
  WeightedGraph<int> g(false);
  g.AddWeightedEdge(1, 2, 5);
  ASSERT_EQ(DegreeBoundedMST(g, 1), 5);
}

TEST(DegreeBoundedMST, TwoVerticesDegreeZeroImpossible) {
  WeightedGraph<int> g(false);
  g.AddWeightedEdge(1, 2, 5);
  ASSERT_EQ(DegreeBoundedMST(g, 0), -1);
}

TEST(DegreeBoundedMST, DisconnectedGraph) {
  WeightedGraph<int> g(false);
  g.AddWeightedEdge(1, 2, 1);
  g.AddWeightedEdge(3, 4, 1);
  ASSERT_EQ(DegreeBoundedMST(g, 2), -1);
}

TEST(DegreeBoundedMST, SimpleTreeAlreadyValid) {
  WeightedGraph<int> g(false);
  g.AddWeightedEdge(1, 2, 1);
  g.AddWeightedEdge(2, 3, 1);
  g.AddWeightedEdge(3, 4, 1);
  ASSERT_EQ(DegreeBoundedMST(g, 2), 3);
}

TEST(DegreeBoundedMST, SimpleTreeDegreeTooSmall) {
  WeightedGraph<int> g(false);
  g.AddWeightedEdge(1, 2, 1);
  g.AddWeightedEdge(2, 3, 1);
  g.AddWeightedEdge(3, 4, 1);
  ASSERT_EQ(DegreeBoundedMST(g, 1), -1);
}

TEST(DegreeBoundedMST, LoopsIgnored) {
  WeightedGraph<int> g(false);
  g.AddWeightedEdge(1, 1, 100);
  g.AddWeightedEdge(1, 2, 1);
  g.AddWeightedEdge(2, 3, 1);
  ASSERT_EQ(DegreeBoundedMST(g, 2), 2);
}

TEST(DegreeBoundedMST, MultipleEdgesChooseMinimum) {
  WeightedGraph<int> g(false);
  g.AddWeightedEdge(1, 2, 10);
  g.AddWeightedEdge(1, 2, 1);
  g.AddWeightedEdge(2, 3, 1);
  ASSERT_EQ(DegreeBoundedMST(g, 2), 2);
}

TEST(DegreeBoundedMST, GivenCounterExample) {
  WeightedGraph<int> g(false);
  g.AddWeightedEdge(1, 2, 1);
  g.AddWeightedEdge(2, 3, 1);
  g.AddWeightedEdge(1, 3, 100);
  g.AddWeightedEdge(3, 4, 200);
  g.AddWeightedEdge(2, 4, 1);

  ASSERT_EQ(DegreeBoundedMST(g, 2), 102);
}

TEST(DegreeBoundedMST, StarGraphDegreeLimited) {
  WeightedGraph<int> g(false);
  g.AddWeightedEdge(1, 2, 1);
  g.AddWeightedEdge(1, 3, 1);
  g.AddWeightedEdge(1, 4, 1);
  g.AddWeightedEdge(1, 5, 1);

  ASSERT_EQ(DegreeBoundedMST(g, 1), -1);
}

TEST(DegreeBoundedMST, StarGraphAllowedDegree) {
  WeightedGraph<int> g(false);
  g.AddWeightedEdge(1, 2, 1);
  g.AddWeightedEdge(1, 3, 1);
  g.AddWeightedEdge(1, 4, 1);
  g.AddWeightedEdge(1, 5, 1);

  ASSERT_EQ(DegreeBoundedMST(g, 4), 4);
}

TEST(DegreeBoundedMST, CompleteGraphDegreeTwo) {
  WeightedGraph<int> g(false);
  g.AddWeightedEdge(1, 2, 1);
  g.AddWeightedEdge(1, 3, 1);
  g.AddWeightedEdge(1, 4, 1);
  g.AddWeightedEdge(2, 3, 1);
  g.AddWeightedEdge(2, 4, 1);
  g.AddWeightedEdge(3, 4, 1);

  ASSERT_EQ(DegreeBoundedMST(g, 2), 3);
}

TEST(DegreeBoundedMST, LargeDegreeEqualsNormalMST) {
  WeightedGraph<int> g(false);
  g.AddWeightedEdge(1, 2, 3);
  g.AddWeightedEdge(2, 3, 4);
  g.AddWeightedEdge(3, 4, 5);
  g.AddWeightedEdge(1, 4, 100);

  ASSERT_EQ(DegreeBoundedMST(g, 10), 12);
}

TEST(DegreeBoundedMST, NoPossibleDegreeBoundedTree) {
  WeightedGraph<int> g(false);
  g.AddWeightedEdge(1, 2, 1);
  g.AddWeightedEdge(1, 3, 1);
  g.AddWeightedEdge(1, 4, 1);
  g.AddWeightedEdge(2, 3, 10);
  g.AddWeightedEdge(3, 4, 10);
  g.AddWeightedEdge(2, 4, 10);

  ASSERT_EQ(DegreeBoundedMST(g, 1), -1);
}

TEST(DegreeBoundedMST, LargeGraphN1000M10000) {
  WeightedGraph<int> g(false);

  const int n = 1000;
  const int m = 10000;
  const int d = 2;

  for (int i = 1; i < n; ++i) {
    g.AddWeightedEdge(i, i + 1, 1);
  }

  g.AddWeightedEdge(1, n, 1000000);

  ASSERT_EQ(DegreeBoundedMST(g, d), n - 1);
}

TEST(DegreeBoundedMST, LargeGraphNoSolution) {
  WeightedGraph<int> g(false);

  const int n = 1000;

  for (int i = 2; i <= n; ++i) g.AddWeightedEdge(1, i, 1);

  ASSERT_EQ(DegreeBoundedMST(g, 1), -1);
}
TEST(DegreeBoundedMST, LargeGraphHighDegree) {
  WeightedGraph<int> g(false);

  const int n = 1000;

  for (int i = 1; i < n; ++i) g.AddWeightedEdge(i, i + 1, i);

  ASSERT_EQ(DegreeBoundedMST(g, n), (n - 1) * n / 2);
}
