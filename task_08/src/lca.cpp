#include "lca.h"

#include <algorithm>
#include <cmath>

LCA::LCA(int n, const std::vector<std::vector<int>>& graph)
    : n_(n), graph_(graph) {
  log_n_ = std::ceil(std::log2(n_)) + 1;
  up_.assign(n_, std::vector<int>(log_n_));
  tin_.assign(n_, 0);
  tout_.assign(n_, 0);
  depth_.assign(n_, 0);
  timer_ = 0;
}

void LCA::Preprocess(int root) {
  Dfs(root, root, 0);
}

void LCA::Dfs(int u, int parent, int depth) {
  tin_[u] = timer_++;
  depth_[u] = depth;
  up_[u][0] = parent;
  
  for (int i = 1; i < log_n_; ++i) {
    up_[u][i] = up_[up_[u][i - 1]][i - 1];
  }
  
  for (int v : graph_[u]) {
    if (v != parent) {
      Dfs(v, u, depth + 1);
    }
  }
  
  tout_[u] = timer_++;
}

bool IsAncestor(int ancestor, int descendant,
                const std::vector<int>& tin,
                const std::vector<int>& tout) {
  return tin[ancestor] <= tin[descendant] &&
         tout[ancestor] >= tout[descendant];
}

int LCA::Query(int u, int v) const {
  if (IsAncestor(u, v, tin_, tout_)) return u;
  if (IsAncestor(v, u, tin_, tout_)) return v;
  
  for (int i = log_n_ - 1; i >= 0; --i) {
    if (!IsAncestor(up_[u][i], v, tin_, tout_)) {
      u = up_[u][i];
    }
  }
  
  return up_[u][0];
}