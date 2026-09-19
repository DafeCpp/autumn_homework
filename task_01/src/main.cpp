#include <iostream>
#include <vector>

#include "dfs.hpp"

int main() {
  int n, m;
  std::cin >> n >> m;

  std::vector<std::vector<int>> components(n, std::vector<int>());

  GraphTrace trace;

  // Граф ориентированный, вершины нумеруются от 0.
  trace.Graph(n, true, 0);

  for (int i = 0; i < m; i++) {
    int vertex1, vertex2;
    std::cin >> vertex1 >> vertex2;

    components[vertex1].push_back(vertex2);

    trace.AddEdge(i, vertex1, vertex2);
  }

  std::vector<Color> color(n, Color::kWhite);
  std::vector<int> sorted_components;
  std::vector<int> parent(n, -1);

  for (int i = 0; i < n; i++) {
    if (color[i] == Color::kWhite) {
      dfs(i, color, components, parent, sorted_components, trace);
    }
  }

  for (int i = static_cast<int>(sorted_components.size()) - 1; i >= 0; i--) {
    std::cout << sorted_components[i] << " ";
  }

  trace.Finish();

  return 0;
}