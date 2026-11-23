#pragma once

#include <iostream>
#include <vector>

struct Edge {
  int to;
  long long cap;
};

class Graph {
 public:
  Graph();
  Graph(int n, std::vector<std::vector<long long> > pipes);
  int size() const;
  bool bfs(int s, int t);
  long long dfs(int v, int t, long long pushed);
  long long get_max_flow();
  long long get_max_flow(int s, int t);

 private:
  void add_edge(int a, int b, long long c);

  std::vector<Edge> edges;
  std::vector<std::vector<int> > graph;
  std::vector<int> level;
  std::vector<int> next;
  int cnt_vertex;
};
