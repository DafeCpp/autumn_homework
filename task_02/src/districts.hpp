#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <vector>

#include "graph.hpp"

template <typename T>
void DFS1(const Graph<T>& graph, const T& v, std::map<T, bool>& visited,
          std::stack<T>& order) {
  visited[v] = true;
  for (auto u : graph.GetAdjVertices(v)) {
    if (!visited[u]) DFS1(graph, u, visited, order);
  }
  order.push(v);
}

template <typename T>
void DFS2(const Graph<T>& graphT, const T& v, std::map<T, int>& comp, int cid) {
  comp[v] = cid;
  for (auto u : graphT.GetAdjVertices(v)) {
    if (comp[u] == -1) DFS2(graphT, u, comp, cid);
  }
}

template <typename T>
int MinEdgesToMakeOneDistrict(Graph<T> graph) {
  std::vector<T> vertices = graph.GetVerticesIds();

  // 1. Первый DFS
  std::map<T, bool> visited;
  for (auto v : vertices) visited[v] = false;
  std::stack<T> order;
  for (auto v : vertices)
    if (!visited[v]) DFS1(graph, v, visited, order);

  // 2. Строим транспонированный граф
  Graph<T> graphT(true);

  // Добавляем все вершины, чтобы DFS2 не падал
  for (auto v : vertices) graphT.AddVertex(v);

  for (auto v : vertices) {
    for (auto u : graph.GetAdjVertices(v)) {
      graphT.AddEdge(u, v);  // переворачиваем ребро
    }
  }

  // 3. Второй DFS для SCC
  std::map<T, int> comp;
  for (auto v : vertices) comp[v] = -1;
  int cid = 0;
  while (!order.empty()) {
    T v = order.top();
    order.pop();
    if (comp[v] == -1) {
      DFS2(graphT, v, comp, cid);
      cid++;
    }
  }

  if (cid == 1) return 0;  // Уже одна SCC

  // 4. Конденсация DAG SCC
  std::vector<int> indeg(cid, 0), outdeg(cid, 0);
  std::set<std::pair<int, int>> seen;
  for (auto v : vertices) {
    for (auto u : graph.GetAdjVertices(v)) {
      int cv = comp[v], cu = comp[u];
      if (cv != cu && seen.insert({cv, cu}).second) {
        outdeg[cv]++;
        indeg[cu]++;
      }
    }
  }

  int sources = std::count(indeg.begin(), indeg.end(), 0);
  int sinks = std::count(outdeg.begin(), outdeg.end(), 0);

  return std::max(sources, sinks);
}
