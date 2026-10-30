#include <iostream>

#include "additional_roads.hpp"

int main() {
  int n, m;
  std::cin >> n >> m;

  std::vector<std::vector<int>> graf(n, std::vector<int>());
  for (int i = 0; i < m; ++i) {
    int a, b;
    std::cin >> a >> b;

    graf[a - 1].push_back(b - 1);
  }

  AdditionalRoads new_roads(graf);
  std::cout << new_roads.amount << "\n";
}
