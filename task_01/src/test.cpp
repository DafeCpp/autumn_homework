#include <gtest/gtest.h>

#include <algorithm>
#include <cassert>
#include <graphs.hpp>
#include <iostream>
#include <vector>
struct Edge {
  int from, to;
};

bool isValidTopologicalOrder(const std::vector<int>& order,
                             const std::vector<Edge>& edges) {
  if (order.empty()) return true;

  std::vector<int> position(order.size());
  for (int i = 0; i < (int)order.size(); ++i) {
    position[order[i]] = i;
  }

  for (const auto& edge : edges) {
    if (position[edge.from] < position[edge.to]) {
      return false;
    }
  }

  return true;
}

void testSimpleGraph() {
  std::cout << "Test 1: Simple graph without cycle... ";

  Graph g(3);
  g.addEdge(0, 1);
  g.addEdge(1, 2);

  std::vector<int> order;
  bool result = g.topologicalSort(order);

  assert(result == true);
  assert(order.size() == 3);

  std::vector<Edge> edges = {{0, 1}, {1, 2}};
  assert(isValidTopologicalOrder(order, edges));

  std::cout << "PASSED" << std::endl;
}

void testGraphWithCycle() {
  std::cout << "Test 2: Graph with cycle... ";

  Graph g(4);
  g.addEdge(0, 1);
  g.addEdge(1, 2);
  g.addEdge(2, 0);
  g.addEdge(3, 1);

  std::vector<int> order;
  bool result = g.topologicalSort(order);

  assert(result == false);

  std::cout << "PASSED" << std::endl;
}

void testEmptyGraph() {
  std::cout << "Test 3: Empty graph (no edges)... ";

  Graph g(5);

  std::vector<int> order;
  bool result = g.topologicalSort(order);

  assert(result == true);
  assert(order.size() == 5);

  std::vector<int> sorted_order = order;
  std::sort(sorted_order.begin(), sorted_order.end());
  for (int i = 0; i < 5; ++i) {
    assert(sorted_order[i] == i);
  }

  std::cout << "PASSED" << std::endl;
}

void testSingleVertex() {
  std::cout << "Test 4: Single vertex graph... ";

  Graph g(1);

  std::vector<int> order;
  bool result = g.topologicalSort(order);

  assert(result == true);
  assert(order.size() == 1);
  assert(order[0] == 0);

  std::cout << "PASSED" << std::endl;
}

void testSelfLoop() {
  std::cout << "Test 5: Self-loop (cycle of length 1)... ";

  Graph g(3);
  g.addEdge(0, 0);

  std::vector<int> order;
  bool result = g.topologicalSort(order);

  assert(result == false);

  std::cout << "PASSED" << std::endl;
}

void testMultipleComponents() {
  std::cout << "Test 6: Multiple independent components... ";

  Graph g(6);
  // Компонента 1: 0 -> 1 -> 2
  g.addEdge(0, 1);
  g.addEdge(1, 2);
  // Компонента 2: 3 -> 4
  g.addEdge(3, 4);
  // Вершина 5 изолирована

  std::vector<int> order;
  bool result = g.topologicalSort(order);

  assert(result == true);
  assert(order.size() == 6);

  std::vector<Edge> edges = {{0, 1}, {1, 2}, {3, 4}};
  assert(isValidTopologicalOrder(order, edges));

  std::cout << "PASSED" << std::endl;
}

void testComplexDAG() {
  std::cout << "Test 7: Complex Graph... ";

  Graph g(7);
  g.addEdge(0, 1);
  g.addEdge(0, 2);
  g.addEdge(1, 3);
  g.addEdge(2, 3);
  g.addEdge(3, 4);
  g.addEdge(3, 5);
  g.addEdge(4, 6);
  g.addEdge(5, 6);

  std::vector<int> order;
  bool result = g.topologicalSort(order);

  assert(result == true);
  assert(order.size() == 7);

  std::vector<Edge> edges = {{0, 1}, {0, 2}, {1, 3}, {2, 3},
                             {3, 4}, {3, 5}, {4, 6}, {5, 6}};
  assert(isValidTopologicalOrder(order, edges));

  std::cout << "PASSED" << std::endl;
}

// Тест 8: Большой цикл
void testLargeCycle() {
  std::cout << "Test 8: Large cycle... ";

  int n = 100;
  Graph g(n);

  // Создаём цикл: 0 -> 1 -> 2 -> ... -> 99 -> 0
  for (int i = 0; i < n; ++i) {
    g.addEdge(i, (i + 1) % n);
  }

  std::vector<int> order;
  bool result = g.topologicalSort(order);

  assert(result == false);

  std::cout << "PASSED" << std::endl;
}

void testLongChain() {
  std::cout << "Test 9: Long chain without cycle... ";

  int n = 100;
  Graph g(n);

  // Создаём цепочку: 0 -> 1 -> 2 -> ... -> 99
  for (int i = 0; i < n - 1; ++i) {
    g.addEdge(i, i + 1);
  }

  std::vector<int> order;
  bool result = g.topologicalSort(order);

  assert(result == true);
  assert(order.size() == n);

  for (int i = 0; i < n; ++i) {
    assert(order[i] == n - 1 - i);
  }

  std::cout << "PASSED" << std::endl;
}

void testInvalidEdges() {
  std::cout << "Test 10: Invalid edges (out of range)... ";

  Graph g(3);
  g.addEdge(0, 1);
  g.addEdge(-1, 2);
  g.addEdge(0, 5);
  g.addEdge(1, 2);

  std::vector<int> order;
  bool result = g.topologicalSort(order);

  assert(result == true);
  assert(order.size() == 3);

  std::vector<Edge> edges = {{0, 1}, {1, 2}};
  assert(isValidTopologicalOrder(order, edges));

  std::cout << "PASSED" << std::endl;
}

int main() {
  std::cout << "Running tests for Graph class..." << std::endl;
  std::cout << "================================" << std::endl;

  testSimpleGraph();
  testGraphWithCycle();
  testEmptyGraph();
  testSingleVertex();
  testSelfLoop();
  testMultipleComponents();
  testComplexDAG();
  testLargeCycle();
  testLongChain();
  testInvalidEdges();

  std::cout << "================================" << std::endl;
  std::cout << "All tests PASSED!" << std::endl;

  return 0;
}
