#include "maxflow.h"

#include <queue>
#include <vector>
#include <climits>

MaxFlow::MaxFlow(int n, const std::vector<std::vector<int>>& edges) : n_(n) {
  capacity_.assign(n, std::vector<int>(n, 0));
  adj_.assign(n, std::vector<int>());

  for (const auto& edge : edges) {
    int u = edge[0];
    int v = edge[1];
    int c = edge[2];
    capacity_[u][v] = c;
    adj_[u].push_back(v);
    adj_[v].push_back(u);
  }
}

int MaxFlow::Calculate(int source, int sink) {
  int max_flow = 0;
  std::vector<int> parent(n_);

  while (BFS(source, sink, parent)) {
    int path_flow = INT_MAX;
    for (int v = sink; v != source; v = parent[v]) {
      int u = parent[v];
      path_flow = std::min(path_flow, capacity_[u][v]);
    }

    for (int v = sink; v != source; v = parent[v]) {
      int u = parent[v];
      capacity_[u][v] -= path_flow;
      capacity_[v][u] += path_flow;
    }

    max_flow += path_flow;
  }

  return max_flow;
}

bool MaxFlow::BFS(int source, int sink, std::vector<int>& parent) {
  std::vector<bool> visited(n_, false);
  std::queue<int> q;
  q.push(source);
  visited[source] = true;
  parent[source] = -1;

  while (!q.empty()) {
    int u = q.front();
    q.pop();

    for (int v : adj_[u]) {
      if (!visited[v] && capacity_[u][v] > 0) {
        if (v == sink) {
          parent[v] = u;
          return true;
        }
        q.push(v);
        parent[v] = u;
        visited[v] = true;
      }
    }
  }

  return false;
}