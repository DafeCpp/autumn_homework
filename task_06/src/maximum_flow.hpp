#pragma once

#include <unordered_map>
#include <vector>

struct Flow {
  int flow;
  int capacity;
};

struct MaxFlow {
  MaxFlow(std::vector<std::unordered_map<int, Flow>> graph) : graph(graph) {
    n = graph.size();
    visited.assign(n, false);
    FordFulkerson();
  }
  int max_flow;

 private:
  int source{-1}, drain{-1};
  std::vector<std::unordered_map<int, Flow>> graph;
  int n;
  int DFS(int u, int addition);
  void FindSourceAndDrain();
  void FordFulkerson();
  std::vector<bool> visited;
};