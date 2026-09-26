#pragma once

#include <algorithm>
#include <vector>

struct Edge {
    int from;
    int to;
};

class KosarajuSolver {
private:
    int n;
    std::vector<std::vector<int>> adj;
    std::vector<std::vector<int>> radj;
    std::vector<bool> vis;
    std::vector<int> order;
    std::vector<int> comp;
    int comp_id;

    void DFS(int u, const std::vector<std::vector<int>>& g, bool build_order) {
        vis[u] = true;
        if (!build_order) comp[u] = comp_id;
        for (int v : g[u]) {
            if (!vis[v]) {
                DFS(v, g, build_order);
            }
        }
        if (build_order) order.push_back(u);
    }

public:
    KosarajuSolver() : n(0), comp_id(0) {}

    void init(int n_vertices, const std::vector<Edge>& edges) {
        n = n_vertices;
        adj.resize(n + 1);
        radj.resize(n + 1);
        vis.resize(n + 1);
        comp.resize(n + 1);

        for (int i = 0; i <= n; ++i) {
            adj[i].clear();
            radj[i].clear();
        }

        for (const auto& edge : edges) {
            adj[edge.from].push_back(edge.to);
            radj[edge.to].push_back(edge.from);
        }
    }

    int solve() {
        for (int i = 0; i <= n; ++i) vis[i] = false;
        order.clear();

        for (int i = 1; i <= n; ++i) {
            if (!vis[i]) DFS(i, adj, true);
        }

        for (int i = 0; i <= n; ++i) vis[i] = false;
        comp_id = 0;
        for (int i = 0; i <= n; ++i) comp[i] = 0;

        while (!order.empty()) {
            int u = order.back();
            order.pop_back();
            if (!vis[u]) {
                ++comp_id;
                DFS(u, radj, false);
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
