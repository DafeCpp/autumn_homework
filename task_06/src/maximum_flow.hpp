#pragma once

#include <unordered_map>
#include <vector>

struct Edge {
  int flow;
  int capacity;
};

struct MaxFlow {
  MaxFlow(std::vector<std::unordered_map<int, Edge>> graph) : graph(graph) {
    n = graph.size();
    visited.assign(n, false);
    FordFulkerson();
  }
  int max_flow;

 private:
  int source{-1}, drain{-1};
  std::vector<std::unordered_map<int, Edge>> graph;
  int n;
  int DFS(int u, int addition);
  void FindSourceAndDrain();
  void FordFulkerson();
  std::vector<bool> visited;
};