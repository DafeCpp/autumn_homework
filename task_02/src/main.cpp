#include <algorithm>
#include <iostream>
#include <vector>

int n, m;
std::vector<std::vector<int>> adj, radj;
std::vector<bool> vis;
std::vector<int> order, comp;
int comp_id;

void DFS(int u, const std::vector<std::vector<int>>& g, bool build_order = false) {
    vis[u] = true;
    for (int v : g[u]) {
        if (!vis[v]) {
            DFS(v, g, build_order);
        }
    }
    if (build_order) order.push_back(u);
}

int Kosaraju() {
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
            DFS(++comp_id, radj);
            comp[u] = comp_id;
        }
    }

    if (comp_id == 1) return 0;

    std::vector<bool> is_src(comp_id + 1, true), is_snk(comp_id + 1, true);
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

int main() {
    std::cin >> n >> m;

    adj.resize(n + 1);
    radj.resize(n + 1);
    vis.resize(n + 1);
    comp.resize(n + 1);

    for (int i = 0; i <= n; ++i) {
        adj[i].clear();
        radj[i].clear();
    }

    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        adj[u].push_back(v);
        radj[v].push_back(u);
    }

    std::cout << Kosaraju() << "\n";
    return 0;
}
