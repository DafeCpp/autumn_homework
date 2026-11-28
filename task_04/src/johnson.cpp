#include "johnson.hpp"

#include <queue>
#include <utility>

void Graph::AddEdge(int u, int v, long long w) { adj[u].push_back({v, w}); }

std::vector<long long> BellmanFord(const Graph& g) {
  int n = g.n;
  std::vector<long long> h(n + 1, 0);

  for (int i = 0; i < n; ++i) {
    bool updated = false;
    for (int u = 1; u <= n; ++u) {
      if (h[u] == kInf) continue;
      for (const auto& edge : g.adj[u]) {
        if (h[u] + edge.weight < h[edge.to]) {
          h[edge.to] = h[u] + edge.weight;
          updated = true;
        }
      }
    }
    if (!updated) break;
  }

  for (int u = 1; u <= n; ++u) {
    if (h[u] == kInf) continue;
    for (const auto& edge : g.adj[u]) {
      if (h[u] + edge.weight < h[edge.to]) {
        return {};
      }
    }
  }

  return h;
}

std::vector<long long> Dijkstra(const Graph& g, int start,
                                const std::vector<long long>& h) {
  int n = g.n;
  std::vector<long long> dist(n + 1, kInf);
  dist[start] = 0;

  using State = std::pair<long long, int>;
  std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
  pq.push({0, start});

  while (!pq.empty()) {
    auto [d, u] = pq.top();
    pq.pop();

    if (d > dist[u]) continue;

    for (const auto& edge : g.adj[u]) {
      long long w = edge.weight + h[u] - h[edge.to];
      if (dist[u] != kInf && dist[u] + w < dist[edge.to]) {
        dist[edge.to] = dist[u] + w;
        pq.push({dist[edge.to], edge.to});
      }
    }
  }

  return dist;
}

std::vector<std::vector<long long>> Johnson(const Graph& g,
                                            bool& has_negative_cycle) {
  int n = g.n;
  std::vector<std::vector<long long>> dist(n + 1,
                                           std::vector<long long>(n + 1, kInf));

  std::vector<long long> h = BellmanFord(g);
  if (h.empty()) {
    has_negative_cycle = true;
    return dist;
  }

  has_negative_cycle = false;

  for (int i = 1; i <= n; ++i) {
    std::vector<long long> d = Dijkstra(g, i, h);
    for (int j = 1; j <= n; ++j) {
      if (d[j] != kInf) {
        dist[i][j] = d[j] - h[i] + h[j];
      }
    }
  }

  return dist;
}