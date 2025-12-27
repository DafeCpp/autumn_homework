#include <bits/stdc++.h>

struct Edge {
  int u, v, w;
};

bool operator<(const Edge& a, const Edge& b) { return a.w < b.w; }

class DSU {
 public:
  explicit DSU(int n) {
    parent.resize(n + 1);
    rank.resize(n + 1, 0);
    for (int i = 0; i <= n; i++) parent[i] = i;
  }
  int find(int x) {
    if (parent[x] != x) parent[x] = find(parent[x]);
    return parent[x];
  }
  bool unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b) return false;
    if (rank[a] < rank[b]) std::swap(a, b);
    parent[b] = a;
    if (rank[a] == rank[b]) rank[a]++;
    return true;
  }

 private:
  std::vector<int> parent;
  std::vector<int> rank;
};

int DegreeConstrainedMST(int n, int m, int d, std::vector<Edge>& edges) {
  std::sort(edges.begin(), edges.end());

  DSU dsu(n);
  std::vector<int> deg(n + 1, 0);

  int count = 0;
  int totalWeight = 0;
  for (auto& e : edges) {
    if (dsu.find(e.u) != dsu.find(e.v) && deg[e.u] < d && deg[e.v] < d) {
      dsu.unite(e.u, e.v);
      deg[e.u]++;
      deg[e.v]++;
      totalWeight += e.w;
      count++;
      if (count == n - 1) break;
    }
  }

  if (count != n - 1) return -1;
  return totalWeight;
}
