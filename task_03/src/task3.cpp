#include "task3.hpp"
#include <vector>
#include <algorithm>

static void dfs(int v, int p, const std::vector<std::vector<int>>& adj,
                std::vector<bool>& visited, std::vector<int>& tin,
                std::vector<int>& low, int& timer,
                std::vector<bool>& is_router,
                std::vector<std::pair<int, int>>& critical_cables) {
    visited[v] = true;
    tin[v] = low[v] = timer++;
    int children = 0;

    for (size_t i = 0; i < adj[v].size(); ++i) {
        int neighbor = adj[v][i];
        
        if (neighbor == p) {
            continue;
        }

        if (visited[neighbor]) {
            low[v] = std::min(low[v], tin[neighbor]);
        } else {
            dfs(neighbor, v, adj, visited, tin, low, timer, is_router, critical_cables);
            
            low[v] = std::min(low[v], low[neighbor]);
            children++;

            if (low[neighbor] > tin[v]) {
                int n1 = v + 1;
                int n2 = neighbor + 1;
                if (n1 > n2) {
                    std::swap(n1, n2); 
                }
                critical_cables.push_back({n1, n2});
            }

            if (p != -1 && low[neighbor] >= tin[v]) {
                is_router[v] = true;
            }
        }
    }

    if (p == -1 && children > 1) {
        is_router[v] = true;
    }
}

NetworkResult FindWeakPoints(int n, const std::vector<std::pair<int, int>>& edges) {
    std::vector<std::vector<int>> adj(n);
    for (size_t i = 0; i < edges.size(); ++i) {
        int u = edges[i].first - 1;
        int v = edges[i].second - 1;
        if (u == v) continue; 
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    std::vector<bool> visited(n, false);
    std::vector<int> tin(n, -1);
    std::vector<int> low(n, -1);
    std::vector<bool> is_router(n, false);
    std::vector<std::pair<int, int>> critical_cables;
    int timer = 0;

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            dfs(i, -1, adj, visited, tin, low, timer, is_router, critical_cables);
        }
    }

    NetworkResult result;
    
    for (int i = 0; i < n; ++i) {
        if (is_router[i]) {
            result.critical_routers.push_back(i + 1); // Обратно к 1-индексации
        }
    }

    std::sort(critical_cables.begin(), critical_cables.end());
    result.critical_cables = critical_cables;

    return result;
}
