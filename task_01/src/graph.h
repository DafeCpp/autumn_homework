#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <utility>

class Graph {
 public:
  explicit Graph(int vertices);
  void AddEdge(int u, int v);
  void PrintResults();

 private:
  void FindCriticalComponents();
  void Dfs(int v, int p, std::vector<bool>* visited, 
           std::vector<int>* tin, std::vector<int>* low);

  int n_;
  std::vector<std::vector<int>> adj_;
  int timer_;
  std::vector<int> articulation_points_;
  std::vector<std::pair<int, int>> bridges_;
};

#endif  // GRAPH_H