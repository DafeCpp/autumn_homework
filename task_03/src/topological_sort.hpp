#include <algorithm>
#include <unordered_map>

#include "graph.hpp"

enum class Color {
  White,  // не посещена
  Gray,   // в стеке рекурсии
  Black   // полностью обработана
};

template <typename T>
void DFS(const Graph<T>& graph, std::unordered_map<T, Color>& color,
         std::vector<T>& res, const T& v) {
  color[v] = Color::Gray;

  for (auto u : graph.GetAdjVertices(v)) {
    if (color[u] == Color::White) {
      DFS(graph, color, res, u);
    } else if (color[u] == Color::Gray) {
      throw std::invalid_argument("Graph is not acyclic!");
    }
  }

  color[v] = Color::Black;
  res.push_back(v);
}

std::vector<int> TopologicalSort(const Graph<int>& graph) {
  std::vector<int> res;
  std::unordered_map<int, Color> color;

  for (int v : graph.GetVerticesIds()) color[v] = Color::White;

  try {
    for (int v : graph.GetVerticesIds()) {
      if (color[v] == Color::White) DFS(graph, color, res, v);
    }
  } catch (std::invalid_argument&) {
    return std::vector<int>{-1};
  }

  std::reverse(res.begin(), res.end());
  return res;
}