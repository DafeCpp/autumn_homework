#include "topology_sort.hpp"

#include <stack>

enum class Color { kWhite, kGray, kBlack };

struct VertexOnStack {
  // для стэка, чтобы не создавать std::pair
  int vertex;
  int index;
};

bool InnerDFS(int u, const std::vector<std::vector<int>>& graf,
              std::vector<Color>& colors, std::vector<int>& result) {
  colors[u] = Color::kGray;
  std::stack<VertexOnStack> stack;
  stack.push({u, 0});

  while (!stack.empty()) {
    int current = stack.top().vertex;
    int index = stack.top().index;

    if (index < graf[current].size()) {
      int v = graf[current][index];
      stack.top().index = ++index;

      if (colors[v] == Color::kWhite) {
        colors[v] = Color::kGray;
        stack.push({v, 0});
      } else if (colors[v] == Color::kGray)
        return false;
    } else {
      colors[current] = Color::kBlack;
      result.push_back(current);
      stack.pop();
    }
  }
  return true;
}

std::optional<std::vector<int>> DFS(const std::vector<std::vector<int>>& graf) {
  std::vector<Color> colors(graf.size(), Color::kWhite);
  std::vector<int> result;

  for (int u = 0; u < graf.size(); ++u) {
    if (colors[u] == Color::kWhite) {
      if (!InnerDFS(u, graf, colors, result)) return std::nullopt;
    }
  }
  return result;
}

std::optional<std::vector<int>> TopologySort(
    const std::vector<std::vector<int>>& graf) {
  return DFS(graf);
}