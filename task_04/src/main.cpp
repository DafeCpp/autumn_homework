#include <cmath>
#include <iostream>

#include "graph.hpp"

int main() {
  int n, m;
  std::cin >> n >> m;

  std::vector<Flow> edges(m);
  for (int i = 0; i < m; ++i) {
    int u, v;
    double w;

    std::cin >> u >> v >> w;
    edges.push_back({u - 1, v - 1, w});
  }

  std::optional<std::vector<std::vector<double>>> distances =
      Graph(n, edges).distances;
  if (!distances)
    std::cout << "NEGATIVE CYCLE\n";
  else {
    std::vector<std::vector<double>> result = *distances;
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        if (j == 0) {
          if (result[i][j] == INFINITY)
            std::cout << "INF";
          else
            std::cout << result[i][j];
        } else {
          if (result[i][j] == INFINITY)
            std::cout << " " << "INF";
          else
            std::cout << " " << result[i][j];
        }
      }
      std::cout << "\n";
    }
  }
}