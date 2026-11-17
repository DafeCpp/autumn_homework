#include "task6.h"

#include <vector>

using namespace std;

int dfs(int u, int t, int push, vector<vector<Edge>>& adj,
        vector<bool>& visited) {
  if (u == t) return push;

  visited[u] = true;

  for (auto& edge : adj[u]) {
    if (!visited[edge.to] && edge.cap - edge.flow > 0) {
      int can_push = min(push, edge.cap - edge.flow);
      int pushed = dfs(edge.to, t, can_push, adj, visited);

      if (pushed > 0) {
        edge.flow += pushed;
        adj[edge.to][edge.rev].flow -= pushed;
        return pushed;
      }
    }
  }
  return 0;
}

long long task6(int n, int m, const vector<vector<int>>& edges_input,
                int source, int sink) {
  vector<vector<Edge>> adj(n + 1);

  for (const auto& e : edges_input) {
    int u = e[0], v = e[1], cap = e[2];
    adj[u].push_back({v, cap, 0, (int)adj[v].size()});
    adj[v].push_back({u, 0, 0, (int)adj[u].size() - 1});
  }

  long long max_flow = 0;

  while (true) {
    vector<bool> visited(n + 1, false);
    int pushed = dfs(source, sink, INF, adj, visited);

    if (pushed == 0) break;

    max_flow += pushed;
  }

  return max_flow;
}
