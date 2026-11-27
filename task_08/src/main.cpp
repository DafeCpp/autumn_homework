#include <iostream>
#include <vector>
#include "task8.h"
using namespace std;
int main() {
    int n, q;
    if (!(cin >> n >> q)) return 0;
    vector<pair<int, int>> edges;
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        edges.push_back({u, v});
    }
    LCA lca_solver(n, edges, 1);
    for (int i = 0; i < q; ++i) {
        int u, v;
        cin >> u >> v;
        cout << lca_solver.get_lca(u, v) << "\n";
    }
    return 0;
}