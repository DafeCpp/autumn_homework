#include <climits>
#include <queue>
#include <vector>

struct Edge {
  int to;
  int rev;
  int cap;
};

class MaxFlow {
 public:
  explicit MaxFlow(int n) : n(n) { graph.resize(n); }

  void addEdge(int u, int v, int cap) {
    graph[u].push_back({v, (int)graph[v].size(), cap});
    graph[v].push_back({u, (int)graph[u].size() - 1, 0});  // обратное ребро с 0
  }

  int edmondsKarp(int s, int t) {
    int flow = 0;
    std::vector<int> parent(n), parentEdge(n);
    while (true) {
      std::fill(parent.begin(), parent.end(), -1);
      std::queue<int> q;
      q.push(s);
      parent[s] = s;
      while (!q.empty() && parent[t] == -1) {
        int u = q.front();
        q.pop();
        for (int i = 0; i < (int)graph[u].size(); i++) {
          Edge &e = graph[u][i];
          if (parent[e.to] == -1 && e.cap > 0) {
            parent[e.to] = u;
            parentEdge[e.to] = i;
            q.push(e.to);
          }
        }
      }
      if (parent[t] == -1) break;  // больше нет пути
      int f = INT_MAX;
      for (int v = t; v != s; v = parent[v]) {
        int u = parent[v];
        Edge &e = graph[u][parentEdge[v]];
        f = std::min(f, e.cap);
      }
      for (int v = t; v != s; v = parent[v]) {
        int u = parent[v];
        Edge &e = graph[u][parentEdge[v]];
        e.cap -= f;
        graph[e.to][e.rev].cap += f;
      }
      flow += f;
    }
    return flow;
  }

 private:
  int n;
  std::vector<std::vector<Edge>> graph;
};
