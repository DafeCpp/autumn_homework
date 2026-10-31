#include "johnson.h"

#include <iostream>
#include <queue>
#include <utility>
#include <vector>

Graph ReadGraph(int n, int m) {
  Graph g;
  g.n = n;
  g.adj.resize(n + 1);

  for (int i = 0; i < m; ++i) {
    int u = 0;
    int v = 0;
    long long w = 0;
    std::cin >> u >> v >> w;
    g.adj[u].push_back({v, w});
  }

  return g;
}

std::vector<long long> BellmanFord(const Graph& g) {
  const int n = g.n;
  std::vector<long long> h(n + 1, 0);

  for (int iter = 0; iter < n; ++iter) {
    bool relaxed = false;

    for (int u = 1; u <= n; ++u) {
      for (const auto& edge : g.adj[u]) {
        const int v = edge.to;
        const long long w = edge.weight;

        if (h[u] + w < h[v]) {
          h[v] = h[u] + w;
          relaxed = true;
        }
      }
    }

    if (!relaxed) {
      break;
    }
  }

  for (int u = 1; u <= n; ++u) {
    for (const auto& edge : g.adj[u]) {
      const int v = edge.to;
      const long long w = edge.weight;

      if (h[u] + w < h[v]) {
        return {};
      }
    }
  }

  return h;
}

std::vector<long long> Dijkstra(const Graph& g, int start,
                                const std::vector<long long>& h) {
  const int n = g.n;
  std::vector<long long> dist(n + 1, kInf);
  std::priority_queue<std::pair<long long, int>,
                      std::vector<std::pair<long long, int>>,
                      std::greater<std::pair<long long, int>>>
      pq;

  dist[start] = 0;
  pq.push({0, start});

  while (!pq.empty()) {
    const auto [d, u] = pq.top();
    pq.pop();

    if (d > dist[u]) {
      continue;
    }

    for (const auto& edge : g.adj[u]) {
      const int v = edge.to;
      const long long w = edge.weight;
      const long long w_prime = w + h[u] - h[v];

      if (dist[u] + w_prime < dist[v]) {
        dist[v] = dist[u] + w_prime;
        pq.push({dist[v], v});
      }
    }
  }

  return dist;
}

std::vector<std::vector<long long>> Johnson(const Graph& g,
                                            bool& has_negative_cycle) {
  const int n = g.n;

  const std::vector<long long> h = BellmanFord(g);

  if (h.empty()) {
    has_negative_cycle = true;
    return {};
  }

  has_negative_cycle = false;

  std::vector<std::vector<long long>> dist(
      n + 1, std::vector<long long>(n + 1, kInf));

  for (int i = 1; i <= n; ++i) {
    const std::vector<long long> dist_prime = Dijkstra(g, i, h);

    for (int j = 1; j <= n; ++j) {
      if (dist_prime[j] != kInf) {
        dist[i][j] = dist_prime[j] - h[i] + h[j];
      }
    }

    dist[i][i] = 0;
  }

  return dist;
}

void PrintResult(const std::vector<std::vector<long long>>& dist,
                 bool has_negative_cycle) {
  if (has_negative_cycle) {
    std::cout << "NEGATIVE CYCLE\n";
    return;
  }

  const int n = static_cast<int>(dist.size()) - 1;
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= n; ++j) {
      if (j > 1) {
        std::cout << ' ';
      }
      if (dist[i][j] == kInf) {
        std::cout << "INF";
      } else {
        std::cout << dist[i][j];
      }
    }
    std::cout << '\n';
  }
}
#ifndef TASK_04_TESTING
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  int m = 0;
  std::cin >> n >> m;

  Graph g = ReadGraph(n, m);

  bool has_negative_cycle = false;
  const std::vector<std::vector<long long>> dist =
      Johnson(g, has_negative_cycle);

  PrintResult(dist, has_negative_cycle);

  return 0;
}
#endif
