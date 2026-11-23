#include "solution.hpp"

#include <functional>
#include <queue>

Matrix johnson(const Graph& graph) {
  int n = graph.size();
  std::vector<long long> h;
  if (!graph.get_potentials(h)) {
    return {};
  }
  const auto& adj = graph.get_adjacency();

  Matrix result(n, std::vector<long long>(n, INF));
  using Entry = std::pair<long long, int>;
  for (int s = 0; s < n; ++s) {
    std::vector<long long>& dist = result[s];
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> q;
    dist[s] = 0;
    q.push({0, s});
    while (!q.empty()) {
      auto [d, v] = q.top();
      q.pop();
      if (d > dist[v]) continue;
      for (auto [u, w] : adj[v]) {
        long long nd = d + w + h[v] - h[u];
        if (nd < dist[u]) {
          dist[u] = nd;
          q.push({nd, u});
        }
      }
    }
    for (int v = 0; v < n; ++v) {
      if (dist[v] != INF) {
        dist[v] += h[v] - h[s];
      }
    }
  }
  return result;
}
