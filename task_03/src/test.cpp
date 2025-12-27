#include <gtest/gtest.h>

#include "topological_sort.hpp"

std::vector<int> error = {-1};

TEST(Test_TopologicalSort, Simple_Test_1) {
  Graph<int> graph;

  auto result = TopologicalSort(graph);
  std::vector<int> expected;

  ASSERT_EQ(result, expected);
}

TEST(Test_TopologicalSort, Test_4) {
  Graph<int> graph;

  graph.AddVertex(1);
  graph.AddVertex(2);
  graph.AddVertex(3);
  graph.AddVertex(4);
  graph.AddVertex(5);
  graph.AddVertex(6);
  graph.AddVertex(7);
  graph.AddVertex(8);
  graph.AddVertex(9);
  graph.AddVertex(10);

  graph.AddEdge(1, 10);
  graph.AddEdge(1, 6);
  graph.AddEdge(10, 3);
  graph.AddEdge(10, 5);
  graph.AddEdge(6, 5);
  graph.AddEdge(6, 2);
  graph.AddEdge(3, 8);
  graph.AddEdge(3, 5);
  graph.AddEdge(5, 8);
  graph.AddEdge(5, 4);
  graph.AddEdge(5, 2);
  graph.AddEdge(2, 4);
  graph.AddEdge(2, 7);
  graph.AddEdge(4, 8);
  graph.AddEdge(9, 8);

  auto result = TopologicalSort(graph);
  std::vector<int> expected{9, 1, 6, 10, 3, 5, 2, 7, 4, 8};

  ASSERT_EQ(result, expected);
}

TEST(Test_TopologicalSort, Test_NotAcyclic_1) {
  Graph<int> graph;

  graph.AddVertex(1);
  graph.AddVertex(2);
  graph.AddVertex(3);

  graph.AddEdge(1, 2);
  graph.AddEdge(2, 3);
  graph.AddEdge(3, 1);

  ASSERT_EQ(TopologicalSort(graph), error);
}

TEST(Test_TopologicalSort, Test_NotAcyclic_3) {
  Graph<int> graph;

  graph.AddVertex(1);
  graph.AddVertex(2);
  graph.AddVertex(3);
  graph.AddVertex(4);
  graph.AddVertex(5);
  graph.AddVertex(6);
  graph.AddVertex(7);
  graph.AddVertex(8);
  graph.AddVertex(9);
  graph.AddVertex(10);
  graph.AddVertex(11);

  graph.AddEdge(1, 10);
  graph.AddEdge(10, 11);
  graph.AddEdge(5, 1);
  graph.AddEdge(5, 6);
  graph.AddEdge(4, 8);
  graph.AddEdge(2, 3);
  graph.AddEdge(10, 9);
  graph.AddEdge(4, 5);
  graph.AddEdge(5, 7);
  graph.AddEdge(4, 7);
  graph.AddEdge(1, 2);
  graph.AddEdge(3, 4);
  graph.AddEdge(1, 6);

  ASSERT_EQ(TopologicalSort(graph), error);
}

TEST(Test_TopologicalSort, Test_EmptyGraph) {
  Graph<int> graph;
  graph.AddVertex(1);
  graph.AddVertex(2);
  graph.AddVertex(3);

  std::vector<int> result = TopologicalSort(graph);

  ASSERT_EQ(result.size(), 3);

  std::set<int> vertices(result.begin(), result.end());
  ASSERT_TRUE(vertices.count(1));
  ASSERT_TRUE(vertices.count(2));
  ASSERT_TRUE(vertices.count(3));
}

TEST(Test_TopologicalSort, Test_SimpleAcyclic) {
  Graph<int> graph;

  graph.AddVertex(1);
  graph.AddVertex(2);
  graph.AddVertex(3);

  graph.AddEdge(1, 2);
  graph.AddEdge(2, 3);

  std::vector<int> result = TopologicalSort(graph);

  ASSERT_FALSE(result.size() == 1 && result[0] == -1);

  ASSERT_EQ(result.size(), 3);
  ASSERT_EQ(result[0], 1);
  ASSERT_EQ(result[1], 2);
  ASSERT_EQ(result[2], 3);
}

TEST(Test_TopologicalSort, Test_SimpleCycle) {
  Graph<int> graph;

  graph.AddVertex(1);
  graph.AddVertex(2);
  graph.AddVertex(3);

  graph.AddEdge(1, 2);
  graph.AddEdge(2, 3);
  graph.AddEdge(3, 1);

  std::vector<int> result = TopologicalSort(graph);

  ASSERT_EQ(result.size(), 1);
  ASSERT_EQ(result[0], -1);
}

TEST(Test_TopologicalSort, Test_SelfLoop) {
  Graph<int> graph;

  graph.AddVertex(1);
  graph.AddVertex(2);

  graph.AddEdge(1, 1);
  graph.AddEdge(1, 2);

  std::vector<int> result = TopologicalSort(graph);

  ASSERT_EQ(result.size(), 1);
  ASSERT_EQ(result[0], -1);
}

