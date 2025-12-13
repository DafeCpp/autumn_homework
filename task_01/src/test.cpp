#include <gtest/gtest.h>

#include "../../lib/src/util.hpp"

std::pair<int, int> NormalizeEdge(std::pair<int, int> edge) {
  if (edge.first > edge.second) {
    return std::make_pair(edge.second, edge.first);
  }
  return edge;
}

TEST(BridgesTest, SimpleChain) {
  std::vector<std::vector<int> > adjList(4);

  adjList[0].push_back(1);
  adjList[1].push_back(0);

  adjList[1].push_back(2);
  adjList[2].push_back(1);

  adjList[2].push_back(3);
  adjList[3].push_back(2);

  Graph graph(adjList);

  std::vector<std::pair<int, int> > bridges = graph.FindBridges();

  ASSERT_EQ(bridges.size(), 3);

  std::vector<std::pair<int, int> > expected;
  expected.push_back(std::make_pair(0, 1));
  expected.push_back(std::make_pair(1, 2));
  expected.push_back(std::make_pair(2, 3));

  for (int i = 0; i < bridges.size(); ++i) {
    bridges[i] = NormalizeEdge(bridges[i]);
  }

  for (int i = 0; i < expected.size(); ++i) {
    expected[i] = NormalizeEdge(expected[i]);
  }

  std::sort(bridges.begin(), bridges.end());
  std::sort(expected.begin(), expected.end());

  ASSERT_EQ(bridges, expected);
}

TEST(BridgesTest, CycleGraph) {
  std::vector<std::vector<int> > adjList(4);

  adjList[0].push_back(1);
  adjList[1].push_back(0);

  adjList[1].push_back(2);
  adjList[2].push_back(1);

  adjList[2].push_back(3);
  adjList[3].push_back(2);

  adjList[3].push_back(0);
  adjList[0].push_back(3);

  Graph graph(adjList);

  std::vector<std::pair<int, int> > bridges = graph.FindBridges();

  ASSERT_TRUE(bridges.empty());
}

TEST(BridgesTest, CycleWithTail) {
  std::vector<std::vector<int> > adjList(5);

  adjList[0].push_back(1);
  adjList[1].push_back(0);

  adjList[1].push_back(2);
  adjList[2].push_back(1);

  adjList[2].push_back(0);
  adjList[0].push_back(2);

  adjList[2].push_back(3);
  adjList[3].push_back(2);

  adjList[3].push_back(4);
  adjList[4].push_back(3);

  Graph graph(adjList);

  std::vector<std::pair<int, int> > bridges = graph.FindBridges();

  ASSERT_EQ(bridges.size(), 2);

  std::vector<std::pair<int, int> > expected;
  expected.push_back(std::make_pair(2, 3));
  expected.push_back(std::make_pair(3, 4));

  for (int i = 0; i < bridges.size(); ++i) {
    bridges[i] = NormalizeEdge(bridges[i]);
  }

  for (int i = 0; i < expected.size(); ++i) {
    expected[i] = NormalizeEdge(expected[i]);
  }

  std::sort(bridges.begin(), bridges.end());
  std::sort(expected.begin(), expected.end());

  ASSERT_EQ(bridges, expected);
}

TEST(BridgesTest, TreeGraph) {
  std::vector<std::vector<int> > adjList(6);

  adjList[0].push_back(1);
  adjList[1].push_back(0);

  adjList[1].push_back(2);
  adjList[2].push_back(1);

  adjList[1].push_back(3);
  adjList[3].push_back(1);

  adjList[3].push_back(4);
  adjList[4].push_back(3);

  adjList[3].push_back(5);
  adjList[5].push_back(3);

  Graph graph(adjList);

  std::vector<std::pair<int, int> > bridges = graph.FindBridges();

  ASSERT_EQ(bridges.size(), 5);
}