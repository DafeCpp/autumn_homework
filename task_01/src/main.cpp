#include <graphs.hpp>
#include <iostream>
#include <vector>

int main() {
  try {
    int n, m;
    if (!(std::cin >> n >> m))
      return 0;

    Graph g(n);
    for (int i = 0; i < m; ++i) {
      int a, b;
      std::cin >> a >> b;
      g.AddEdge(a, b);
    }

    if (auto order = g.TopologicalSort()) {
      for (size_t i = 0; i < order->size(); ++i) {
        std::cout << (*order)[i] << (i + 1 < order->size() ? " " : "");
      }
      std::cout << "\n";
    } else {
      std::cout << -1 << "\n";
    }
  } catch (const std::exception &e) {

    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
