#include "util.hpp"

std :: vector<int> Graph::TopologySort(int vartex) {
    dfs(vartex);
    return path;
}

void Graph::dfs(int vartex) {
    path.push_back(vartex);
    visited[vartex] = Color::black;
    for (auto v : Description[vartex]) {
        if (visited[v] == Color::white) dfs(v);
    }
}

