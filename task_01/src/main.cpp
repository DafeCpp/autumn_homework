#include <iostream>
#include <vector>

std::vector <std::vector <int>> G;
std::vector <int> color;
std::vector <int> sorted;

void dfs(int v) {
    color[v] = 1;
    for (auto u : G[v]) {
        if (color[u] == 0) {
            dfs(u);
        }
        else if (color[u] == 1) {
            return;
        }
    }
    color[v] = 2;
    sorted.push_back(v);
}

int main() { 
    int n, m;
    std::cin >> n >> m;
    
    G.resize(n);
    color.resize(n);

    for (int i = 0; i < m; i++) {
        int a, b;
        std::cin >> a >> b;
        G[a].push_back(b);
    }

    for (int i = 0; i < n; i++) {
        if (color[i] == 0) {
            dfs(i);
        }
    }

    if (n == sorted.size()) {
        for (int i = 0; i < n; i++) {
            std::cout << sorted[i] << ' ';
        }
    }
    else {
        std::cout << -1;
    }
    std::cout << '\n';

    return 0; 
}
