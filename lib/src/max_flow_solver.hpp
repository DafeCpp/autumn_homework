#ifndef MAX_FLOW_SOLVER_HPP
#define MAX_FLOW_SOLVER_HPP

#include <algorithm>
#include <limits>
#include <queue>
#include <vector>

class MaxFlowSolver {
 public:
  struct Edge {
    int to;
    long long cap;
    int rev;
  };

  MaxFlowSolver(int n) : n_(n), adj_(n + 1), level_(n + 1, -1) {}

  void addEdge(int u, int v, long long c) {
    if (u <= 0 || u > n_ || v <= 0 || v > n_ || u == v) return;

    int rev_u = static_cast<int>(adj_[v].size());
    int rev_v = static_cast<int>(adj_[u].size());

    adj_[u].push_back({v, c, rev_u});

    adj_[v].push_back({u, 0, rev_v});
  }

  long long maxFlow(int source, int sink) {
    long long flow = 0;
    while (bfs(source, sink)) {
      std::vector<int> ptr(n_ + 1, 0);
      while (long long pushed = dfs(
                 source, sink, std::numeric_limits<long long>::max(), ptr)) {
        flow += pushed;
      }
    }
    return flow;
  }

 private:
  int n_;
  std::vector<std::vector<Edge>> adj_;
  std::vector<int> level_;

  bool bfs(int s, int t) {
    std::fill(level_.begin(), level_.end(), -1);
    std::queue<int> q;
    level_[s] = 0;
    q.push(s);

    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (const auto& e : adj_[u]) {
        if (e.cap > 0 && level_[e.to] == -1) {
          level_[e.to] = level_[u] + 1;
          q.push(e.to);
        }
      }
    }
    return level_[t] != -1;
  }

  long long dfs(int u, int t, long long f, std::vector<int>& ptr) {
    if (f == 0) return 0;
    if (u == t) return f;

    for (int& i = ptr[u]; i < static_cast<int>(adj_[u].size()); ++i) {
      Edge& e = adj_[u][i];
      if (level_[e.to] == level_[u] + 1 && e.cap > 0) {
        long long pushed = dfs(e.to, t, std::min(f, e.cap), ptr);
        if (pushed > 0) {
          e.cap -= pushed;
          adj_[e.to][e.rev].cap += pushed;
          return pushed;
        }
      }
    }
    return 0;
  }
};

#endif  // MAX_FLOW_SOLVER_HPP