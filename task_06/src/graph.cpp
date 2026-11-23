#include "graph.hpp"

#include <algorithm>
#include <queue>

Graph::Graph() {
  int n, m;
  std::cin >> n >> m;
  graph.resize(n);
  for (int i = 0; i < m; ++i) {
    int a, b;
    long long c;
    std::cin >> a >> b >> c;
    add_edge(a - 1, b - 1, c);
  }
  cnt_vertex = n;
}

Graph::Graph(int n, std::vector<std::vector<long long> > pipes) {
  graph.resize(n);
  for (const std::vector<long long>& p : pipes) {
    add_edge(p[0], p[1], p[2]);
  }
  cnt_vertex = n;
}

void Graph::add_edge(int a, int b, long long c) {
  graph[a].push_back(edges.size());
  edges.push_back({b, c});
  graph[b].push_back(edges.size());
  edges.push_back({a, 0});
}

int Graph::size() const { return cnt_vertex; }

bool Graph::bfs(int s, int t) {
  level.assign(cnt_vertex, -1);
  level[s] = 0;
  std::queue<int> q;
  q.push(s);
  while (!q.empty()) {
    int v = q.front();
    q.pop();
    for (int id : graph[v]) {
      if (edges[id].cap > 0 && level[edges[id].to] == -1) {
        level[edges[id].to] = level[v] + 1;
        q.push(edges[id].to);
      }
    }
  }
  return level[t] != -1;
}

long long Graph::dfs(int v, int t, long long pushed) {
  if (v == t || pushed == 0) {
    return pushed;
  }
  std::vector<int> path;
  while (v != t) {
    int& i = next[v];
    if (i == (int)graph[v].size()) {
      if (path.empty()) return 0;
      v = edges[path.back() ^ 1].to;
      path.pop_back();
      ++next[v];
      continue;
    }
    int id = graph[v][i];
    int u = edges[id].to;
    if (level[u] == level[v] + 1 && edges[id].cap > 0) {
      path.push_back(id);
      v = u;
    } else {
      ++i;
    }
  }
  for (int id : path) {
    pushed = std::min(pushed, edges[id].cap);
  }
  for (int id : path) {
    edges[id].cap -= pushed;
    edges[id ^ 1].cap += pushed;
  }
  return pushed;
}

long long Graph::get_max_flow() {
  int s = 0, t = cnt_vertex - 1;
  int sources = 0, sinks = 0;
  for (int v = 0; v < cnt_vertex; ++v) {
    bool has_in = false, has_out = false;
    for (int id : graph[v]) {
      if (edges[id].to == v) continue;
      // Odd IDs are the reverse edges of incoming pipes.
      if (id % 2 == 0) {
        has_out = true;
      } else {
        has_in = true;
      }
    }
    if (has_out && !has_in) {
      s = v;
      ++sources;
    }
    if (has_in && !has_out) {
      t = v;
      ++sinks;
    }
  }
  if (sources == 1 && sinks == 1) return get_max_flow(s, t);
  return get_max_flow(0, cnt_vertex - 1);
}

long long Graph::get_max_flow(int s, int t) {
  if (s == t) {
    return 0;
  }
  long long flow = 0;
  while (bfs(s, t)) {
    next.assign(cnt_vertex, 0);
    while (long long pushed = dfs(s, t, 4e18)) {
      flow += pushed;
    }
  }
  return flow;
}
