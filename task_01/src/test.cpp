#include <gtest/gtest.h>

#include <vector>

#include "task1.cpp"

// Цепочка зависимостей
TEST(Task1Tests, LinearChain) {
  int n = 3;
  std::vector<std::pair<int, int>> edges = {{0, 1}, {1, 2}};
  std::vector<int> expected = {2, 1, 0};
  ASSERT_EQ(Solve(n, edges), expected);
}

// Цикл из трех вершин
TEST(Task1Tests, CycleDependency) {
  int n = 4;
  std::vector<std::pair<int, int>> edges = {{0, 1}, {1, 2}, {2, 0}, {3, 1}};
  std::vector<int> expected = {-1};
  ASSERT_EQ(Solve(n, edges), expected);
}

// Петля (зависимость от себя)
TEST(Task1Tests, SelfDependencyLoop) {
  int n = 2;
  std::vector<std::pair<int, int>> edges = {{1, 1}};
  std::vector<int> expected = {-1};
  ASSERT_EQ(Solve(n, edges), expected);
}

// Независимые цепочки
TEST(Task1Tests, DisconnectedChains) {
  int n = 4;
  std::vector<std::pair<int, int>> edges = {{0, 1}, {2, 3}};
  std::vector<int> res = Solve(n, edges);

  ASSERT_EQ(res.size(), 4);
  std::vector<int> pos(n);
  for (int i = 0; i < n; ++i) {
    pos[res[i]] = i;
  }
  ASSERT_LT(pos[1], pos[0]);
  ASSERT_LT(pos[3], pos[2]);
}

// Ромбовидный граф
TEST(Task1Tests, DiamondDependency) {
  int n = 4;
  std::vector<std::pair<int, int>> edges = {{0, 1}, {0, 2}, {1, 3}, {2, 3}};
  std::vector<int> res = Solve(n, edges);

  ASSERT_EQ(res.size(), 4);
  ASSERT_EQ(res[0], 3);
  ASSERT_EQ(res[3], 0);
}
