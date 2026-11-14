#pragma once

#include <algorithm>
#include <vector>

struct Edge {
  int from;
  int to;
};

class KosarajuSolver {
 private:
  int n = 0;
  std::vector<std::vector<int>> adj;
  std::vector<std::vector<int>> radj;
  std::vector<bool> vis;
  std::vector<int> order;
  std::vector<int> comp;
  int comp_id = 0;

 public:
  KosarajuSolver() = default;

  void init(int n_vertices, const std::vector<Edge>& edges) {
    n = n_vertices;
    adj.assign(n + 1, {});
    radj.assign(n + 1, {});
    vis.assign(n + 1, false);
    comp.assign(n + 1, 0);
    order.clear();
    comp_id = 0;

    for (const auto& edge : edges) {
      adj[edge.from].push_back(edge.to);
      radj[edge.to].push_back(edge.from);
    }
  }

  int solve() {
    std::fill(vis.begin(), vis.end(), false);
    order.clear();

    for (int i = 1; i <= n; ++i) {
      if (!vis[i]) {
        std::vector<std::pair<int, size_t>> st;
        st.push_back({i, 0});
        vis[i] = true;

        while (!st.empty()) {
          auto& top = st.back();
          int u = top.first;
          size_t& idx = top.second;

          if (idx < adj[u].size()) {
            int v = adj[u][idx++];
            if (!vis[v]) {
              vis[v] = true;
              st.push_back({v, 0});
            }
          } else {
            order.push_back(u);
            st.pop_back();
          }
        }
      }
    }

    std::fill(vis.begin(), vis.end(), false);
    std::fill(comp.begin(), comp.end(), 0);
    comp_id = 0;

    for (auto it = order.rbegin(); it != order.rend(); ++it) {
      int u = *it;
      if (!vis[u]) {
        ++comp_id;
        std::vector<int> st;
        st.push_back(u);
        vis[u] = true;
        comp[u] = comp_id;

        while (!st.empty()) {
          int curr = st.back();
          st.pop_back();
          for (int v : radj[curr]) {
            if (!vis[v]) {
              vis[v] = true;
              comp[v] = comp_id;
              st.push_back(v);
            }
          }
        }
      }
    }

    if (comp_id == 1) return 0;

    std::vector<bool> is_src(comp_id + 1, true);
    std::vector<bool> is_snk(comp_id + 1, true);

    for (int u = 1; u <= n; ++u) {
      for (int v : adj[u]) {
        if (comp[u] != comp[v]) {
          is_snk[comp[u]] = false;
          is_src[comp[v]] = false;
        }
      }
    }

    int src = 0, snk = 0;
    for (int i = 1; i <= comp_id; ++i) {
      src += is_src[i];
      snk += is_snk[i];
    }

    return std::max(src, snk);
  }
};
