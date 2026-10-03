#include <iostream>
#include <vector>

#include "network.h"

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  int n, m;
  if (!(std::cin >> n >> m)) {
    return 0;
  }

  NetworkAnalyzer analyzer(n);
  for (int i = 0; i < m; ++i) {
    int u, v;
    std::cin >> u >> v;
    analyzer.addEdge(u, v, i);
  }

  NetworkResult result = analyzer.solve();

  // 1. Количество критических устройств
  std::cout << result.cut_vertices.size() << "\n";

  // 2. Список критических устройств
  if (result.cut_vertices.empty()) {
    std::cout << "-\n";
  } else {
    for (size_t i = 0; i < result.cut_vertices.size(); ++i) {
      std::cout << result.cut_vertices[i] << (i + 1 == result.cut_vertices.size() ? "" : " ");
    }
    std::cout << "\n";
  }

  // 3. Количество критических соединений
  std::cout << result.bridges.size() << "\n";

  // 4. Список критических соединений
  if (result.bridges.empty()) {
    std::cout << "-\n";
  } else {
    for (size_t i = 0; i < result.bridges.size(); ++i) {
      std::cout << result.bridges[i].first << " " << result.bridges[i].second;
      if (i + 1 < result.bridges.size()) {
        std::cout << "; ";
      }
    }
    std::cout << "\n";
  }

  return 0;
}
