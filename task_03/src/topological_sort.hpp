#include <algorithm>
#include <unordered_map>

#include "graph.hpp"

template <typename T>
void DFS(Graph<T>& graph, std::unordered_map<T, bool>& visited,
         std::vector<T>& res, const T& curr_vertex) {
  if (visited[curr_vertex])
    throw std::invalid_argument("Graph is not acyclic!");

  visited[curr_vertex] = true;

  for (auto adj_vertex : graph.GetAdjVertices(curr_vertex))
    if (graph.ContainsVertex(adj_vertex)) DFS(graph, visited, res, adj_vertex);

  graph.DeleteVertex(curr_vertex);
  res.push_back(curr_vertex);
}

std::vector<int> TopologicalSort(Graph<int> graph) {
  std::vector<int> reversed_res;
  std::unordered_map<int, bool> visited;
  try {
    while (graph.GetVerticesCount())
      DFS(graph, visited, reversed_res, graph.GetVerticesIds()[0]);
  } catch (std::invalid_argument) {
    return std::vector<int>{-1};
  }

  reverse(reversed_res.begin(), reversed_res.end());

  return reversed_res;
}
