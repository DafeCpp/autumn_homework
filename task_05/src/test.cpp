#include <gtest/gtest.h>
#include "../../lib/src/util.hpp"

TEST(DegreeConstrainedMSTTest, SimpleChain) {
  std::vector<std::vector<Graph::Edge>> graph(3);

  graph[0].push_back({1, 1});
  graph[1].push_back({0, 1});

  graph[1].push_back({2, 2});
  graph[2].push_back({1, 2});

  Graph g(graph, true);

  long long result = g.DegreeConstrainedMST(2);

  ASSERT_EQ(result, 3);
}

TEST(DegreeConstrainedMSTTest, ImpossibleDueToDegreeLimit) {
  std::vector<std::vector<Graph::Edge>> graph(3);

  graph[0].push_back({1, 1});
  graph[1].push_back({0, 1});

  graph[1].push_back({2, 2});
  graph[2].push_back({1, 2});

  Graph g(graph, true);

  ASSERT_THROW(g.DegreeConstrainedMST(1), std::runtime_error);
}

TEST(DegreeConstrainedMSTTest, MultipleChoices) {
  std::vector<std::vector<Graph::Edge>> graph(4);

  graph[0].push_back({1, 1});
  graph[1].push_back({0, 1});

  graph[0].push_back({2, 3});
  graph[2].push_back({0, 3});

  graph[1].push_back({3, 4});
  graph[3].push_back({1, 4});

  graph[2].push_back({3, 1});
  graph[3].push_back({2, 1});

  graph[0].push_back({3, 2});
  graph[3].push_back({0, 2});

  Graph g(graph, true);

  long long result = g.DegreeConstrainedMST(2);

  ASSERT_EQ(result, 1 + 1 + 2);
}

TEST(DegreeConstrainedMSTTest, ParallelEdges) {
  std::vector<std::vector<Graph::Edge>> graph(2);

  graph[0].push_back({1, 10});
  graph[1].push_back({0, 10});

  graph[0].push_back({1, 3});
  graph[1].push_back({0, 3});

  Graph g(graph, true);

  long long result = g.DegreeConstrainedMST(1);

  ASSERT_EQ(result, 3);
}