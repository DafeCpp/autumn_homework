#pragma once

#include <iostream>
#include <utility>
#include <vector>

class Graph {
  public:
    Graph();
    Graph(int n, std::vector<std::vector<int> > edges);
    int size() const;
    const std::vector<std::vector<std::pair<int, long long> > >& get_adjacency() const;
    bool get_potentials(std::vector<long long>& h) const;

  private:
    std::vector<std::vector<std::pair<int, long long> > > graph;
    int cnt_vertex;
};
