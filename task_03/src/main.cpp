#include "topological_sort.h"
#include <iostream>
#include <vector>

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n, m;
  std::cin >> n >> m;

  std::vector<std::pair<int, int>> edges(m);
  for (int i = 0; i < m; ++i) {
    int u, v;
    std::cin >> u >> v;
    edges[i] = {u, v};
  }

  std::vector<int> result;
  bool has_order = TopologicalSorter::Sort(n, edges, result);

  if (!has_order) {
    std::cout << -1;
  } else {
    for (int i = 0; i < n; ++i) {
      std::cout << result[i] << " ";
    }
  }

  return 0;
}
