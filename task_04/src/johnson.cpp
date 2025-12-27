#include "johnson.h"

#include <queue>
#include <utility>
#include <vector>
#include <climits>

Johnson::Johnson(int vertex_count, const std::vector<std::vector<int>>& edges)
    : vertex_count_(vertex_count), edges_(edges) {}

std::vector<std::vector<int>> Johnson::Run() {
  std::vector<std::vector<std::pair<int, int>>> original_graph(vertex_count_);
  for (const auto& edge : edges_) {
    original_graph[edge[0]].emplace_back(edge[1], edge[2]);
  }

  std::vector<int> potential(vertex_count_ + 1, 0);
  for (int i = 0; i < vertex_count_; ++i) {
    edges_.push_back({vertex_count_, i, 0});
  }

  if (!BellmanFord(potential)) {
    return {};
  }
  potential.pop_back();

  std::vector<std::vector<std::pair<int, int>>> reweighted_graph(vertex_count_);
  for (const auto& edge : edges_) {
    if (edge[0] == vertex_count_) continue;
    int u = edge[0];
    int v = edge[1];
    int w = edge[2] + potential[u] - potential[v];
    reweighted_graph[u].emplace_back(v, w);
  }

  std::vector<std::vector<int>> distances(vertex_count_);
  for (int u = 0; u < vertex_count_; ++u) {
    std::vector<int> d = Dijkstra(u, reweighted_graph);
    for (int v = 0; v < vertex_count_; ++v) {
      if (d[v] != INT_MAX) {
        d[v] = d[v] - potential[u] + potential[v];
      }
    }
    distances[u] = d;
  }
  return distances;
}

bool Johnson::BellmanFord(std::vector<int>& distances) {
  distances.assign(vertex_count_ + 1, INT_MAX);
  distances[vertex_count_] = 0;

  for (int i = 0; i < vertex_count_; ++i) {
    for (const auto& edge : edges_) {
      int u = edge[0];
      int v = edge[1];
      int w = edge[2];
      if (distances[u] != INT_MAX && distances[u] + w < distances[v]) {
        distances[v] = distances[u] + w;
      }
    }
  }

  for (const auto& edge : edges_) {
    int u = edge[0];
    int v = edge[1];
    int w = edge[2];
    if (distances[u] != INT_MAX && distances[u] + w < distances[v]) {
      return false;
    }
  }
  return true;
}

std::vector<int> Johnson::Dijkstra(int source,
                                   const std::vector<std::vector<std::pair<int, int>>>& graph) {
  std::vector<int> dist(vertex_count_, INT_MAX);
  dist[source] = 0;
  std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,
                      std::greater<std::pair<int, int>>>
      pq;
  pq.emplace(0, source);

  while (!pq.empty()) {
    auto [current_dist, u] = pq.top();
    pq.pop();

    if (current_dist > dist[u]) continue;

    for (const auto& neighbor : graph[u]) {
      int v = neighbor.first;
      int w = neighbor.second;
      if (dist[u] + w < dist[v]) {
        dist[v] = dist[u] + w;
        pq.emplace(dist[v], v);
      }
    }
  }
  return dist;
}