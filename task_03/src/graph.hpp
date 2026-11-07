#pragma once

#include <iostream>
#include <utility>
#include <vector>

class Graph {
  public:
    Graph();
    Graph(int n, std::vector<std::pair<int, int> > edges);
    void dfs(int v, int parent);
    std::vector<int> get_cut_vertices();
    std::vector<std::pair<int, int> > get_bridges();

  private:
    void run();

    std::vector<std::vector<int> > graph;
    std::vector<bool> visited;
    std::vector<bool> is_cut;
    std::vector<int> tin;
    std::vector<int> low;
    std::vector<std::pair<int, int> > bridges;
    int timer;
    int cnt_vertex;
};
