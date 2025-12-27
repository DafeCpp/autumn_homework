#ifndef LCA_H
#define LCA_H

#include <vector>

class LCA {
 public:
  LCA(int n, const std::vector<std::vector<int>>& graph);
  void Preprocess(int root);
  int Query(int u, int v) const;

 private:
  void Dfs(int u, int parent, int depth);
  int n_;
  int log_n_;
  std::vector<std::vector<int>> graph_;
  std::vector<std::vector<int>> up_;
  std::vector<int> tin_;
  std::vector<int> tout_;
  std::vector<int> depth_;
  int timer_;
};

#endif