#pragma once

#include <iostream>
#include <vector>

class Graph {
  public:
    Graph();
    Graph(std::vector<std::vector<int> > gr): graph(gr), cnt_vertex(gr.size()) {}
    void dfs(int v);
    int get_scc(std::vector<int>& result);
    const std::vector<std::vector<int> >& get_adjacency() const;
    int size() const;

  private:
    std::vector<std::vector<int> > graph;
    std::vector<int> tin;
    std::vector<int> low;
    std::vector<int> comp;
    std::vector<bool> on_stack;
    std::vector<int> scc_stack;
    int timer;
    int cnt_comp;
    int cnt_vertex;
};
