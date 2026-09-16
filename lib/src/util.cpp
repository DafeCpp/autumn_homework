#include "util.hpp"

class Graph {
  std::vector<std::vector<int>> list;
  std::vector<bool> visited;

 public:
  Graph();
  void Build(std::vector<std::pair<int, int>> &v) {
    for (int i = 0; i < v.size(); i++) {
      list[v[i].first].push_back(v[i].second);
    }
    visited.resize(v.size(), 0);
  }
  void Dfs(int cur) {
    visited[cur] = 1;
    for (auto next : list[cur]) {
      if (visited[next]) {
        continue;
      }
      Dfs(next);
    }
  }
  ~Graph();
};