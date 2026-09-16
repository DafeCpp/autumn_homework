#include <iostream>

#include "scc.hpp"

int main() {
  int n, m;
  if (!(std::cin >> n >> m) || n < 1 || n > 200000 || m < 0 || m > 200000)
    return 1;
  std::vector<scc_example::Edge> edges(m);
  for (auto& [u, v] : edges)
    if (!(std::cin >> u >> v) || u < 1 || u > n || v < 1 || v > n) return 1;
  graph_trace::Recorder trace;
  auto* recording = trace.IsEnabled() ? &trace : nullptr;
#ifdef SCC_TARJAN
  const auto component = scc_example::Tarjan(n, edges, recording);
#else
  const auto component = scc_example::Kosaraju(n, edges, recording);
#endif
  trace.Finish();
  for (int v = 1; v <= n; ++v) std::cout << (v == 1 ? "" : " ") << component[v];
  std::cout << '\n';
}
