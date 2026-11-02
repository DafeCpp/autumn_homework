#include "graph.hpp"

#include <algorithm>

Graph::Graph() {
    int n, m;
    std::cin >> n >> m;
    graph.resize(n);
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        if (a == b) continue;
        graph[a - 1].push_back(b - 1);
        graph[b - 1].push_back(a - 1);
    }
    cnt_vertex = n;
}

Graph::Graph(int n, std::vector<std::pair<int, int> > edges) {
    graph.resize(n);
    for (auto [a, b] : edges) {
        if (a == b) continue;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }
    cnt_vertex = n;
}

void Graph::dfs(int v, int parent) {
    visited[v] = true;
    tin[v] = low[v] = timer++;
    bool skipped_parent = false;
    int children = 0;
    for (int u : graph[v]) {
        if (u == parent && !skipped_parent) {
            skipped_parent = true;
            continue;
        }
        if (visited[u]) {
            low[v] = std::min(low[v], tin[u]);
        }
        else {
            dfs(u, v);
            low[v] = std::min(low[v], low[u]);
            if (low[u] >= tin[v] && parent != -1) is_cut[v] = true;
            if (low[u] > tin[v]) bridges.push_back({std::min(v, u), std::max(v, u)});
            ++children;
        }
    }
    if (parent == -1 && children > 1) is_cut[v] = true;
}

void Graph::run() {
    visited.assign(cnt_vertex, false);
    is_cut.assign(cnt_vertex, false);
    tin.assign(cnt_vertex, 0);
    low.assign(cnt_vertex, 0);
    bridges.clear();
    timer = 0;
    for (int i = 0; i < cnt_vertex; ++i) {
        if (!visited[i]) {
            dfs(i, -1);
        }
    }
}

std::vector<int> Graph::get_cut_vertices() {
    run();
    std::vector<int> result;
    for (int i = 0; i < cnt_vertex; ++i) {
        if (is_cut[i]) result.push_back(i);
    }
    return result;
}

std::vector<std::pair<int, int> > Graph::get_bridges() {
    run();
    std::vector<std::pair<int, int> > result = bridges;
    std::sort(result.begin(), result.end());
    return result;
}
