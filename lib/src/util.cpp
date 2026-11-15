#include "util.hpp"
#include <queue>
#include <algorithm>
#include <vector>

std::vector<int> topological_sort(int n,
                                  const std::vector<std::pair<int,int>>& edges) {
    std::vector<std::vector<int>> graph(n);
    std::vector<int> indeg(n, 0);

    for (auto [a, b] : edges) {
        graph[b].push_back(a);
        ++indeg[a];
    }

    std::queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (indeg[i] == 0) q.push(i);
    }

    std::vector<int> result;
    result.reserve(n);

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        result.push_back(v);
        for (int u : graph[v]) {
            if (--indeg[u] == 0) q.push(u);
        }
    }

    if ((int)result.size() != n) return {};
    return result;
}

int min_edges_to_make_strongly_connected(int n, const std::vector<std::pair<int,int>>& edges) {
    if (n <= 1) return 0;

    std::vector<std::vector<int>> adj(n + 1);
    std::vector<std::vector<int>> radj(n + 1);
    for (auto [a, b] : edges) {
        adj[a].push_back(b);
        radj[b].push_back(a);
    }

    std::vector<bool> used(n + 1, false);
    std::vector<int> order;
    order.reserve(n);

    for (int i = 1; i <= n; ++i) {
        if (used[i]) continue;
        std::vector<std::pair<int, int>> st;
        st.push_back({i, 0});
        used[i] = true;
        while (!st.empty()) {
            int v = st.back().first;
            int& idx = st.back().second;
            if (idx < (int)adj[v].size()) {
                int u = adj[v][idx++];
                if (!used[u]) {
                    used[u] = true;
                    st.push_back({u, 0});
                }
            } else {
                order.push_back(v);
                st.pop_back();
            }
        }
    }

    std::vector<int> comp(n + 1, -1);
    int comp_count = 0;
    for (int i = n - 1; i >= 0; --i) {
        int v = order[i];
        if (comp[v] != -1) continue;
        std::vector<int> st = {v};
        comp[v] = comp_count;
        while (!st.empty()) {
            int u = st.back();
            st.pop_back();
            for (int w : radj[u]) {
                if (comp[w] == -1) {
                    comp[w] = comp_count;
                    st.push_back(w);
                }
            }
        }
        comp_count++;
    }

    if (comp_count == 1) return 0;

    std::vector<bool> has_in(comp_count, false);
    std::vector<bool> has_out(comp_count, false);

    for (int a = 1; a <= n; ++a) {
        for (int b : adj[a]) {
            if (comp[a] != comp[b]) {
                has_out[comp[a]] = true;
                has_in[comp[b]] = true;
            }
        }
    }

    int sources = 0, sinks = 0;
    for (int i = 0; i < comp_count; ++i) {
        if (!has_in[i]) sources++;
        if (!has_out[i]) sinks++;
    }

    return std::max(sources, sinks);
}