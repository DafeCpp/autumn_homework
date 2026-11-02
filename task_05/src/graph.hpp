#pragma once

#include <iostream>
#include <vector>

struct Edge {
    int u, v, w;
};

class Graph {
  public:
    Graph();
    Graph(int n, std::vector<Edge> edges, int d);
    int size() const;
    int get_max_degree() const;
    const std::vector<Edge>& get_edges() const;

  private:
    std::vector<Edge> edges;
    int cnt_vertex;
    int max_degree;
};
