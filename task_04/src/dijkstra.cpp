#include "dijkstra.hpp"

std::vector<long long> dijkstra_simple(const std::vector<std::vector<std::pair<int, long long>>>& graph, int start) {
  std::vector<long long> distance(graph.size(), INF);
  std::vector<bool> visited(graph.size(), false);
  distance[start] = 0;

  for (int i = 0; i < static_cast<int>(graph.size()); ++i) {
    int nearest = -1;
    for (int v = 0; v < static_cast<int>(graph.size()); ++v) {
      if (!visited[v] && (nearest == -1 || distance[v] < distance[nearest])) {
        nearest = v;
      }
    }
    if (nearest == -1 || distance[nearest] == INF) {
      break;
    }
    visited[nearest] = true;
    for (const auto& [to, weight] : graph[nearest]) {
      if (distance[nearest] + weight < distance[to]) {
        distance[to] = distance[nearest] + weight;
      }
    }
  }
  return distance;
}
