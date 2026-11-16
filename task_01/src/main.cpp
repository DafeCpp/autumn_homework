#include <iostream>
#include <vector>

#include "task1.hpp"

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

  std::vector<int> result = Solve(n, edges);

  if (result.size() == 1 && result[0] == -1) {
    std::cout << -1 << "\n";
  } else {
    for (size_t i = 0; i < result.size(); ++i) {
      std::cout << result[i] << (i + 1 == result.size() ? "" : " ");
    }
    std::cout << "\n";
  }

  return 0;
}
