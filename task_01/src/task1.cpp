#include "task1.hpp"

#include <vector>

static bool dfs(int v, const std::vector<std::vector<int>>& adj,
                std::vector<int>& color, std::vector<int>& order) {
  color[v] = 1;
  for (size_t i = 0; i < adj[v].size(); ++i) {
    int neighbor = adj[v][i];
    if (color[neighbor] == 0) {
      if (dfs(neighbor, adj, color, order)) return true;
    } else if (color[neighbor] == 1) {
      return true;
    }
  }
  color[v] = 2;
  order.push_back(v);
  return false;
}

std::vector<int> Solve(int n, const std::vector<std::pair<int, int>>& edges) {
  std::vector<std::vector<int>> adj(n);
  for (size_t i = 0; i < edges.size(); ++i) {
    adj[edges[i].first].push_back(edges[i].second);
  }
  std::vector<int> color(n, 0);
  std::vector<int> order;
  for (int i = 0; i < n; ++i) {
    if (color[i] == 0) {
      if (dfs(i, adj, color, order)) return {-1};
    }
  }
  return order;
}
