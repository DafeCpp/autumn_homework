#include "task2.hpp"

#include <algorithm>
#include <vector>

static void dfs1(int v, const std::vector<std::vector<int>>& adj,
                 std::vector<bool>& visited, std::vector<int>& order) {
  visited[v] = true;
  for (size_t i = 0; i < adj[v].size(); ++i) {
    int neighbor = adj[v][i];
    if (visited[neighbor] == false) {
      dfs1(neighbor, adj, visited, order);
    }
  }
  order.push_back(v);
}

static void dfs2(int v, const std::vector<std::vector<int>>& adj_rev,
                 std::vector<int>& group, int group_id) {
  group[v] = group_id;
  for (size_t i = 0; i < adj_rev[v].size(); ++i) {
    int neighbor = adj_rev[v][i];
    if (group[neighbor] == -1) {
      dfs2(neighbor, adj_rev, group, group_id);
    }
  }
}

int MinEdges(int n, const std::vector<std::pair<int, int>>& edges) {
  if (n <= 1) {
    return 0;
  }

  std::vector<std::vector<int>> adj(n);
  std::vector<std::vector<int>> adj_rev(n);

  for (size_t i = 0; i < edges.size(); ++i) {
    int u = edges[i].first - 1;
    int v = edges[i].second - 1;
    if (u == v) continue;
    adj[u].push_back(v);
    adj_rev[v].push_back(u);
  }

  std::vector<bool> visited(n, false);
  std::vector<int> order;
  for (int i = 0; i < n; ++i) {
    if (!visited[i]) {
      dfs1(i, adj, visited, order);
    }
  }
  std::reverse(order.begin(), order.end());

  std::vector<int> group(n, -1);
  int group_count = 0;
  for (size_t i = 0; i < order.size(); ++i) {
    int v = order[i];
    if (group[v] == -1) {
      dfs2(v, adj_rev, group, group_count);
      group_count++;
    }
  }

  if (group_count == 1) {
    return 0;
  }

  std::vector<int> in_degree(group_count, 0);
  std::vector<int> out_degree(group_count, 0);

  for (int u = 0; u < n; ++u) {
    for (size_t i = 0; i < adj[u].size(); ++i) {
      int v = adj[u][i];
      if (group[u] != group[v]) {
        out_degree[group[u]]++;
        in_degree[group[v]]++;
      }
    }
  }

  int sources = 0;
  int sinks = 0;
  for (int i = 0; i < group_count; ++i) {
    if (in_degree[i] == 0) {
      sources++;
    }
    if (out_degree[i] == 0) {
      sinks++;
    }
  }

  return std::max(sources, sinks);
}
