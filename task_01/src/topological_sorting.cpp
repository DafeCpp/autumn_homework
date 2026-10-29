#include "topological_sorting.hpp"

#include <cstdlib>
#include <iostream>
#include <vector>

void dfs(int vertex, std::vector<Color>& color,
         const std::vector<std::vector<int>>& graph, std::vector<int>& order,
         GraphTrace& trace) {
  color[vertex] = Color::GRAY;

  trace.Node(vertex, "active");
  trace.Step("Начали обрабатывать вершину");

  for (int neighbor : graph[vertex]) {
    if (color[neighbor] == Color::GRAY) {
      trace.Step("Обнаружен цикл");
      trace.Finish();

      std::cout << -1;
      std::exit(0);
    }

    if (color[neighbor] == Color::WHITE) {
      dfs(neighbor, color, graph, order, trace);
    }
  }

  color[vertex] = Color::BLACK;
  order.push_back(vertex);

  trace.Node(vertex, "done");
  trace.Step("Завершили обработку вершины");
}