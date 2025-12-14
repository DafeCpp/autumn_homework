#include <gtest/gtest.h>
#include "../../lib/src/util.hpp"

TEST(MaxFlowTest, SinglePath) {
  std::vector<std::vector<Graph::Edge>> graph(2);

  graph[0].push_back({1, 5});

  Graph g(graph, true);

  int maxFlow = g.MaxFlow(0, 1);

  ASSERT_EQ(maxFlow, 5);
}

TEST(MaxFlowTest, TwoParallelPaths) {
  std::vector<std::vector<Graph::Edge>> graph(4);

  graph[0].push_back({1, 3});
  graph[1].push_back({3, 3});

  graph[0].push_back({2, 2});
  graph[2].push_back({3, 2});

  Graph g(graph, true);

  int maxFlow = g.MaxFlow(0, 3);

  ASSERT_EQ(maxFlow, 5);
}

TEST(MaxFlowTest, Bottleneck) {
  std::vector<std::vector<Graph::Edge>> graph(4);

  graph[0].push_back({1, 10});
  graph[1].push_back({2, 1});
  graph[2].push_back({3, 10});

  Graph g(graph, true);

  int maxFlow = g.MaxFlow(0, 3);

  ASSERT_EQ(maxFlow, 1);
}

TEST(MaxFlowTest, MultipleEdges) {
  std::vector<std::vector<Graph::Edge>> graph(2);

  graph[0].push_back({1, 3});
  graph[0].push_back({1, 4});

  Graph g(graph, true);

  int maxFlow = g.MaxFlow(0, 1);

  ASSERT_EQ(maxFlow, 7);
}

TEST(MaxFlowTest, SelfLoopIgnored) {
  std::vector<std::vector<Graph::Edge>> graph(3);

  graph[0].push_back({0, 100});
  graph[0].push_back({1, 5});
  graph[1].push_back({2, 5});

  Graph g(graph, true);

  int maxFlow = g.MaxFlow(0, 2);

  ASSERT_EQ(maxFlow, 5);
}

TEST(MaxFlowTest, ComplexNetwork) {
  std::vector<std::vector<Graph::Edge>> graph(6);

  graph[0].push_back({1, 10});
  graph[0].push_back({2, 10});

  graph[1].push_back({3, 4});
  graph[1].push_back({4, 8});

  graph[2].push_back({4, 9});

  graph[3].push_back({5, 10});
  graph[4].push_back({5, 10});

  Graph g(graph, true);

  int maxFlow = g.MaxFlow(0, 5);

  ASSERT_EQ(maxFlow, 14);
}