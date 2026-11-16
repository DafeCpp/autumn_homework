#include <gtest/gtest.h>

#include <vector>

#include "task6.cpp"

// 1. Последовательная цепочка труб (поток ограничен самой узкой трубой)
TEST(MaxFlowTests, SimpleChain) {
  int n = 3;
  int m = 2;
  std::vector<Edge> edges = {{1, 2, 10, 0}, {2, 3, 5, 1}};

  Res res = Solve(n, m, edges);
  ASSERT_EQ(res.max_flow, 5);
}

// 2. Два параллельных пути от истока к стоку
TEST(MaxFlowTests, ParallelPaths) {
  int n = 4;
  int m = 4;
  std::vector<Edge> edges = {
      {1, 2, 10, 0}, {1, 3, 10, 1}, {2, 4, 5, 2}, {3, 4, 7, 3}};

  Res res = Solve(n, m, edges);
  ASSERT_EQ(res.max_flow, 12);
}

// 3. Автоматическое определение истока и стока при перемешанных номерах вершин
TEST(MaxFlowTests, AutoSourceSinkDetection) {
  int n = 3;
  int m = 2;
  std::vector<Edge> edges = {
      {3, 2, 15, 0},  // 3 — исток
      {2, 1, 10, 1}   // 1 — сток
  };

  Res res = Solve(n, m, edges);
  ASSERT_EQ(res.max_flow, 10);
}

// 4. Кратные трубы между одними и теми же районами
TEST(MaxFlowTests, MultipleEdges) {
  int n = 2;
  int m = 2;
  std::vector<Edge> edges = {{1, 2, 4, 0}, {1, 2, 6, 1}};

  Res res = Solve(n, m, edges);
  ASSERT_EQ(res.max_flow, 10);
}
