#include <iostream>
#include <vector>

#include "task2.hpp"

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  int m = 0;
  if (!(std::cin >> n >> m)) {
    return 0;
  }

  std::vector<std::pair<int, int>> edges(m);
  for (int i = 0; i < m; ++i) {
    std::cin >> edges[i].first >> edges[i].second;
  }

  std::cout << MinEdges(n, edges) << "\n";

  return 0;
}
