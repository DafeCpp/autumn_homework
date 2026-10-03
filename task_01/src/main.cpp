#include <graphs.hpp>
#include <iostream>
#include <vector>

int main() {
  int n, m;
  if (!(std::cin >> n >> m)) return 0;

  Graph g(n);
  for (int i = 0; i < m; ++i) {
    int a, b;
    std::cin >> a >> b;
    g.addEdge(a, b);
  }

  std::vector<int> order;
  if (g.topologicalSort(order)) {
    for (int i = 0; i < n; ++i) {
      std::cout << order[i];
      if (i + 1 < n) std::cout << " ";
    }
    std::cout << "\n";
  } else {
    std::cout << -1 << "\n";
  }

  return 0;
}