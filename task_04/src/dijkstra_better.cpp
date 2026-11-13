#include "dijkstra.hpp"

#include <set>
#include <vector>

std::vector<long long> dijkstra(const std::vector<std::vector<std::pair<int, long long>>>& graph, int start) {
  std::vector<long long> distance(graph.size(), INF);
  distance[start] = 0;

  std::set<std::pair<long long, int>> q;
  q.insert({0, start});

  while (!q.empty()) {
    int v = q.begin()->second;
    q.erase(q.begin());

    for (auto& [to, weight] : graph[v]) {
      if (distance[v] + weight < distance[to]) {
        q.erase({distance[to], to});
        distance[to] = distance[v] + weight;
        q.insert({distance[to], to});
      }
    }
  }

  return distance;
}
