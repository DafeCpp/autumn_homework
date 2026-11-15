#include <iostream>
#include <vector>
#include "../../lib/src/util.hpp"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    if (!(std::cin >> n >> m)) return 0;

    std::vector<std::pair<int, int>> edges;
    edges.reserve(m);
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        edges.emplace_back(a, b);
    }

    std::cout << min_edges_to_make_strongly_connected(n, edges) << "\n";

    return 0;
}