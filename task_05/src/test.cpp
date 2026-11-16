#include <gtest/gtest.h>

#include "task5.cpp"

TEST(BoundedMSTTests, SimpleChain) {
  int n = 3;
  int m = 2;
  int d = 2;
  std::vector<Edge> edges = {{1, 2, 10, 0}, {2, 3, 5, 1}};

  Res res = SolvedMST(n, m, d, edges);
  ASSERT_EQ(res.success, true);
  ASSERT_EQ(res.total_weight, 15);
}

TEST(BoundedMSTTests, ImpossibleDegreeStar) {
  int n = 4;
  int m = 3;
  int d = 2;
  std::vector<Edge> edges = {{1, 2, 1, 0}, {1, 3, 1, 1}, {1, 4, 1, 2}};

  Res res = SolvedMST(n, m, d, edges);
  ASSERT_EQ(res.success, false);
}

TEST(BoundedMSTTests, DisconnectedGraph) {
  int n = 4;
  int m = 2;
  int d = 3;
  std::vector<Edge> edges = {{1, 2, 5, 0}, {2, 3, 5, 1}};

  Res res = SolvedMST(n, m, d, edges);
  ASSERT_EQ(res.success, false);
}

TEST(BoundedMSTTests, GraphWithSelfLoops) {
  int n = 2;
  int m = 2;
  int d = 1;
  std::vector<Edge> edges = {
      {1, 1, 100, 0},  // Петля (должна проигнорироваться)
      {1, 2, 5, 1}};

  Res res = SolvedMST(n, m, d, edges);
  ASSERT_EQ(res.success, true);
  ASSERT_EQ(res.total_weight, 5);
}

TEST(BoundedMSTTests, LocalSearchSwap) {
  int n = 4;
  int m = 4;
  int d = 2;
  std::vector<Edge> edges = {
      {1, 2, 1, 0},
      {1, 3, 1, 1},
      {1, 4, 1, 2},
      {2, 4, 10, 3}  // Спасительное более дорогое ребро
  };

  Res res = SolvedMST(n, m, d, edges);
  ASSERT_EQ(res.success, true);
  ASSERT_EQ(res.total_weight, 12);
}
