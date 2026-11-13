#include "bellman.hpp"

bool bellman(int vertexCount, const std::vector<Edge>& edges, int start,
             std::vector<long long>& distance) {
  distance.assign(vertexCount, INF);
  distance[start] = 0;

  for (int i = 0; i < vertexCount - 1; ++i) {
    for (auto& [a, b, weight] : edges) {
      if (distance[a] < INF && distance[a] + weight < distance[b]) {
        distance[b] = distance[a] + weight;
      }
    }
  }

  for (const auto& [a, b, weight] : edges) {
    if (distance[a] < INF && distance[a] + weight < distance[b]) {
      return false;  // существует отрицательный цикл из вершины start
    }
  }

  return true;
}
