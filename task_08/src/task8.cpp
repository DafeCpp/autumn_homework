#include "task8.h"

#include <algorithm>
#include <cstddef>

LCA::LCA(int n, const std::vector<std::pair<int, int>>& edges, int root) {
  if (n <= 0) {
    return;
  }

  int size = std::max(n, root) + 1;
  std::vector<std::vector<int>> adj(size);
  for (const auto& edge : edges) {
    adj[edge.first].push_back(edge.second);
    adj[edge.second].push_back(edge.first);
  }

  depth.assign(size, 0);
  first.assign(size, -1);
  euler.reserve(2 * n - 1);

  std::vector<int> parent(size, -1);
  std::vector<std::size_t> edge_idx(size, 0);
  std::vector<int> stack;
  stack.reserve(n);

  parent[root] = root;
  depth[root] = 0;
  stack.push_back(root);

  while (!stack.empty()) {
    int u = stack.back();

    if (first[u] == -1) {
      first[u] = static_cast<int>(euler.size());
    }
    euler.push_back(u);

    while (edge_idx[u] < adj[u].size() && adj[u][edge_idx[u]] == parent[u]) {
      ++edge_idx[u];
    }

    if (edge_idx[u] < adj[u].size()) {
      int v = adj[u][edge_idx[u]++];
      parent[v] = u;
      depth[v] = depth[u] + 1;
      stack.push_back(v);
    } else {
      stack.pop_back();
    }
  }

  int m = static_cast<int>(euler.size());

  log2_table.assign(m + 1, 0);
  for (int i = 2; i <= m; ++i) {
    log2_table[i] = log2_table[i / 2] + 1;
  }

  int max_log = log2_table[m] + 1;
  st.assign(max_log, std::vector<int>(m));
  st[0] = euler;

  for (int k = 1; k < max_log; ++k) {
    int len = 1 << k;
    int half = 1 << (k - 1);
    for (int i = 0; i + len <= m; ++i) {
      int left = st[k - 1][i];
      int right = st[k - 1][i + half];
      st[k][i] = (depth[left] < depth[right]) ? left : right;
    }
  }
}

int LCA::get_lca(int u, int v) const {
  int l = first[u];
  int r = first[v];
  if (l > r) {
    std::swap(l, r);
  }

  int k = log2_table[r - l + 1];
  int left = st[k][l];
  int right = st[k][r - (1 << k) + 1];
  return (depth[left] < depth[right]) ? left : right;
}