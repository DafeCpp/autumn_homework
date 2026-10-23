#include <iostream>
#include <vector>

#include "task3.hpp"

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

  NetworkResult res = FindWeakPoints(n, edges);

  std::cout << res.critical_routers.size() << "\n";

  if (res.critical_routers.empty()) {
    std::cout << "-\n";
  } else {
    for (size_t i = 0; i < res.critical_routers.size(); ++i) {
      std::cout << res.critical_routers[i]
                << (i + 1 == res.critical_routers.size() ? "" : " ");
    }
    std::cout << "\n";
  }

  std::cout << res.critical_cables.size() << "\n";

  if (res.critical_cables.empty()) {
    std::cout << "-\n";
  } else {
    for (size_t i = 0; i < res.critical_cables.size(); ++i) {
      std::cout << res.critical_cables[i].first << " "
                << res.critical_cables[i].second;
      if (i + 1 < res.critical_cables.size()) {
        std::cout << "; ";
      }
    }
    std::cout << "\n";
  }

  return 0;
}
