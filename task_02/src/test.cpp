#include <gtest/gtest.h>

#include "task2.hpp"

TEST(GraphTests, Example1) {
  int n = 2;
  std::vector<std::pair<int, int>> edges = {{1, 2}};
  ASSERT_EQ(MinEdges(n, edges), 1);
}

TEST(GraphTests, Example2) {
  int n = 4;
  std::vector<std::pair<int, int>> edges = {{1, 2}, {3, 2}, {2, 4}};
  ASSERT_EQ(MinEdges(n, edges), 2);
}

TEST(GraphTests, Example3) {
  int n = 5;
  std::vector<std::pair<int, int>> edges = {{1, 2}, {3, 2}, {2, 4}, {2, 5}};
  ASSERT_EQ(MinEdges(n, edges), 2);
}

// Граф уже полностью связан в один цикл (дороги добавлять не нужно)
TEST(GraphTests, AlreadyConnectedCycle) {
  int n = 3;
  std::vector<std::pair<int, int>> edges = {{1, 2}, {2, 3}, {3, 1}};
  ASSERT_EQ(MinEdges(n, edges), 0);
}

// Вообще нет дорог (каждая вершина сама по себе исток и сток)
TEST(GraphTests, NoEdges) {
  int n = 3;
  std::vector<std::pair<int, int>> edges = {};
  ASSERT_EQ(MinEdges(n, edges), 3);
}

// Только одна вершина в графе
TEST(GraphTests, SingleVertex) {
  int n = 1;
  std::vector<std::pair<int, int>> edges = {};
  ASSERT_EQ(MinEdges(n, edges), 0);
}
