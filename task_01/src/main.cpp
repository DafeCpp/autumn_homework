#include <iostream>

#include "topology_sort.hpp"

int main() {
  int n, m;
  std::cin >> n >> m;

  std::vector<std::vector<int>> graf(n, std::vector<int>());

  for (int i = 0; i < m; ++i) {
    int a, b;
    std::cin >> a >> b;
    graf[b].push_back(a);
  }
  std::vector<int> result = TopologySort(graf);

  for (int i = 0; i < result.size(); ++i) std::cout << result[i] << " ";
}