TEST(Test_TopologicalSort, Test_Disconnected) {
  Graph<int> graph;

  graph.AddVertex(1);
  graph.AddVertex(2);
  graph.AddVertex(3);
  graph.AddVertex(4);
  graph.AddVertex(5);

  graph.AddEdge(1, 2);
  graph.AddEdge(2, 3);

  graph.AddEdge(4, 5);

  std::vector<int> result = TopologicalSort(graph);

  ASSERT_FALSE(result.size() == 1 && result[0] == -1);

  ASSERT_EQ(result.size(), 5);

  std::unordered_map<int, int> position;
  for (int i = 0; i < 5; i++) {
    position[result[i]] = i;
  }

  ASSERT_LT(position[1], position[2]);
  ASSERT_LT(position[2], position[3]);
  ASSERT_LT(position[4], position[5]);
}

TEST(Test_TopologicalSort, Test_MultiplePaths) {
  Graph<int> graph;

  for (int i = 1; i <= 6; i++) {
    graph.AddVertex(i);
  }

  graph.AddEdge(1, 2);
  graph.AddEdge(1, 3);
  graph.AddEdge(2, 4);
  graph.AddEdge(3, 4);
  graph.AddEdge(4, 5);
  graph.AddEdge(4, 6);
  graph.AddEdge(5, 6);

  std::vector<int> result = TopologicalSort(graph);

  ASSERT_FALSE(result.size() == 1 && result[0] == -1);

  ASSERT_EQ(result.size(), 6);

  std::unordered_map<int, int> position;
  for (int i = 0; i < 6; i++) {
    position[result[i]] = i;
  }

  ASSERT_LT(position[1], position[2]);
  ASSERT_LT(position[1], position[3]);
  ASSERT_LT(position[2], position[4]);
  ASSERT_LT(position[3], position[4]);
  ASSERT_LT(position[4], position[5]);
  ASSERT_LT(position[4], position[6]);
  ASSERT_LT(position[5], position[6]);
}

TEST(Test_TopologicalSort, Test_NotAcyclic_4) {
  Graph<int> graph;

  graph.AddVertex(1);
  graph.AddVertex(2);
  graph.AddVertex(3);
  graph.AddVertex(4);
  graph.AddVertex(5);
  graph.AddVertex(6);
  graph.AddVertex(7);
  graph.AddVertex(8);
  graph.AddVertex(9);
  graph.AddVertex(10);
  graph.AddVertex(11);

  graph.AddEdge(1, 10);
  graph.AddEdge(10, 11);
  graph.AddEdge(5, 1);
  graph.AddEdge(5, 6);
  graph.AddEdge(4, 8);
  graph.AddEdge(2, 3);
  graph.AddEdge(10, 9);
  graph.AddEdge(4, 5);
  graph.AddEdge(5, 7);
  graph.AddEdge(4, 7);
  graph.AddEdge(1, 2);
  graph.AddEdge(3, 4);
  graph.AddEdge(1, 6);

  std::vector<int> result = TopologicalSort(graph);

  ASSERT_EQ(result.size(), 1);
  ASSERT_EQ(result[0], -1);
}

TEST(Test_TopologicalSort, Test_LargeAcyclic) {
  Graph<int> graph;

  for (int i = 1; i <= 100; i++) {
    graph.AddVertex(i);
  }

  for (int i = 1; i < 100; i++) {
    graph.AddEdge(i, i + 1);
  }

  std::vector<int> result = TopologicalSort(graph);

  ASSERT_FALSE(result.size() == 1 && result[0] == -1);

  ASSERT_EQ(result.size(), 100);

  for (int i = 0; i < 99; i++) {
    ASSERT_EQ(result[i], i + 1);
  }
}

TEST(Test_TopologicalSort, Test_NoEdges) {
  Graph<int> graph;

  for (int i = 1; i <= 5; i++) {
    graph.AddVertex(i);
  }

  std::vector<int> result = TopologicalSort(graph);

  ASSERT_FALSE(result.size() == 1 && result[0] == -1);

  ASSERT_EQ(result.size(), 5);

  std::set<int> vertices(result.begin(), result.end());
  for (int i = 1; i <= 5; i++) {
    ASSERT_TRUE(vertices.count(i));
  }
}

TEST(Test_TopologicalSort, Test_ComplexCycle) {
  Graph<int> graph;

  for (int i = 1; i <= 8; i++) {
    graph.AddVertex(i);
  }

  graph.AddEdge(1, 2);
  graph.AddEdge(2, 3);
  graph.AddEdge(3, 4);
  graph.AddEdge(4, 5);
  graph.AddEdge(5, 1);

  graph.AddEdge(6, 7);
  graph.AddEdge(7, 8);

  std::vector<int> result = TopologicalSort(graph);

  ASSERT_EQ(result.size(), 1);
  ASSERT_EQ(result[0], -1);
}
