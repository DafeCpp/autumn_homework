#ifndef MAXFLOW_H
#define MAXFLOW_H

#include <vector>

class MaxFlow {
 public:
  MaxFlow(int n, const std::vector<std::vector<int>>& edges);
  int Calculate(int source, int sink);

 private:
  bool BFS(int source, int sink, std::vector<int>& parent);
  int n_;
  std::vector<std::vector<int>> capacity_;
  std::vector<std::vector<int>> adj_;
};

#endif