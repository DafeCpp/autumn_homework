#include "graph.hpp"

#include <algorithm>

Graph::Graph() {
  int n, m;
  std::cin >> n >> m;
  graph.resize(n);
  for (int i = 0; i < m; ++i) {
    int a, b;
    std::cin >> a >> b;
    graph[a - 1].push_back(b - 1);
  }
  cnt_vertex = n;
}

const std::vector<std::vector<int> >& Graph::get_adjacency() const {
  return graph;
}

int Graph::size() const { return cnt_vertex; }

void Graph::dfs(int start) {
  std::vector<int> stack = {start};
  while (!stack.empty()) {
    int v = stack.back();
    if (tin[v] == -1) {
      tin[v] = low[v] = timer++;
      scc_stack.push_back(v);
      on_stack[v] = true;
    }
    if (next_edge[v] < graph[v].size()) {
      int u = graph[v][next_edge[v]];
      ++next_edge[v];
      if (tin[u] == -1) {
        stack.push_back(u);
      } else if (on_stack[u]) {
        low[v] = std::min(low[v], tin[u]);
      }
      continue;
    }
    if (low[v] == tin[v]) {
      int x;
      do {
        x = scc_stack.back();
        scc_stack.pop_back();
        on_stack[x] = false;
        comp[x] = cnt_comp;
      } while (x != v);
      ++cnt_comp;
    }
    stack.pop_back();
    if (!stack.empty()) {
      int parent = stack.back();
      low[parent] = std::min(low[parent], low[v]);
    }
  }
}

int Graph::get_scc(std::vector<int>& result) {
  tin.assign(cnt_vertex, -1);
  next_edge.assign(cnt_vertex, 0);
  low.assign(cnt_vertex, 0);
  comp.assign(cnt_vertex, -1);
  on_stack.assign(cnt_vertex, false);
  scc_stack.clear();
  timer = 0;
  cnt_comp = 0;
  for (int i = 0; i < cnt_vertex; ++i) {
    if (tin[i] == -1) {
      dfs(i);
    }
  }
  result = comp;
  return cnt_comp;
}
