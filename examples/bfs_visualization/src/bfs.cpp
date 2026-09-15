#include "bfs.hpp"

#include <queue>
#include <stdexcept>
#include <string>
#include <utility>

#include "graph_trace.hpp"

namespace bfs_example {
std::vector<int> Bfs(int n, const std::vector<Edge>& edges,
                     graph_trace::Recorder* trace) {
  if (n <= 0) throw std::invalid_argument("The graph must have a vertex 1");
  for (const auto& edge : edges) {
    if (edge.from < 1 || edge.from > n || edge.to < 1 || edge.to > n) {
      throw std::invalid_argument("Edge endpoint is outside 1..n");
    }
  }
  std::vector<std::vector<std::pair<int, int>>> graph(n + 1);
  if (trace) trace->graph(n);
  for (std::size_t id = 0; id < edges.size(); ++id) {
    auto [u, v] = edges[id];
    graph[u].push_back({v, static_cast<int>(id)});
    graph[v].push_back({u, static_cast<int>(id)});
    if (trace) trace->add_edge(static_cast<int>(id), u, v);
  }
  std::vector<int> distance(n + 1, -1);
  std::queue<int> queue;
  distance[1] = 0;
  queue.push(1);
  if (trace) {
    trace->node(1, "queued");
    trace->value(1, "distance", 0);
    trace->step("Стартовая вершина добавлена в очередь");
  }
  while (!queue.empty()) {
    int v = queue.front();
    queue.pop();
    if (trace) trace->node(v, "active");
    for (auto [to, edge_id] : graph[v]) {
      if (distance[to] != -1) continue;
      distance[to] = distance[v] + 1;
      queue.push(to);
      if (trace) {
        trace->node(to, "queued");
        trace->value(to, "distance", distance[to]);
        trace->edge(edge_id, "tree");
      }
    }
    if (trace) {
      trace->step("Обработали соседей вершины " + std::to_string(v));
      trace->node(v, "done");
    }
  }
  if (trace) {
    trace->step("Поиск завершён; непосещённые вершины недостижимы");
  }
  return distance;
}
}  // namespace bfs_example
