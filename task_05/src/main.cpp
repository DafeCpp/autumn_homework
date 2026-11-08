#include <iostream>
#include <vector>

int main() {
  int n, m, d;
  std::cin >> n >> m >> d;

  std::vector<std::vector<std::pair<int, int>>> graph(
      n, std::vector<std::pair<int, int>>(n));
  for (int i = 1; i < m; ++i) {
    int u, v, w;
    std::cin >> u >> v >> w;
    graph[u].push_back({u, w});
  }
}
