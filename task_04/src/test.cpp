#include <gtest/gtest.h>

#include "../../lib/src/util.hpp"

// TODO: причесать функции Johnson, Dijkstra, BellmanFord в util.cpp, а так они работают
// TODO: дописать тесты

TEST(JohnsonTest, SimpleGraph) {
  // Взвешенный список смежности
  std::vector<std::vector<Graph::Edge>> adjList(4);
  adjList[0].push_back({1, 1});
  adjList[1].push_back({2, 2});
  adjList[2].push_back({3, 3});
  adjList[0].push_back({3, 10});

  Graph g(adjList, true);

  std::vector<std::vector<long long>> dist = g.Johnson();

  ASSERT_EQ(dist[0][0], 0);
  ASSERT_EQ(dist[0][1], 1);
  ASSERT_EQ(dist[0][2], 3);
  ASSERT_EQ(dist[0][3], 6);
  ASSERT_EQ(dist[1][3], 5);
}

TEST(JohnsonTest, NegativeEdge) {
  std::vector<std::vector<Graph::Edge>> adjList(3);
  adjList[0].push_back({1, 4});
  adjList[0].push_back({2, 2});
  adjList[2].push_back({1, -1});

  Graph g(adjList, true);

  std::vector<std::vector<long long>> dist = g.Johnson();

  ASSERT_EQ(dist[0][1], 1);  // путь 0->2->1 = 2+(-1)
  ASSERT_EQ(dist[0][2], 2);
  ASSERT_EQ(dist[2][1], -1);
}

TEST(JohnsonTest, MultiplePaths) {
  std::vector<std::vector<Graph::Edge>> adjList(5);
  adjList[0].push_back({1, 2});
  adjList[0].push_back({2, 5});
  adjList[1].push_back({2, 1});
  adjList[1].push_back({3, 2});
  adjList[2].push_back({3, 1});
  adjList[3].push_back({4, 3});
  adjList[2].push_back({4, 5});

  Graph g(adjList, true);

  std::vector<std::vector<long long>> dist = g.Johnson();

  ASSERT_EQ(dist[0][3], 4);  // путь 0->1->3 = 2+2
  ASSERT_EQ(dist[0][4], 7);  // путь 0->1->3->4 = 2+2+3
  ASSERT_EQ(dist[2][4], 4);  // путь 2->3->4 = 1+3
}