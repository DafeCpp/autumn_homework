#include "bridges_and_articulation_points.hpp"

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
void dfs(int v, int parentEdge, const Graph& graph, std::vector<int>& tin,
         std::vector<int>& low, std::vector<bool>& isCut,
         std::vector<std::pair<int, int>>& bridges, int& timer,
         GraphTrace& trace) {
  tin[v] = low[v] = timer++;
  trace.Node(v, "active");
  trace.Value(v, "tin", tin[v]);
  trace.Value(v, "low", low[v]);
  trace.Step("Вошли в вершину");
  int children = 0;

  for (const Edge& edge : graph[v]) {
    int to = edge.to;
    int id = edge.id;

    if (id == parentEdge) {
      continue;
    }

    if (tin[to] == -1) {
      children++;
      trace.Edge(id, "tree");
      trace.Value(to, "parent", v);
      dfs(to, id, graph, tin, low, isCut, bridges, timer, trace);

      low[v] = std::min(low[v], low[to]);
      trace.Value(v, "low", low[v]);
      trace.Step("Вернулись из дочерней вершины и обновили low");

      if (low[to] > tin[v]) {
        bridges.push_back({std::min(v, to), std::max(v, to)});
        trace.Edge(id, "bridge");
        trace.Step("Нашли мост: low[to] > tin[v]");
      }

      if (parentEdge != -1 && low[to] >= tin[v]) {
        isCut[v] = true;
        trace.Node(v, "cut");
        trace.Step("Нашли точку сочленения: low[to] >= tin[v]");
      }
    } else {
      low[v] = std::min(low[v], tin[to]);

      if (tin[to] < tin[v]) {
        trace.Edge(id, "back");
        trace.Value(v, "low", low[v]);
        trace.Step("Нашли обратное ребро к предку");
      }
    }
  }

  if (parentEdge == -1 && children > 1) {
    isCut[v] = true;
  }
  trace.Node(v, isCut[v] ? "cut" : "done");
  trace.Step("Закончили обработку вершины");
}