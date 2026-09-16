#include <gtest/gtest.h>

#include <random>
#include <stdexcept>

#include "scc.hpp"

namespace {
using scc_example::Edge;
void Check(int n, const std::vector<Edge>& edges,
           const std::vector<int>& expected) {
  EXPECT_EQ(scc_example::Kosaraju(n, edges), expected);
  EXPECT_EQ(scc_example::Tarjan(n, edges), expected);
}
TEST(Scc, SingleAndIsolated) {
  Check(1, {}, {0, 1});
  Check(4, {}, {0, 1, 2, 3, 4});
}
TEST(Scc, ConnectedCycles) {
  Check(6, {{1, 2}, {2, 3}, {3, 1}, {3, 4}, {4, 5}, {5, 4}, {5, 6}},
        {0, 1, 1, 1, 4, 4, 6});
}
TEST(Scc, LoopsAndParallelEdges) {
  Check(3, {{1, 1}, {1, 2}, {1, 2}, {2, 1}, {3, 3}}, {0, 1, 1, 3});
}
TEST(Scc, CrossEdgeToStackAndEdgeToCompletedComponent) {
  // DFS уже вышел из 2, но 2 остаётся в стеке при обработке 3 → 2.
  // Позднее 4 → 2 ведёт в готовую КСС и не должно менять lowlink[4].
  Check(4, {{1, 2}, {2, 1}, {1, 3}, {3, 2}, {4, 2}}, {0, 1, 1, 1, 4});
}
TEST(Scc, InvalidInput) {
  for (auto algorithm : {scc_example::Kosaraju, scc_example::Tarjan}) {
    EXPECT_THROW(algorithm(0, {}, nullptr), std::invalid_argument);
    EXPECT_THROW(algorithm(2, {{0, 1}}, nullptr), std::invalid_argument);
    EXPECT_THROW(algorithm(2, {{1, 3}}, nullptr), std::invalid_argument);
  }
}
TEST(Scc, LongChainWithoutRecursion) {
  const int n = 100000;
  std::vector<Edge> edges;
  std::vector<int> expected(n + 1);
  for (int v = 1; v <= n; ++v) {
    expected[v] = v;
    if (v > 1) edges.push_back({v - 1, v});
  }
  Check(n, edges, expected);
  edges.push_back({n, 1});
  std::fill(expected.begin() + 1, expected.end(), 1);
  Check(n, edges, expected);
}
TEST(Scc, RandomGraphsAgainstReachabilityOracle) {
  std::mt19937 rng(20260916);
  for (int sample = 0; sample < 300; ++sample) {
    const int n = 1 + rng() % 12;
    std::vector<Edge> edges;
    std::vector<std::vector<bool>> reach(n + 1, std::vector<bool>(n + 1));
    for (int u = 1; u <= n; ++u) {
      reach[u][u] = true;
      for (int v = 1; v <= n; ++v) {
        if (rng() % 5 == 0) {
          edges.push_back({u, v});
          reach[u][v] = true;
        }
      }
    }
    for (int k = 1; k <= n; ++k)
      for (int u = 1; u <= n; ++u)
        for (int v = 1; v <= n; ++v)
          reach[u][v] = reach[u][v] || (reach[u][k] && reach[k][v]);
    std::vector<int> expected(n + 1);
    for (int u = 1; u <= n; ++u)
      for (int v = 1; v <= n; ++v)
        if (reach[u][v] && reach[v][u]) {
          expected[u] = v;
          break;
        }
    Check(n, edges, expected);
  }
}
}  // namespace
