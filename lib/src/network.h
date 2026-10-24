#pragma once

#include <algorithm>
#include <vector>

struct NetworkResult {
  std::vector<int> cut_vertices;
  std::vector<std::pair<int, int>> bridges;
};

class NetworkAnalyzer {
private:
  int n;
  std::vector<std::vector<std::pair<int, int>>> adj;
  std::vector<std::pair<int, int>> edges;
  int timer_counter;
  std::vector<int> tin;
  std::vector<int> low;
  std::vector<bool> isCut;
  std::vector<int> bridges;

  void dfs(int v, int parentEdge) {
    tin[v] = low[v] = timer_counter++;
    int children = 0;

    for (const auto& edge : adj[v]) {
      int to = edge.first;
      int id = edge.second;

      if (id == parentEdge) {
        continue;
      }

      if (tin[to] == -1) {
        children++;
        dfs(to, id);
        low[v] = std::min(low[v], low[to]);

        if (low[to] > tin[v]) {
          bridges.push_back(id);
        }

        if (parentEdge != -1 && low[to] >= tin[v]) {
          isCut[v] = true;
        }
      } else {
        low[v] = std::min(low[v], tin[to]);
      }
    }

    if (parentEdge == -1 && children >= 2) {
      isCut[v] = true;
    }
  }

public:
  explicit NetworkAnalyzer(int n_vertices) : n(n_vertices), timer_counter(0) {
    adj.resize(n + 1);
    tin.assign(n + 1, -1);
    low.assign(n + 1, -1);
    isCut.assign(n + 1, false);
  }

  void addEdge(int u, int v, int id) {
    // Убрали ранний return. Алгоритм DFS сам корректно обработает петлю,
    // а вектор edges сохранит правильную индексацию по id.
    adj[u].push_back({v, id});
    adj[v].push_back({u, id});
    edges.push_back({u, v});
  }

  NetworkResult solve() {
    bridges.clear();
    for (int i = 1; i <= n; ++i) {
      if (tin[i] == -1) {
        dfs(i, -1);
      }
    }

    NetworkResult result;
    for (int i = 1; i <= n; ++i) {
      if (isCut[i]) {
        result.cut_vertices.push_back(i);
      }
    }
    std::sort(result.cut_vertices.begin(), result.cut_vertices.end());

    for (int id : bridges) {
      int u = edges[id].first;
      int v = edges[id].second;
      if (u > v) {
        std::swap(u, v);
      }
      result.bridges.push_back({u, v});
    }
    std::sort(result.bridges.begin(), result.bridges.end());

    return result;
  }
};
