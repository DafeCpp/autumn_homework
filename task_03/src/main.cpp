#include <algorithm>
#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>

#include "bridges_and_articulation_points.hpp"

int main() {
  int n, m;
  std::cin >> n >> m;

  std::vector<std::vector<Edge>> graph(n + 1);
  std::vector<int> tin(n + 1, -1);
  std::vector<int> low(n + 1, -1);
  std::vector<bool> isCut(n + 1, false);
  std::vector<std::pair<int, int>> bridges;
  int timer = 0;
  GraphTrace trace;
  trace.Graph(n);

  for (int id = 0; id < m; ++id) {
    int u, v;
    std::cin >> u >> v;

    graph[u].push_back({v, id});
    graph[v].push_back({u, id});
    trace.AddEdge(id, u, v);
  }
  trace.Step("Граф построен");

  for (int v = 1; v <= n; ++v) {
    if (tin[v] == -1) {
      dfs(v, -1, graph, tin, low, isCut, bridges, timer, trace);
    }
  }
  trace.Step("DFS завершен");
  trace.Finish();

  std::vector<int> cutVertices;

  for (int v = 1; v <= n; ++v) {
    if (isCut[v]) {
      cutVertices.push_back(v);
    }
  }

  sort(bridges.begin(), bridges.end());

  std::cout << cutVertices.size() << '\n';

  if (cutVertices.empty()) {
    std::cout << "-\n";
  } else {
    for (int i = 0; i < cutVertices.size(); ++i) {
      if (i > 0) {
        std::cout << ' ';
      }

      std::cout << cutVertices[i];
    }

    std::cout << '\n';
  }

  std::cout << bridges.size() << '\n';

  if (bridges.empty()) {
    std::cout << "-\n";
  } else {
    for (int i = 0; i < bridges.size(); ++i) {
      if (i > 0) {
        std::cout << "; ";
      }

      std::cout << bridges[i].first << ' ' << bridges[i].second;
    }

    std::cout << '\n';
  }

  return 0;
}
