#include "task4.hpp"


#include <queue>
#include <vector>

const long long INF = 1e18;

static bool BellmanFord(int n, const std::vector<Edge>& edges,
                        std::vector<long long>& h) {
  h.assign(n + 1, 0);

  std::vector<Edge> extended_edges = edges;
  for (int i = 0; i < n; ++i) {
    extended_edges.push_back({n, i, 0});
  }

  for (int i = 0; i < n; ++i) {
    for (size_t j = 0; j < extended_edges.size(); ++j) {
      int u = extended_edges[j].from;
      int v = extended_edges[j].to;
      long long w = extended_edges[j].weight;
      if (h[u] != INF && h[u] + w < h[v]) {
        h[v] = h[u] + w;
      }
    }
  }

  for (size_t j = 0; j < extended_edges.size(); ++j) {
    int u = extended_edges[j].from;
    int v = extended_edges[j].to;
    long long w = extended_edges[j].weight;
    if (h[u] != INF && h[u] + w < h[v]) {
      return false;
    }
  }

  return true;
}

static std::vector<long long> Dijkstra(
    int n, int src,
    const std::vector<std::vector<std::pair<int, long long>>>& adj) {
  std::vector<long long> dist(n, INF);
  dist[src] = 0;

  std::priority_queue<std::pair<long long, int>> pq;
  pq.push({0, src});

  while (!pq.empty()) {
    long long d = -pq.top().first;
    int u = pq.top().second;
    pq.pop();

    if (d > dist[u]) continue;

    for (size_t i = 0; i < adj[u].size(); ++i) {
      int v = adj[u][i].first;
      long long w = adj[u][i].second;
      if (dist[u] + w < dist[v]) {
        dist[v] = dist[u] + w;
        pq.push({-dist[v], v});
      }
    }
  }

  return dist;
}

std::vector<std::vector<long long>> Johnson(int n,
                                            const std::vector<Edge>& edges) {
  std::vector<long long> h(n + 1, INF);

  if (!BellmanFord(n, edges, h)) {
    return {};
  }

  std::vector<std::vector<std::pair<int, long long>>> adj(n);
  for (size_t i = 0; i < edges.size(); ++i) {
    int u = edges[i].from;
    int v = edges[i].to;
    long long w = edges[i].weight;
    long long new_w = w + h[u] - h[v];
    adj[u].push_back({v, new_w});
  }

  std::vector<std::vector<long long>> distance_matrix(n);
  for (int i = 0; i < n; ++i) {
    std::vector<long long> d = Dijkstra(n, i, adj);

    for (int j = 0; j < n; ++j) {
      if (d[j] != INF) {
        d[j] = d[j] - h[i] + h[j];
      }
    }
    distance_matrix[i] = d;
  }

  return distance_matrix;
}
