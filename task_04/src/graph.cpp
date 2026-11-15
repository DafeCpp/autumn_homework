#include <cmath>
#include <graph.hpp>
#include <optional>
#include <queue>

void Graph::DistancesReconstruction(const int& start,
                                    std::vector<double>& distances) {
  for (int i = 0; i < distances.size(); ++i) {
    if (distances[i] != INFINITY)
      distances[i] += (potentials[i] - potentials[start]);
  }
}

std::optional<std::vector<double>> Graph::BellmanFord() {
  std::vector<double> distances(n, 0);

  for (int k = 1; k < n; ++k) {
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
  std::vector<double> distances(n, INFINITY);
  distances[start] = 0;

  std::priority_queue<std::pair<double, int>,
                      std::vector<std::pair<double, int>>,
                      std::greater<std::pair<double, int>>>
      heap;

  heap.push({0, start});
  while (!heap.empty()) {
    auto [dist_u, u] = heap.top();
    heap.pop();

    if (distances[u] != dist_u) continue;
    for (auto [v, weight] : reweighted_graph[u]) {
      if (dist_u + weight < distances[v]) {
        distances[v] = dist_u + weight;
        heap.push({distances[v], v});
      }
    }
  }

  DistancesReconstruction(start, distances);
  return distances;
}

void Graph::Reweigh() {
  for (auto [u, v, weight] : edges) {
    double new_weight = weight + potentials[u] - potentials[v];
    reweighted_graph[u].push_back({v, new_weight});
  }
}

std::optional<std::vector<std::vector<double>>> Graph::Johnson() {
  std::optional<std::vector<double>> potentials_ = BellmanFord();
  if (!potentials_) return std::nullopt;
  potentials = *potentials_;
  Reweigh();

  std::vector<std::vector<double>> result(n);
  for (int u = 0; u < n; ++u) result[u] = Dijkstra(u);
  return result;
}