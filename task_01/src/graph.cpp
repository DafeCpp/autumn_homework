#include "graph.hpp"

Graph::Graph() {
      int n, m;
      std::cin >> n >> m;
      graph.resize(n);
      for (int i = 0; i < m; ++i) {
            int a, b;
            std::cin >> a >> b;
            graph[a].push_back(b);
      }
      cnt_vertex = n;
      have_cycle = false;
}

void Graph::dfs(int v) {
    color[v] = 1;
    for (int u : graph[v]) {
        if (!color[u]) {
          dfs(u);
        }
        else if (color[u] == 1) have_cycle = true;
    }
    color[v] = 2;
    top_sort.push_back(v);
}

std::vector<int> Graph::get_top_sort() {
    color.assign(cnt_vertex, 0);
    top_sort.clear();
    for (int i = 0; i < cnt_vertex; ++i) {
        if (!color[i]) {
            dfs(i);
        }
    }
    if (have_cycle) {
        return std::vector<int>(1, -1);
    }
    return top_sort;
}