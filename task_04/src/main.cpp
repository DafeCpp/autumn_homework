#include <iostream>
#include <vector>

#include "johnson.hpp"

int main() {
  int n, m;
  std::cin >> n >> m;

  std::vector<Edge> edges;
  for (int i = 0; i < m; ++i) {
    int u, v;
    long long weight;
    std::cin >> u >> v >> weight;
    edges.push_back({u, v, weight});
  }

  DistanceMatrix distances;
  if (!johnson(n, edges, distances)) {
    std::cout << "Отрицательный цикл\n";
    return 0;
  }

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (j > 0) {
        std::cout << ' ';
      }
      if (distances[i][j] == INF) {
        std::cout << "INF";
      } else {
        std::cout << distances[i][j];
      }
    }
    std::cout << '\n';
  }
}
