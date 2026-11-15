#pragma once
#include <vector>
#include <utility>

std::vector<int> topological_sort(int n,
                                  const std::vector<std::pair<int,int>>& edges);

int min_edges_to_make_strongly_connected(int n, 
                                         const std::vector<std::pair<int,int>>& edges);

struct NetworkVulnerabilities {
    std::vector<int> critical_routers;
    std::vector<std::pair<int, int>> critical_cables;
};

NetworkVulnerabilities find_vulnerabilities(
    int n, 
    const std::vector<std::pair<int, int>>& edges
);