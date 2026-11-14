#include <algorithm>
#include <iostream>
#include <vector>

struct NetworkResult {
  std::vector<int> cut_vertices;
  std::vector<std::pair<int, int>> bridges;
};

class NetworkAnalyzer {
  int n;
  std::vector<std::vector<int>> adj;

  int timer;
  std::vector<int> tin, low;
  std::vector<bool> is_cut;
  std::vector<std::pair<int, int>> bridges;

  void dfs(int u, int p) {
    tin[u] = low[u] = timer++;
    int children = 0;

    for (int v : adj[u]) {
      if (v == p) continue;

      if (tin[v] != -1) {
        low[u] = std::min(low[u], tin[v]);
      } else {
        children++;
        dfs(v, u);
        low[u] = std::min(low[u], low[v]);

        if (low[v] > tin[u]) {
          int u_node = u;
          int v_node = v;
          if (u_node > v_node) std::swap(u_node, v_node);
          bridges.push_back({u_node, v_node});
        }

        if (low[v] >= tin[u] && p != -1) {
          is_cut[u] = true;
        }
      }
    }

    if (p == -1 && children > 1) {
      is_cut[u] = true;
    }
  }

 public:
  std::vector<std::pair<int, int>> edges;

  NetworkAnalyzer(int n)
      : n(n),
        adj(n + 1),
        timer(0),
        tin(n + 1, -1),
        low(n + 1, -1),
        is_cut(n + 1, false) {}

  void addEdge(int u, int v, int id) { edges.push_back({u, v}); }

  NetworkResult solve() {
    std::vector<std::pair<int, int>> unique_edges;
    for (auto& e : edges) {
      int u = e.first;
      int v = e.second;
      if (u > v) std::swap(u, v);
      if (u != v) {
        unique_edges.push_back({u, v});
      }
    }

    std::sort(unique_edges.begin(), unique_edges.end());
    unique_edges.erase(std::unique(unique_edges.begin(), unique_edges.end()),
                       unique_edges.end());

    for (auto& e : unique_edges) {
      adj[e.first].push_back(e.second);
      adj[e.second].push_back(e.first);
    }

    for (int i = 1; i <= n; ++i) {
      if (tin[i] == -1) {
        dfs(i, -1);
      }
    }

    NetworkResult res;
    for (int i = 1; i <= n; ++i) {
      if (is_cut[i]) {
        res.cut_vertices.push_back(i);
      }
    }
    std::sort(res.cut_vertices.begin(), res.cut_vertices.end());

    std::sort(bridges.begin(), bridges.end());
    res.bridges = bridges;

    return res;
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  int n, m;
  if (!(std::cin >> n >> m)) return 0;

  NetworkAnalyzer analyzer(n);
  for (int i = 0; i < m; ++i) {
    int u, v;
    std::cin >> u >> v;
    analyzer.addEdge(u, v, i);
  }

  NetworkResult result = analyzer.solve();

  std::cout << result.cut_vertices.size() << "\n";
  if (result.cut_vertices.empty()) {
    std::cout << "-\n";
  } else {
    for (size_t i = 0; i < result.cut_vertices.size(); ++i) {
      std::cout << result.cut_vertices[i]
                << (i + 1 == result.cut_vertices.size() ? "" : " ");
    }
    std::cout << "\n";
  }

  std::cout << result.bridges.size() << "\n";
  if (result.bridges.empty()) {
    std::cout << "-\n";
  } else {
    for (size_t i = 0; i < result.bridges.size(); ++i) {
      std::cout << result.bridges[i].first << " " << result.bridges[i].second;
      if (i + 1 < result.bridges.size()) {
        std::cout << "; ";
      }
    }
    std::cout << "\n";
  }

  return 0;
}