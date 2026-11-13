#include "johnson.hpp"

#include <string>

#include "bellman.hpp"
#include "dijkstra.hpp"
#include "graph_trace.hpp"

bool johnson(int n, const std::vector<Edge>& edges, DistanceMatrix& result) {
  result.clear();
  graph_trace::Recorder trace;
  trace.Graph(n, true, 0);
  for (int id = 0; id < edges.size(); ++id) {
    trace.AddEdge(id, edges[id].a, edges[id].b);
  }
  trace.Step("Исходный граф построен");

  std::vector<Edge> allEdges = edges;
  for (int v = 0; v < n; ++v) {
    allEdges.push_back({n, v, 0});
  }

  std::vector<long long> h;
  if (!bellman(n + 1, allEdges, n, h)) {
    trace.Step("Беллман — Форд обнаружил отрицательный цикл");
    trace.Finish();
    return false;
  }

  for (int v = 0; v < n; ++v) {
    trace.Value(v, "h", h[v]);
  }
  trace.Step("Вычислены потенциалы h с помощью Беллмана — Форда");

  Graph graph(n);
  for (const auto& edge : edges) {
    long long newWeight = edge.weight + h[edge.a] - h[edge.b];
    graph[edge.a].push_back({edge.b, newWeight});
  }

  trace.Step("Веса пересчитаны: w + h[from] - h[to]; запускаем Дейкстру");
  result.resize(n);
  for (int from = 0; from < n; ++from) {
    for (int v = 0; v < n; ++v) {
      trace.Node(v, "idle");
    }
    trace.Node(from, "active");
    result[from] = dijkstra(graph, from);
    for (int to = 0; to < n; ++to) {
      if (result[from][to] != INF) {
        result[from][to] = result[from][to] - h[from] + h[to];
        trace.Value(to, "from_" + std::to_string(from), result[from][to]);
        trace.Node(to, "done");
      }
    }
  }
  trace.Step("Алгоритм Джонсона завершён");
  trace.Finish();
  return true;
}
