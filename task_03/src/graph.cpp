#include "graph.hpp"

#include <algorithm>

Graph::Graph() {
  int n, m;
  std::cin >> n >> m;
  graph.resize(n);
  for (int i = 0; i < m; ++i) {
    int a, b;
    std::cin >> a >> b;
    if (a == b) continue;
    graph[a - 1].push_back(b - 1);
    graph[b - 1].push_back(a - 1);
  }
  cnt_vertex = n;
}

Graph::Graph(int n, std::vector<std::pair<int, int> > edges) {
  graph.resize(n);
  for (auto [a, b] : edges) {
    if (a == b) continue;
    graph[a].push_back(b);
    graph[b].push_back(a);
  }
  cnt_vertex = n;
}

void Graph::dfs(int start, int start_parent) {
  std::vector<int> stack = {start};
  int root_children = 0;
  parent[start] = start_parent;
  tin[start] = low[start] = timer++;
  while (!stack.empty()) {
    int v = stack.back();
    if (next_edge[v] < graph[v].size()) {
      int u = graph[v][next_edge[v]];
      ++next_edge[v];
      if (u == parent[v]) continue;
      if (tin[u] != -1) {
        low[v] = std::min(low[v], tin[u]);
      } else {
        if (v == start) ++root_children;
        parent[u] = v;
        tin[u] = low[u] = timer++;
        stack.push_back(u);
      }
      continue;
    }
    stack.pop_back();
    if (v == start) break;
    int p = parent[v];
    low[p] = std::min(low[p], low[v]);
    if (parent[p] != -1 && low[v] >= tin[p]) is_cut[p] = true;
    if (low[v] > tin[p]) bridges.push_back({std::min(p, v), std::max(p, v)});
  }
  if (start_parent == -1 && root_children > 1) is_cut[start] = true;
}

void Graph::run() {
  parent.assign(cnt_vertex, -1);
  next_edge.assign(cnt_vertex, 0);
  is_cut.assign(cnt_vertex, false);
  tin.assign(cnt_vertex, -1);
  low.assign(cnt_vertex, 0);
  bridges.clear();
  timer = 0;
  for (int i = 0; i < cnt_vertex; ++i) {
    if (tin[i] == -1) {
      dfs(i, -1);
    }
  }
}

std::vector<int> Graph::get_cut_vertices() {
  run();
  std::vector<int> result;
  for (int i = 0; i < cnt_vertex; ++i) {
    if (is_cut[i]) result.push_back(i);
  }
  return result;
}

std::vector<std::pair<int, int> > Graph::get_bridges() {
  run();
  std::vector<std::pair<int, int> > result = bridges;
  std::sort(result.begin(), result.end());
  return result;
}
