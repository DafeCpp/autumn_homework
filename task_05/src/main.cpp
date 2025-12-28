#include <algorithm>
#include <ctime>
#include <iostream>
#include <vector>

struct Edge {
  int u, v, w;
  int temp_w;  // Временный вес для рандомизации
};

struct DSU {
  std::vector<int> parent;
  DSU(int n) {
    parent.resize(n + 1);
    for (int i = 0; i <= n; ++i) parent[i] = i;
  }
  int find(int i) {
    if (parent[i] == i) return i;
    return parent[i] = find(parent[i]);
  }
  bool unite(int i, int j) {
    int root_i = find(i);
    int root_j = find(j);
    if (root_i != root_j) {
      parent[root_i] = root_j;
      return true;
    }
    return false;
  }
};

int solve() {
  int n, m;
  if (!(std::cin >> n >> m)) return -1;
  std::vector<Edge> edges(m);
  for (int i = 0; i < m; ++i) {
    std::cin >> edges[i].u >> edges[i].v >> edges[i].w;
  }
  int d_limit;
  std::cin >> d_limit;

  if (n == 1) return 0;
  if (d_limit < 1 || (d_limit == 1 && n > 2)) return -1;

  int min_total_weight = -1;
  std::srand(42);  // Фиксированный сид для детерминизма результата

  // Делаем достаточное количество итераций для поиска оптимума
  // При N=1000 и M=10000 это займет около 0.3-0.5 сек.
  for (int iter = 0; iter < 1000; ++iter) {
    for (int i = 0; i < m; ++i) {
      // Слегка искажаем веса: исходный вес + малый случайный шум
      // Это заставляет Крускала выбирать разные пути при равных или близких
      // весах
      edges[i].temp_w = edges[i].w * 100 + (std::rand() % 100);
    }

    std::sort(edges.begin(), edges.end(),
              [](const Edge& a, const Edge& b) { return a.temp_w < b.temp_w; });

    DSU dsu(n);
    std::vector<int> deg(n + 1, 0);
    int current_weight = 0;
    int count = 0;

    for (int i = 0; i < m; ++i) {
      int u = edges[i].u;
      int v = edges[i].v;
      if (dsu.find(u) != dsu.find(v)) {
        if (deg[u] < d_limit && deg[v] < d_limit) {
          dsu.unite(u, v);
          deg[u]++;
          deg[v]++;
          current_weight += edges[i].w;
          count++;
        }
      }
    }

    if (count == n - 1) {
      if (min_total_weight == -1 || current_weight < min_total_weight) {
        min_total_weight = current_weight;
      }
    }
  }

  return min_total_weight;
}

int main() {
  int result = solve();
  if (result == -1) {
    std::cout << "Невозможно построить остовное дерево с заданным ограничением "
                 "степени"
              << std::endl;
  } else {
    std::cout << result << std::endl;
  }
  return 0;
}
