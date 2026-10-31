#include <cmath>
#include <graph.hpp>
#include <optional>

/*
std::optional<std::vector<double>> Graph::BellmanFord(int start) {
  // std::vector<double> distances_ford(graph.size(), INFINITY);
  // std::vector<std::vector<double>> d(graph.size(),
  // std::vector<double>(INFINITY));
  std::vector<double> distances(graph.size(), INFINITY);
  // d[0][start] = 0;
  distances[start] = 0;

  for (int k = 1; k < graph.size(); ++k) {
    bool changed = false;

    for (auto [u, v, weight] : edges) {
      if (distances[u] == INFINITY) continue;
      if (distances[u] + weight < distances[v]) {
        distances[v] = distances[u] + weight;
        changed = true;
      }
    }
    if (!changed) break;
  }

  // Проверка отрицательного цикла
  for (auto [u, v, weight] : edges) {
    if (distances[u] != INFINITY && distances[u] + weight < distances[v]) {
      return std::nullopt;
    }
  }

  return distances;
};
*/

std::optional<std::vector<double>> Graph::BellmanFord() {
  std::vector<double> distances(graph.size(), 0);

  for (int k = 1; k < graph.size(); ++k) {
    bool changed = false;

    for (auto [u, v, weight] : edges) {
      if (distances[u] + weight < distances[v]) {
        distances[v] = distances[u] + weight;
        changed = true;
      }
    }
    if (!changed) break;
  }

  // Проверка отрицательного цикла
  for (auto [u, v, weight] : edges) {
    if (distances[u] + weight < distances[v]) {
      return std::nullopt;
    }
  }

  return distances;
};

std::vector<double> Graph::Dijkstra(int start) {
  std::vector<double> distances(reweighted_graph.size(), INFINITY);
}

void Graph::Reweigh(const std::vector<double>& distances) {
  for (auto [u, v, weight] : edges) {
    double new_weight = weight + distances[v] - distances[u];
    reweighted_graph[u].push_back({v, new_weight});
  }
}

bool Graph::Johnson() {
  std::optional<std::vector<double>> distances = BellmanFord();
  if (!distances) return false;
  Reweigh(*distances);
}