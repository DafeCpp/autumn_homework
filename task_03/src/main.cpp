#include <iostream>
#include <vector>
#include <string>
#include "../../lib/src/util.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    if (!(std::cin >> n >> m)) return 0;

    std::vector<std::pair<int, int>> edges;
    edges.reserve(m);
    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        if (u == v) continue; 
        edges.emplace_back(u, v);
    }

    NetworkVulnerabilities res = find_vulnerabilities(n, edges);

    std::cout << res.critical_routers.size() << "\n";
    
    // 2. Список устройств
    if (res.critical_routers.empty()) {
        std::cout << "-\n";
    } else {
        for (size_t i = 0; i < res.critical_routers.size(); ++i) {
            if (i > 0) std::cout << " ";
            std::cout << res.critical_routers[i];
        }
        std::cout << "\n";
    }

    std::cout << res.critical_cables.size() << "\n";

    if (res.critical_cables.empty()) {
        std::cout << "-\n";
    } else {
        for (size_t i = 0; i < res.critical_cables.size(); ++i) {
            if (i > 0) std::cout << "; ";
            std::cout << res.critical_cables[i].first << " " << res.critical_cables[i].second;
        }
        std::cout << "\n";
    }

    return 0;
}