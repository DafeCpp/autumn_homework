#include "graphs.hpp"

#include <algorithm>
#include <stack>
#include <stdexcept>
#include <utility>

Graph::Graph(int vertices) : num_vertices_(vertices), adj_list_(vertices) {}

void Graph::AddEdge(int u, int v) {
  if (u < 0 || u >= num_vertices_ || v < 0 || v >= num_vertices_) {
    throw std::out_of_range("Vertex index out of range");
  }
  adj_list_[u].push_back(v);
}

std::optional<std::vector<int>> Graph::TopologicalSort() const {
  std::vector<Color> color(num_vertices_, Color::White);
  std::vector<int> result;
  result.reserve(num_vertices_);

  std::stack<std::pair<int, int>> st;

  for (int start = 0; start < num_vertices_; ++start) {
    if (color[start] != Color::White) continue;

    st.push({start, 0});
    color[start] = Color::Gray;
    while (!st.empty()) {
      auto& top = st.top();
      int u = top.first;
      int& idx = top.second;

      bool pushed = false;
      while (idx < static_cast<int>(adj_list_[u].size())) {
        int v = adj_list_[u][idx];
        idx++;

        if (color[v] == Color::Gray) {
          return std::nullopt;
        }
        if (color[v] == Color::White) {
          color[v] = Color::Gray;
          st.push({v, 0});
          pushed = true;
          break;
        }
      }
      if (!pushed) {
        color[u] = Color::Black;
        result.push_back(u);
        st.pop();
      }
    }
  }

  std::reverse(result.begin(), result.end());

  return result;
}