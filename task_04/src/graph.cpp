#include "graph.hpp"

Graph::Graph() {
    int n, m;
    std::cin >> n >> m;
    graph.resize(n);
    for (int i = 0; i < m; ++i) {
        int a, b;
        long long w;
        std::cin >> a >> b >> w;
        graph[a - 1].push_back({b - 1, w});
    }
    cnt_vertex = n;
}

Graph::Graph(int n, std::vector<std::vector<int> > edges) {
    graph.resize(n);
    for (const std::vector<int>& e : edges) {
        graph[e[0]].push_back({e[1], e[2]});
    }
    cnt_vertex = n;
}

int Graph::size() const {
    return cnt_vertex;
}

const std::vector<std::vector<std::pair<int, long long> > >& Graph::get_adjacency() const {
    return graph;
}

bool Graph::get_potentials(std::vector<long long>& h) const {
    h.assign(cnt_vertex, 0);
    for (int iter = 0; iter < cnt_vertex; ++iter) {
        bool changed = false;
        for (int v = 0; v < cnt_vertex; ++v) {
            for (auto [u, w] : graph[v]) {
                if (h[v] + w < h[u]) {
                    h[u] = h[v] + w;
                    changed = true;
                }
            }
        }
        if (!changed) {
            return true;
        }
    }
    return false;
}
