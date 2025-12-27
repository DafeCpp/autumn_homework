#include <iostream>

#include "topological_sort.hpp"

int main() {
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

  std::vector<int> ans = TopologicalSort(graph);

  for (auto i : ans) {
    std::cout << i << " ";
  }

  return 0;
}
