#ifndef MAX_FLOW_SOLVER_HPP
#define MAX_FLOW_SOLVER_HPP

#include <algorithm>
#include <climits>
#include <queue>
#include <vector>

class MaxFlowSolver {
 private:
  struct Edge {
    int to;
    long long capacity;
    long long flow;
    int rev;
  };

  int n;
  std::vector<std::vector<Edge>> adj;
  std::vector<int> level;
  std::vector<int> ptr;

  bool bfs(int s, int t) {
    std::fill(level.begin(), level.end(), -1);
    level[s] = 0;
    std::queue<int> q;
    q.push(s);

    while (!q.empty()) {
      int v = q.front();
      q.pop();
      for (const auto& edge : adj[v]) {
        if (edge.capacity - edge.flow > 0 && level[edge.to] == -1) {
          level[edge.to] = level[v] + 1;
          q.push(edge.to);
        }
      }
    }
    return level[t] != -1;
  }

  long long dfs(int v, int t, long long pushed) {
    if (pushed == 0) return 0;
    if (v == t) return pushed;

    for (int& cid = ptr[v]; cid < (int)adj[v].size(); ++cid) {
      auto& edge = adj[v][cid];
      int tr = edge.to;
      if (level[v] + 1 != level[tr] || edge.capacity - edge.flow == 0) continue;

      long long push = dfs(tr, t, std::min(pushed, edge.capacity - edge.flow));
      if (push == 0) continue;

      edge.flow += push;
      adj[tr][edge.rev].flow -= push;
      return push;
    }
    return 0;
  }

 public:
  MaxFlowSolver(int vertices)
      : n(vertices), adj(vertices), level(vertices), ptr(vertices) {}

  void addEdge(int from, int to, long long capacity) {
    adj[from].push_back({to, capacity, 0, (int)adj[to].size()});
    adj[to].push_back({from, 0, 0, (int)adj[from].size() - 1});
  }

  long long maxFlow(int s, int t) {
    long long flow = 0;
    while (bfs(s, t)) {
      std::fill(ptr.begin(), ptr.end(), 0);
      while (long long pushed = dfs(s, t, LLONG_MAX)) {
        flow += pushed;
      }
    }
    return flow;
  }
};

#endif  // MAX_FLOW_SOLVER_HPP