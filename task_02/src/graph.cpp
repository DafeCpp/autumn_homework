#include "graph.hpp"

#include <algorithm>

Graph::Graph() {
    int n, m;
    std::cin >> n >> m;
    graph.resize(n);
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        graph[a - 1].push_back(b - 1);
    }
    cnt_vertex = n;
}

const std::vector<std::vector<int> >& Graph::get_adjacency() const {
    return graph;
}

int Graph::size() const {
    return cnt_vertex;
}

void Graph::dfs(int v) {
    tin[v] = low[v] = timer++;
    scc_stack.push_back(v);
    on_stack[v] = true;
    for (int u : graph[v]) {
        if (tin[u] == -1) {
            dfs(u);
            low[v] = std::min(low[v], low[u]);
        }
        else if (on_stack[u]) {
            low[v] = std::min(low[v], tin[u]);
        }
    }
    if (low[v] == tin[v]) {
        int x;
        do {
            x = scc_stack.back();
            scc_stack.pop_back();
            on_stack[x] = false;
            comp[x] = cnt_comp;
        } while (x != v);
        ++cnt_comp;
    }
}

int Graph::get_scc(std::vector<int>& result) {
    tin.assign(cnt_vertex, -1);
    low.assign(cnt_vertex, 0);
    comp.assign(cnt_vertex, -1);
    on_stack.assign(cnt_vertex, false);
    scc_stack.clear();
    timer = 0;
    cnt_comp = 0;
    for (int i = 0; i < cnt_vertex; ++i) {
        if (tin[i] == -1) {
            dfs(i);
        }
    }
    result = comp;
    return cnt_comp;
}
