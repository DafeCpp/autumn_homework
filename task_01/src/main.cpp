#include "graph.h"
#include <iostream>

// В main.cpp:
int main() {
  int n, m;
  std::cin >> n >> m;
  
  Graph g(n);
  
  for (int i = 0; i < m; i++) {
    int u, v;
    std::cin >> u >> v;
    //1-based в 0-based
    u--; v--;
    g.AddEdge(u, v);
  }
  
  g.PrintResults();
  return 0;
}
