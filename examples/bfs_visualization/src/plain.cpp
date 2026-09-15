#include <iostream>
#include <queue>
#include <utility>
#include <vector>

int main() {
  int n, m;
  if (!(std::cin >> n >> m) || n < 1 || n > 200000 || m < 0 || m > 200000)
    return 1;
  std::vector<std::vector<std::pair<int, int>>> graph(n + 1);
  for (int id = 0; id < m; ++id) {
    int u, v;
    if (!(std::cin >> u >> v) || u < 1 || u > n || v < 1 || v > n) return 1;
    graph[u].push_back({v, id});
    graph[v].push_back({u, id});
  }
  std::vector<int> distance(n + 1, -1);
  std::queue<int> queue;
  distance[1] = 0;
  queue.push(1);
  while (!queue.empty()) {
    int v = queue.front();
    queue.pop();
    for (auto [to, edge_id] : graph[v]) {
      if (distance[to] != -1) continue;
      distance[to] = distance[v] + 1;
      queue.push(to);
    }
  }
  for (int v = 1; v <= n; ++v) {
    if (v > 1) std::cout << ' ';
    std::cout << distance[v];
  }
  std::cout << '\n';
}
