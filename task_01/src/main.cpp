#include <iostream>

#include "find_bridges.hpp"
#include "find_critical_points.hpp"

int main() {
  int n, m;
  std::cin >> n >> m;

  Graph<int> graph(false);
  for (int i = 0; i < n; i++) graph.AddVertex(i + 1);

  for (int i = 0; i < m; i++) {
    int u, v;
    std::cin >> u >> v;
    graph.AddEdge(u, v);
  }

  std::vector<Edge<int>> bridges = FindBridges(graph);
  std::vector<int> points = FindJointVertices(graph);

  std::cout << points.size() << std::endl;
  if (points.size() == 0) std::cout << "-";
  for (auto i : points) {
    std::cout << i << " ";
  }
  std::cout << "\n";
  std::cout << bridges.size() << std::endl;
  if (bridges.size() == 0) std::cout << "-" << std::endl;
  for (auto i : bridges) {
    std::cout << i.v1 << " " << i.v2 << "; ";
  }

  return 0;
}
