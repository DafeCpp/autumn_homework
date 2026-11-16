#include "task6.hpp"

#include <algorithm>
#include <queue>
#include <vector>

namespace {

const int INF = 1e9;

int bfs(int scr, int dst, const std::vector<std::vector<int>>& adj,
        const std::vector<std::vector<int>>& capacity,
        std::vector<int>& parent) {
  std::fill(parent.begin(), parent.end(), -1);
  parent[scr] = scr;

  std::vector<int> flow(adj.size(), 0);
  flow[scr] = INF;

  std::queue<int> q;
  q.push(scr);

  while (!q.empty()) {
    int cur = q.front();
    q.pop();

    if (cur == dst) return flow[dst];

    for (size_t i = 0; i < adj[cur].size(); ++i) {
      int next = adj[cur][i];

      if (parent[next] == -1 && capacity[cur][next] > 0) {
        parent[next] = cur;

        flow[next] = std::min(flow[cur], capacity[cur][next]);
        q.push(next);
      }
    }
  }
  return 0;
}

}  // namespace

Res Solve(int n, int m, const std::vector<Edge>& edges) {
  std::vector<int> in_degree(n, 0);
  std::vector<int> out_degree(n, 0);

  for (size_t i = 0; i < edges.size(); ++i) {
    int u = edges[i].u - 1;
    int v = edges[i].v - 1;

    if (u == v) continue;

    out_degree[u]++;
    in_degree[v]++;
  }
  int scr = 0;      // исток
  int dst = n - 1;  // сток

  for (int i = 0; i < n; ++i) {
    if (in_degree[i] == 0 and out_degree[i] > 0) {
      scr = i;
      break;
    }
  }

  for (int i = 0; i < n; ++i) {
    if (in_degree[i] > 0 and out_degree[i] == 0) {
      dst = i;
      break;
    }
  }

  std::vector<std::vector<int>> capacity(
      n, std::vector<int>(
             n, 0));  // вместимость труб, строки - откуда, столбцы - куда
  std::vector<std::vector<int>> adj(n);  // список соседей

  for (size_t i = 0; i < edges.size(); ++i) {
    int u = edges[i].u - 1;
    int v = edges[i].v - 1;
    int c = edges[i].w;

    if (u == v) continue;

    if (capacity[u][v] == 0 and capacity[v][u] == 0) {
      adj[u].push_back(v);
      adj[v].push_back(u);
    }
    capacity[u][v] += c;
  }

  int total_flow = 0;
  std::vector<int> parent(n);
  int new_flow = 0;

  while (true) {
    new_flow = bfs(scr, dst, adj, capacity, parent);

    if (new_flow == 0) break;

    total_flow += new_flow;
    int cur = dst;
    while (cur != scr) {
      int prev = parent[cur];
      capacity[prev][cur] -= new_flow;
      capacity[cur][prev] += new_flow;

      cur = prev;
    }
  }
  return {total_flow};
}
