#include <iostream>
#include <vector>
#include "kosaraju.h"

int main() {
    int n, m;
    std::cin >> n >> m;

    std::vector<Edge> edges(m);
    for (int i = 0; i < m; ++i) {
        std::cin >> edges[i].from >> edges[i].to;
    }

    KosarajuSolver solver;
    solver.init(n, edges);
    std::cout << solver.solve() << "\n";

    return 0;
}
