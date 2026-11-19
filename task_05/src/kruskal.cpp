#include "kruskal.hpp"

#include <algorithm>

#include "dsu.hpp"

long long kruskal(int n, std::vector<Edge> edges) {
  std::sort(edges.begin(), edges.end(),
            [](Edge a, Edge b) { return a.weight < b.weight; });

  DSU dsu(n);
  long long total = 0;
  int taken = 0;

  for (int i = 0; i < edges.size(); i++) {
    Edge e = edges[i];
    if (dsu.unite(e.firstVertex, e.secondVertex)) {
      total += e.weight;
      ++taken;

      if (taken == n - 1) {
        break;
      }
    }
  }

  if (taken == n - 1) {
    return total;
  }
  return -1;
}
