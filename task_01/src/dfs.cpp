#include "dfs.hpp"

#include <cstdlib>
#include <iostream>

void dfs(int vertex, std::vector<Color>& color,
         const std::vector<std::vector<int>>& graph, std::vector<int>& parent,
         std::vector<int>& sorted_components, GraphTrace& trace) {
  color[vertex] = Color::kGray;

  trace.Node(vertex, "active");
  trace.Step("Начали обрабатывать вершину");

  for (int neighbor : graph[vertex]) {
    if (color[neighbor] == Color::kGray) {
      trace.Step("Обнаружен цикл");
      trace.Finish();

      std::cout << -1;
      std::exit(0);
    }

    if (color[neighbor] == Color::kWhite) {
      parent[neighbor] = vertex;

      dfs(neighbor, color, graph, parent, sorted_components, trace);
    }
  }

  sorted_components.push_back(vertex);
  color[vertex] = Color::kBlack;

  trace.Node(vertex, "done");
  trace.Step("Завершили обработку вершины");
}