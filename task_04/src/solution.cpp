#include "solution.hpp"

#include <queue>

std::optional<std::vector<std::vector<long long> > > johnson(Graph graph) {
    int n = graph.size();
    std::vector<long long> h;
    if (!graph.get_potentials(h)) {
        return std::nullopt;
    }
    const std::vector<std::vector<std::pair<int, long long> > >& adj = graph.get_adjacency();

    std::vector<std::vector<long long> > result(n, std::vector<long long>(n, INF));
    for (int s = 0; s < n; ++s) {
        std::vector<long long>& dist = result[s];
        std::priority_queue<std::pair<long long, int>, std::vector<std::pair<long long, int> >,
                            std::greater<std::pair<long long, int> > >
            q;
        dist[s] = 0;
        q.push({0, s});
        while (!q.empty()) {
            auto [d, v] = q.top();
            q.pop();
            if (d > dist[v]) continue;
            for (auto [u, w] : adj[v]) {
                long long nd = d + w + h[v] - h[u];
                if (nd < dist[u]) {
                    dist[u] = nd;
                    q.push({nd, u});
                }
            }
        }
        for (int v = 0; v < n; ++v) {
            if (dist[v] != INF) {
                dist[v] += h[v] - h[s];
            }
        }
    }
    return result;
}
