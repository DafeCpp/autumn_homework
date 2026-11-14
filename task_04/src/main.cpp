#include <iostream>
#include <vector>

#include "johnson.hpp"

Graph ReadGraph(int n, int m) {
  Graph g(n);
  for (int i = 0; i < m; ++i) {
    int u, v;
    long long w;
    std::cin >> u >> v >> w;
    g.AddEdge(u, v, w);
  }
  return g;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n, m;
  if (!(std::cin >> n >> m)) {
    return 0;
  }

  Graph g = ReadGraph(n, m);

  bool has_negative_cycle = false;
  std::vector<std::vector<long long>> dist = Johnson(g, has_negative_cycle);

  if (has_negative_cycle) {
    std::cout << "NEGATIVE CYCLE\n";
  } else {
    for (int i = 1; i <= n; ++i) {
      for (int j = 1; j <= n; ++j) {
        if (dist[i][j] == kInf) {
          std::cout << "INF";
        } else {
          std::cout << dist[i][j];
        }
        if (j < n) {
          std::cout << " ";
        }
      }
      std::cout << "\n";
    }
  }

  return 0;
}