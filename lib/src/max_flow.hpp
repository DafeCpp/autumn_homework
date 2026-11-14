#pragma once

#include <algorithm>
#include <limits>
#include <queue>
#include <vector>

constexpr long long kInf = std::numeric_limits<long long>::max();

struct FlowEdge {
  int to;
  long long cap;
  long long flow;
  int rev;
};

class MaxFlowNetwork {
 public:
  explicit MaxFlowNetwork(int n) : n_(n), adj_(n), level_(n), ptr_(n) {}

  void AddEdge(int from, int to, long long cap) {
    adj_[from].push_back({to, cap, 0, static_cast<int>(adj_[to].size())});
    adj_[to].push_back({from, 0, 0, static_cast<int>(adj_[from].size()) - 1});
  }

  long long GetMaxFlow(int s, int t) {
    if (s == t) return 0;
    long long flow = 0;
    while (Bfs(s, t)) {
      std::fill(ptr_.begin(), ptr_.end(), 0);
      while (long long pushed = Dfs(s, t, kInf)) {
        flow += pushed;
      }
    }
    return flow;
  }

 private:
  int n_;
  std::vector<std::vector<FlowEdge>> adj_;
  std::vector<int> level_;
  std::vector<int> ptr_;

  bool Bfs(int s, int t) {
    std::fill(level_.begin(), level_.end(), -1);
    level_[s] = 0;
    std::queue<int> q;
    q.push(s);

    while (!q.empty()) {
      int v = q.front();
      q.pop();
      for (const auto& edge : adj_[v]) {
        if (edge.cap - edge.flow > 0 && level_[edge.to] == -1) {
          level_[edge.to] = level_[v] + 1;
          q.push(edge.to);
        }
      }
    }
    return level_[t] != -1;
  }

  long long Dfs(int v, int t, long long pushed) {
    if (pushed == 0) return 0;
    if (v == t) return pushed;

    for (int& cid = ptr_[v]; cid < static_cast<int>(adj_[v].size()); ++cid) {
      auto& edge = adj_[v][cid];
      int tr = edge.to;

      if (level_[v] + 1 != level_[tr] || edge.cap - edge.flow == 0) {
        continue;
      }

      long long push = Dfs(tr, t, std::min(pushed, edge.cap - edge.flow));
      if (push == 0) continue;

      edge.flow += push;
      adj_[tr][edge.rev].flow -= push;
      return push;
    }
    return 0;
  }
};