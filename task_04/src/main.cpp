#include <iostream>
#include <vector>
#include "task4.hpp"

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  int m = 0;
  if (!(std::cin >> n >> m)) {
    return 0;
  }

  std::vector<Edge> edges(m);
  for (int i = 0; i < m; ++i) {
    std::cin >> edges[i].from >> edges[i].to >> edges[i].weight;
  }

  std::vector<std::vector<long long>> result = Johnson(n, edges);

  if (result.empty()) {
    std::cout << "Negative cycle detected\n";
    return 0;
  }

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (result[i][j] == 1e18) {
        std::cout << "INF ";
      } else {
        std::cout << result[i][j] << " ";
      }
    }
    std::cout << "\n";
  }

  return 0;
}
