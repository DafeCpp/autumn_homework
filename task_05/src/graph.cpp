#include "graph.hpp"

#include <algorithm>
#include <map>

Graph::Graph() {
    int n, m, d;
    std::cin >> n >> m >> d;
    std::vector<Edge> input;
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        std::cin >> u >> v >> w;
        input.push_back({u - 1, v - 1, w});
    }
    *this = Graph(n, input, d);
}

Graph::Graph(int n, std::vector<Edge> input, int d) {
    std::map<std::pair<int, int>, int> best;
    for (const Edge& e : input) {
        if (e.u == e.v) continue;
        std::pair<int, int> key = {std::min(e.u, e.v), std::max(e.u, e.v)};
        if (!best.count(key) || e.w < best[key]) {
            best[key] = e.w;
        }
    }
    for (auto [key, w] : best) {
        edges.push_back({key.first, key.second, w});
    }
    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) { return a.w < b.w; });
    cnt_vertex = n;
    max_degree = d;
}

int Graph::size() const {
    return cnt_vertex;
}

int Graph::get_max_degree() const {
    return max_degree;
}

const std::vector<Edge>& Graph::get_edges() const {
    return edges;
}
