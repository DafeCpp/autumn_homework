#include <iostream>
#include <string>
#include <vector>

#include "installation_order.hpp"

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n, m;
  std::cin >> n >> m;

  std::vector<std::vector<int>> dependents(n);
  for (int i = 0; i < m; ++i) {
    int a, b;
    std::cin >> a >> b;
    dependents[b].push_back(a);
  }

  const auto order = installation_order(dependents);
  if (order.size() != dependents.size()) {
    std::cout << -1 << '\n';
    return 0;
  }

  for (int i = 0; i < n; ++i) {
    if (i > 0) {
      std::cout << ' ';
    }
    std::cout << order[i];
  }
  std::cout << '\n';
}