#include <algorithm>
#include <iostream>
#include <vector>

#include "topological_sorting.hpp"

int main() {
  int n, m;
  std::cin >> n >> m;
  std::vector<std::vector<int>> graph(n, std::vector<int>());
  GraphTrace trace;
  trace.Graph(n, true, 0);
  for (int i = 0; i < m; i++) {
    int vertex1, vertex2;
    std::cin >> vertex1 >> vertex2;
    graph[vertex1].push_back(vertex2);
    trace.AddEdge(i, vertex1, vertex2);
  }

  std::vector<Color> color(n, Color::WHITE);
  std::vector<int> order;

  for (int i = 0; i < n; i++) {
    if (color[i] == Color::WHITE) {
      dfs(i, color, graph, order, trace);
    }
  }
  for (int vertex : order) {
    std::cout << vertex << " ";
  }

  trace.Finish();
  return 0;
}