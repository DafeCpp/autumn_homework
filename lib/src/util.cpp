#include "util.hpp"
#include <queue>

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