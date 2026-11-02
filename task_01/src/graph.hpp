#pragma once

#include <iostream>
#include <vector>

class Graph {
  public:
    Graph();
    Graph(std::vector<std::vector<int> > gr): graph(gr), have_cycle(false), cnt_vertex(gr.size()) {}
    void dfs(int v);
    std::vector<int> get_top_sort();

  private:
    std::vector<std::vector<int> > graph;
    bool have_cycle;
    std::vector<int> color;
    std::vector<int> top_sort;
    int cnt_vertex;
};