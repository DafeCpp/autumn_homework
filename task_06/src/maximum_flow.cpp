#include "maximum_flow.hpp"

void MaxFlow::FindSourceAndDrain() {
  std::vector<bool> has_incoming_edge(n, false);
  std::vector<bool> has_outgoing_edge(n, false);
  for (int i = 0; i < n; ++i) {
    for (auto& [j, fc] : graph[i]) {
      auto& [flow, capacity] = fc;
      if (capacity > 0) {
        has_incoming_edge[j] = true;
        has_outgoing_edge[i] = true;
      }
    }
  }
  for (int i = 0; i < n; ++i) {
    if (!has_incoming_edge[i] && has_outgoing_edge[i]) source = i;
    if (has_incoming_edge[i] && !has_outgoing_edge[i]) drain = i;
  }
}

int MaxFlow::DFS(int u, int addition) {
  if (u == drain) return addition;
  visited[u] = true;
  for (auto& [v, fc] : graph[u]) {
    auto& [flow, capacity] = fc;
    if (visited[v]) continue;
    if (capacity - flow <= 0) continue;
    int current_addition = DFS(v, std::min(addition, capacity - flow));
    if (current_addition > 0) {
      graph[u][v].flow += current_addition;
      graph[v][u].flow -= current_addition;
      return current_addition;
    }
  }
  return 0;
}

void MaxFlow::FordFulkerson() {
  FindSourceAndDrain();
  max_flow = 0;
  while (true) {
    std::fill(visited.begin(), visited.end(), false);
    int flow = DFS(source, 2e6);
    if (flow == 0) break;
    max_flow += flow;
  }
}