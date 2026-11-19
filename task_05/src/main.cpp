#include <iostream>
#include <vector>

#include "mst.hpp"

int main() {
  int n, m, specialVertex, maxDegree;
  std::cin >> n >> m >> specialVertex >> maxDegree;

  std::vector<Edge> edges;
  for (int i = 0; i < m; ++i) {
    int u, v, weight;
    std::cin >> u >> v >> weight;
    if (u != v) {
      edges.push_back({u - 1, v - 1, weight});
    }
  }

  long long result = constrainedMst(n, edges, specialVertex - 1, maxDegree);
  if (result != -1) {
    std::cout << result << '\n';
  } else {
    std::cout << "Невозможно построить остовное дерево с заданным ограничением "
                 "степени\n";
  }
}
