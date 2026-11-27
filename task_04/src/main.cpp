#include <iostream>
#include <string>
#include <vector>

#include "task4.h"

int main() {
  int n, m;
  std::cin >> n >> m;
  std::vector<Edge> edges;
  for (int i = 0; i < m; i++) {
    int u, v, w;
    std::cin >> u >> v >> w;

    edges.push_back({u, v, w});
  }
  auto result = task4(n, m, edges);
  for (std::string i : result) {
    std::cout << i << "\n";
  }
  return 0;
}
