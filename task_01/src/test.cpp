#include <gtest/gtest.h>

#include <vector>

#include "installation_order.hpp"

TEST(InstallationOrder, NoDependencies) {
  const std::vector<std::vector<int>> graph(3);
  EXPECT_EQ(installation_order(graph).size(), graph.size());
}

TEST(InstallationOrder, IndependentBranchesAndSharedDependent) {
  // 0 -> 2, 1 -> 2, 1 -> 3, 2 -> 4, 3 -> 4. Вершина 5 изолирована.
  const std::vector<std::vector<int>> graph{{2}, {2, 3}, {4}, {4}, {}, {}};
  const auto order = installation_order(graph);
  ASSERT_EQ(order.size(), graph.size());

  const int n = static_cast<int>(graph.size());
  std::vector<int> position(n, -1);
  for (int i = 0; i < n; ++i) {
    const int component = order[i];
    ASSERT_GE(component, 0);
    ASSERT_LT(component, n);
    ASSERT_EQ(position[component], -1);
    position[component] = i;
  }

  for (int from = 0; from < n; ++from) {
    for (int to : graph[from]) {
      EXPECT_LT(position[from], position[to]) << from << " -> " << to;
    }
  }
}

TEST(InstallationOrder, LongChainInReverseIndexOrder) {
  constexpr int n = 200'000;
  std::vector<std::vector<int>> graph(n);
  for (int i = 1; i < n; ++i) {
    graph[i].push_back(i - 1);
  }

  const auto order = installation_order(graph);
  ASSERT_EQ(order.size(), graph.size());
  for (int i = 0; i < n; ++i) {
    ASSERT_EQ(order[i], n - 1 - i) << "position " << i;
  }
}

TEST(InstallationOrder, SelfLoopWithIndependentVertex) {
  const std::vector<std::vector<int>> graph{{0}, {}};
  EXPECT_LT(installation_order(graph).size(), graph.size());
}

TEST(InstallationOrder, CycleInDisconnectedComponent) {
  // 0 -> 1, отдельно 2 -> 3 -> 2 и изолированная вершина 4.
  const std::vector<std::vector<int>> graph{{1}, {}, {3}, {2}, {}};
  EXPECT_LT(installation_order(graph).size(), graph.size());
}

TEST(InstallationOrder, CycleWithIncomingAndOutgoingTails) {
  // 0 -> 1 -> 2 -> 3 -> 1, из цикла выходит ветка 3 -> 4 -> 5.
  const std::vector<std::vector<int>> graph{{1}, {2}, {3}, {1, 4}, {5}, {}};
  EXPECT_LT(installation_order(graph).size(), graph.size());
}

TEST(InstallationOrder, BackEdgeNearEndOfLongChain) {
  constexpr int n = 200'000;
  std::vector<std::vector<int>> graph(n);
  for (int i = 0; i < n - 1; ++i) {
    graph[i].push_back(i + 1);
  }
  graph[n - 1].push_back(n - 3);

  EXPECT_LT(installation_order(graph).size(), graph.size());
}