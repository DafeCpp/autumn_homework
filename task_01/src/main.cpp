#include <iostream>
#include "../../lib/src/util.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    std::vector<std::pair<int,int>> edges;
    edges.reserve(m);
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        edges.emplace_back(a, b);
    }

    std::vector<int> order = topological_sort(n, edges);

    if (order.empty()) {
        std::cout << -1 << '\n';
    } else {
        for (int i = 0; i < n; ++i) {
            if (i) std::cout << ' ';
            std::cout << order[i];
        }
        std::cout << '\n';
    }

    return 0;
}