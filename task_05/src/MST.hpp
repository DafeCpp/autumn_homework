#include <algorithm>
#include <climits>
#include <functional>
#include <numeric>
#include <unordered_map>
#include <vector>

#include "weighted_graph.hpp"

long long DegreeBoundedMST(const WeightedGraph<int>& g, int d) {
  struct Edge {
    int u, v, w, id;
  };

  const std::vector<WeightedEdge<int>>& W = g.GetWeightedEdges();
  std::vector<int> verts = g.GetVerticesIds();
  int n = verts.size();
  int m = W.size();

  if (n == 0) return 0;
  if (n == 1) return 0;

  // --- перенумерация вершин ---
  std::unordered_map<int, int> id;
  for (int i = 0; i < n; ++i) id[verts[i]] = i + 1;

  std::vector<Edge> edges;
  edges.reserve(m);
  for (int i = 0; i < m; ++i) {
    int u = id[W[i].start_vertex];
    int v = id[W[i].end_vertex];
    edges.push_back({u, v, W[i].weight, i});
  }

  struct DSU {
    std::vector<int> p, r;
    DSU(int n) : p(n + 1), r(n + 1, 0) { std::iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int a, int b) {
      a = find(a);
      b = find(b);
      if (a == b) return false;
      if (r[a] < r[b]) std::swap(a, b);
      p[b] = a;
      if (r[a] == r[b]) r[a]++;
      return true;
    }
  };

  std::vector<char> inTree(m, 0);
  {
    std::vector<int> ord(m);
    std::iota(ord.begin(), ord.end(), 0);
    std::sort(ord.begin(), ord.end(),
              [&](int a, int b) { return edges[a].w < edges[b].w; });

    DSU dsu(n);
    int cnt = 0;
    for (int idx : ord) {
      Edge& e = edges[idx];
      if (e.u == e.v) continue;
      if (dsu.unite(e.u, e.v)) {
        inTree[e.id] = 1;
        if (++cnt == n - 1) break;
      }
    }
    if (cnt != n - 1) return -1;  // граф несвязный
  }

  std::vector<std::vector<std::pair<int, int>>> adj(n + 1);
  std::vector<int> deg(n + 1, 0);
  long long total = 0;

  auto rebuild = [&]() {
    for (int i = 1; i <= n; ++i) {
      adj[i].clear();
      deg[i] = 0;
    }
    total = 0;
    for (const Edge& e : edges) {
      if (!inTree[e.id]) continue;
      adj[e.u].push_back({e.v, e.id});
      adj[e.v].push_back({e.u, e.id});
      deg[e.u]++;
      deg[e.v]++;
      total += e.w;
    }
  };

  rebuild();

  auto degrees_ok = [&]() {
    for (int i = 1; i <= n; ++i)
      if (deg[i] > d) return false;
    return true;
  };

  if (degrees_ok()) return total;

  // --- локальные обмены ---
  std::vector<int> comp(n + 1);
  std::function<void(int, int)> dfs = [&](int v, int ban) {
    comp[v] = 1;
    for (const auto& [to, id] : adj[v]) {
      if (id == ban || comp[to]) continue;
      dfs(to, ban);
    }
  };

  for (int iter = 0; iter < 5000; ++iter) {
    int bad = -1;
    for (int i = 1; i <= n; ++i) {
      if (deg[i] > d) {
        bad = i;
        break;
      }
    }
    if (bad == -1) return total;

    long long best = LLONG_MAX;
    int rem = -1, add = -1;

    for (const auto& [to, remId] : adj[bad]) {
      std::fill(comp.begin(), comp.end(), 0);
      dfs(bad, remId);

      for (const Edge& e : edges) {
        if (inTree[e.id]) continue;
        if (comp[e.u] == comp[e.v]) continue;

        int du = deg[e.u] + 1 - (e.u == bad);
        int dv = deg[e.v] + 1 - (e.v == bad);
        if (du > d || dv > d) continue;

        long long delta = e.w - edges[remId].w;
        if (delta < best) {
          best = delta;
          rem = remId;
          add = e.id;
        }
      }
    }

    if (rem == -1) return -1;

    inTree[rem] = 0;
    inTree[add] = 1;
    rebuild();
  }

  return degrees_ok() ? total : -1;
}
