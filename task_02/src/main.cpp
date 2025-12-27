#include <iostream>

#include "districts.hpp"

int main() {
  int n, m;
  std::cin >> n >> m;

  Graph<int> graph;
  for (int i = 0; i < n; i++) graph.AddVertex(i + 1);

  for (int i = 0; i < m; i++) {
    int u, v;
    std::cin >> u >> v;
    graph.AddEdge(u, v);
  }

  std::cout << MinEdgesToMakeOneDistrict(graph);
}
