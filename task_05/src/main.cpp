#include <bits/stdc++.h>
using namespace std;
#include "weighted_graph.hpp"
/*
  Возвращает:
    - минимальный вес остовного дерева с ограничением степени d
    - или -1, если такое дерево построить невозможно
*/

long long DegreeBoundedMST(const WeightedGraph<int> &g, int d) {
  struct Edge {
    int u, v, w, id;
  };

  const auto &W = g.GetWeightedEdges();
  auto verts = g.GetVerticesIds();
  int n = verts.size();
  int m = W.size();

  if (n == 0) return 0;
  if (n == 1) return 0;

  // --- перенумерация вершин ---
  unordered_map<int, int> id;
  for (int i = 0; i < n; ++i) id[verts[i]] = i + 1;

  vector<Edge> edges;
  edges.reserve(m);
  for (int i = 0; i < m; ++i) {
    int u = id[W[i].start_vertex];
    int v = id[W[i].end_vertex];
    edges.push_back({u, v, W[i].weight, i});
  }

  // --- DSU ---
  struct DSU {
    vector<int> p, r;
    DSU(int n) : p(n + 1), r(n + 1, 0) { iota(p.begin(), p.end(), 0); }
    int f(int x) { return p[x] == x ? x : p[x] = f(p[x]); }
    bool u(int a, int b) {
      a = f(a);
      b = f(b);
      if (a == b) return false;
      if (r[a] < r[b]) swap(a, b);
      p[b] = a;
      if (r[a] == r[b]) r[a]++;
      return true;
    }
  };

  // --- попытка Краскала ---
  vector<char> inTree(m, 0);
  {
    vector<int> ord(m);
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(),
         [&](int a, int b) { return edges[a].w < edges[b].w; });

    DSU dsu(n);
    int cnt = 0;
    for (int idx : ord) {
      auto &e = edges[idx];
      if (e.u == e.v) continue;
      if (dsu.u(e.u, e.v)) {
        inTree[e.id] = 1;
        if (++cnt == n - 1) break;
      }
    }
    if (cnt != n - 1) return -1;  // граф несвязный
  }

  // --- восстановление дерева ---
  vector<vector<pair<int, int>>> adj(n + 1);
  vector<int> deg(n + 1, 0);
  long long total = 0;

  auto rebuild = [&]() {
    for (int i = 1; i <= n; ++i) adj[i].clear(), deg[i] = 0;
    total = 0;
    for (auto &e : edges) {
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
  vector<int> comp(n + 1);
  function<void(int, int)> dfs = [&](int v, int ban) {
    comp[v] = 1;
    for (auto [to, id] : adj[v]) {
      if (id == ban || comp[to]) continue;
      dfs(to, ban);
    }
  };

  for (int iter = 0; iter < 5000; ++iter) {
    int bad = -1;
    for (int i = 1; i <= n; ++i)
      if (deg[i] > d) {
        bad = i;
        break;
      }
    if (bad == -1) return total;

    long long best = LLONG_MAX;
    int rem = -1, add = -1;

    for (auto [to, remId] : adj[bad]) {
      fill(comp.begin(), comp.end(), 0);
      dfs(bad, remId);

      for (auto &e : edges) {
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

int main() {
  int n, m;
  std::cin >> n >> m;

  WeightedGraph<int> graph(false);
  for (int i = 0; i < n; i++) graph.AddVertex(i + 1);

  for (int i = 0; i < m; i++) {
    int u, v, w;
    std::cin >> u >> v >> w;
    graph.AddWeightedEdge(u, v, w);
  }
  int d;
  std::cin >> d;
  std::cout << DegreeBoundedMST(graph, d);
}