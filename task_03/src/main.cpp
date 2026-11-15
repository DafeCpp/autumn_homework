#include <iostream>

#include "get_critical_elements.hpp"

int main() {
  int n, m;
  std::cin >> n >> m;

  std::vector<std::set<int>> graf(n, std::set<int>());
  for (int i = 0; i < m; ++i) {
    int u, v;
    std::cin >> u >> v;
    if (u == v) continue;
    graf[v - 1].insert(u - 1);
    graf[u - 1].insert(v - 1);
  }

  AdditionalRoads elements = AdditionalRoads(graf);

  std::cout << elements.critical_devices.size() << "\n";
  if (elements.critical_devices.size() == 0)
    std::cout << "-\n";
  else {
    bool first = true;
    for (int device : elements.critical_devices) {
      if (first)
        std::cout << device + 1;
      else
        std::cout << " " << device + 1;
      first = false;
    }
    std::cout << "\n";
  }

  std::cout << elements.critical_wiring.size() << "\n";
  if (elements.critical_wiring.size() == 0)
    std::cout << "-\n";
  else {
    bool first = true;
    for (std::pair<int, int> wiring : elements.critical_wiring) {
      if (first)
        std::cout << wiring.first + 1 << " " << wiring.second + 1;
      else
        std::cout << "; " << wiring.first + 1 << " " << wiring.second + 1;
      first = false;
    }
    std::cout << "\n";
  }
}
