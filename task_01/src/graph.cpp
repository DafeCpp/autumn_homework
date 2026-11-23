#include "graph.hpp"

Graph::Graph() {
  int n, m;
  std::cin >> n >> m;
  graph.resize(n);
  for (int i = 0; i < m; ++i) {
    int a, b;
    std::cin >> a >> b;
    graph[a].push_back(b);
  }
  cnt_vertex = n;
  have_cycle = false;
}

void Graph::dfs(int start) {
  std::vector<int> stack = {start};
  color[start] = 1;
  while (!stack.empty()) {
    int v = stack.back();
    if (next_edge[v] == graph[v].size()) {
      color[v] = 2;
      top_sort.push_back(v);
      stack.pop_back();
      continue;
    }
    int u = graph[v][next_edge[v]];
    ++next_edge[v];
    if (!color[u]) {
      color[u] = 1;
      stack.push_back(u);
    } else if (color[u] == 1) {
      have_cycle = true;
    }
  }
}

std::vector<int> Graph::get_top_sort() {
  color.assign(cnt_vertex, 0);
  next_edge.assign(cnt_vertex, 0);
  top_sort.clear();
  for (int i = 0; i < cnt_vertex; ++i) {
    if (!color[i]) {
      dfs(i);
    }
  }
  if (have_cycle) {
    return std::vector<int>(1, -1);
  }
  return top_sort;
}
