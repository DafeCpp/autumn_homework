#include <gtest/gtest.h>

#include <cmath>

#include "graph.hpp"

void ExpectDistances(
    const std::optional<std::vector<std::vector<double>>>& dist,
    const std::optional<std::vector<std::vector<double>>>& expected) {
  if (!expected) {
    EXPECT_TRUE(!dist);
    return;
  }
  for (int i = 0; i < (*expected).size(); ++i) {
    for (int j = 0; j < (*expected)[i].size(); ++j) {
      double got = (*dist)[i][j];
      double want = (*expected)[i][j];
      if (want == INFINITY)
        ASSERT_EQ(got, INFINITY);
      else
        EXPECT_NEAR(got, want, 1e-9);
    }
  }
}

TEST(JohnsonGraph, SingleVertexNoEdges) {
  Graph g(1, {});
  ExpectDistances(g.distances, std::vector<std::vector<double>>{{0}});
}

TEST(JohnsonGraph, TwoVerticesNoEdges) {
  Graph g(2, {});
  ExpectDistances(g.distances, std::vector<std::vector<double>>{{0, INFINITY},
                                                                {INFINITY, 0}});
}

TEST(JohnsonGraph, SingleVertexNegativeLoop) {
  std::vector<Edge> edges = {{0, 0, -5}};
  Graph g(1, edges);
  ExpectDistances(g.distances, std::nullopt);
}

TEST(JohnsonGraph, SingleVertexPositiveLoop) {
  std::vector<Edge> edges = {{0, 0, 7}};
  Graph g(1, edges);
  ExpectDistances(g.distances, std::vector<std::vector<double>>{{0}});
}

TEST(JohnsonGraph, SimpleChain) {
  std::vector<Edge> edges = {{0, 1, 1}, {1, 2, 2}};
  Graph g(3, edges);
  ExpectDistances(g.distances,
                  std::vector<std::vector<double>>{
                      {0, 1, 3}, {INFINITY, 0, 2}, {INFINITY, INFINITY, 0}});
}

TEST(JohnsonGraph, SimpleCyclePositive) {
  std::vector<Edge> edges = {{0, 1, 1}, {1, 2, 2}, {2, 0, 3}};
  Graph g(3, edges);
  ExpectDistances(g.distances, std::vector<std::vector<double>>{
                                   {0, 1, 3}, {5, 0, 2}, {3, 4, 0}});
}

TEST(JohnsonGraph, SimpleCycleNegative) {
  std::vector<Edge> edges = {{0, 1, 1}, {1, 2, 2}, {2, 0, -4}};
  Graph g(3, edges);
  ExpectDistances(g.distances, std::nullopt);
}

TEST(JohnsonGraph, DisconnectedComponents) {
  std::vector<Edge> edges = {{0, 1, 5}, {2, 3, 7}};
  Graph g(4, edges);
  ExpectDistances(g.distances, std::vector<std::vector<double>>{
                                   {0, 5, INFINITY, INFINITY},
                                   {INFINITY, 0, INFINITY, INFINITY},
                                   {INFINITY, INFINITY, 0, 7},
                                   {INFINITY, INFINITY, INFINITY, 0}});
}

TEST(JohnsonGraph, MultipleEdgesSameDirection) {
  std::vector<Edge> edges = {{0, 1, 5}, {0, 1, 2}};
  Graph g(2, edges);
  ExpectDistances(g.distances,
                  std::vector<std::vector<double>>{{0, 2}, {INFINITY, 0}});
}

TEST(JohnsonGraph, MultipleEdgesWithNegative) {
  std::vector<Edge> edges = {{0, 1, 3}, {0, 1, -1}};
  Graph g(2, edges);
  ExpectDistances(g.distances,
                  std::vector<std::vector<double>>{{0, -1}, {INFINITY, 0}});
}

TEST(JohnsonGraph, NegativeEdgesNoCycle) {
  std::vector<Edge> edges = {{0, 1, -5}, {1, 2, -3}, {0, 2, 0}};
  Graph g(3, edges);
  ExpectDistances(g.distances,
                  std::vector<std::vector<double>>{
                      {0, -5, -8}, {INFINITY, 0, -3}, {INFINITY, INFINITY, 0}});
}

TEST(JohnsonGraph, ZeroWeightCycle) {
  std::vector<Edge> edges = {{0, 1, 1}, {1, 2, -1}, {2, 0, 0}};
  Graph g(3, edges);
  ExpectDistances(g.distances, std::vector<std::vector<double>>{
                                   {0, 1, 0}, {-1, 0, -1}, {0, 1, 0}});
}

TEST(JohnsonGraph, PositiveSelfLoop) {
  std::vector<Edge> edges = {{0, 0, 10}, {0, 1, 3}};
  Graph g(2, edges);
  ExpectDistances(g.distances,
                  std::vector<std::vector<double>>{{0, 3}, {INFINITY, 0}});
}

TEST(JohnsonGraph, ZeroSelfLoop) {
  std::vector<Edge> edges = {{0, 0, 0}, {0, 1, 2}};
  Graph g(2, edges);
  ExpectDistances(g.distances,
                  std::vector<std::vector<double>>{{0, 2}, {INFINITY, 0}});
}

TEST(JohnsonGraph, NegativeSelfLoop) {
  std::vector<Edge> edges = {{1, 1, -1}};
  Graph g(2, edges);
  ExpectDistances(g.distances, std::nullopt);
}

TEST(JohnsonGraph, TwoNegativeCycles) {
  std::vector<Edge> edges = {{0, 1, -1}, {1, 0, -1}, {2, 3, -2}, {3, 2, -2}};
  Graph g(4, edges);
  ExpectDistances(g.distances, std::nullopt);
}

TEST(JohnsonGraph, FractionalWeights) {
  std::vector<Edge> edges = {{0, 1, 1.5}, {1, 2, -0.5}, {0, 2, 3.0}};
  Graph g(3, edges);
  ExpectDistances(
      g.distances,
      std::vector<std::vector<double>>{
          {0, 1.5, 1.0}, {INFINITY, 0, -0.5}, {INFINITY, INFINITY, 0}});
}