#include <iostream>

#include "max_flow_solver.hpp"

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n, m;
  if (!(std::cin >> n >> m)) return 0;

  MaxFlowSolver solver(n);

  for (int i = 0; i < m; ++i) {
    int u, v;
    long long c;
    std::cin >> u >> v >> c;
    solver.addEdge(u, v, c);
  }

  int source = 0;
  int sink = n - 1;

  std::cout << solver.maxFlow(source, sink) << "\n";
  return 0;
}