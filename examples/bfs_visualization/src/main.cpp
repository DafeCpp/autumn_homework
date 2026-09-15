#include <iostream>
#include <memory>
#include <vector>

#include "bfs.hpp"
#include "graph_trace.hpp"

int main() {
  int n, m;
  if (!(std::cin >> n >> m) || n < 1 || n > 200000 || m < 0 || m > 200000)
    return 1;
  std::vector<bfs_example::Edge> edges(m);
  for (auto& edge : edges) {
    if (!(std::cin >> edge.from >> edge.to) || edge.from < 1 || edge.from > n ||
        edge.to < 1 || edge.to > n)
      return 1;
  }
  auto trace = std::make_shared<graph_trace::Recorder>();
  const auto distance = bfs_example::Bfs(n, edges, trace);
  trace->finish();
  for (int v = 1; v <= n; ++v) {
    if (v > 1) std::cout << ' ';
    std::cout << distance[v];
  }
  std::cout << '\n';
}
