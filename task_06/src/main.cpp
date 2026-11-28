#include <iostream>
#include <vector>

#include "max_flow_solver.hpp"

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n, m;
  if (!(std::cin >> n >> m)) return 0;

  MaxFlowSolver solver(n);

  std::vector<int> in_degree(n + 1, 0);
  std::vector<int> out_degree(n + 1, 0);

  for (int i = 0; i < m; ++i) {
    int u, v;
    long long c;
    std::cin >> u >> v >> c;
    solver.addEdge(u, v, c);

    out_degree[u]++;
    in_degree[v]++;
  }

  int source = 1;
  int sink = n;

  for (int i = 1; i <= n; ++i) {
    if (in_degree[i] == 0) {
      source = i;
    }
    if (out_degree[i] == 0) {
      sink = i;
    }
  }

  std::cout << solver.maxFlow(source, sink) << "\n";
  return 0;
}