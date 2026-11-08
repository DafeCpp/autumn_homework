#include <iostream>

#include "maximum_flow.hpp"

int main() {
  int n, m;
  std::cin >> n >> m;

  std::vector<std::unordered_map<int, Flow>> graph(
      n, std::unordered_map<int, Flow>());
  for (int i = 0; i < m; ++i) {
    int u, v, c;
    std::cin >> u >> v >> c;
    --u;
    --v;
    graph[u][v].capacity += c;  // если есть кратные ребра
    if (!graph[v].count(u)) graph[v][u] = {0, 0};
  }
  MaxFlow flow(graph);
  std::cout << flow.max_flow << "\n";
}
