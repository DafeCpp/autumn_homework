#ifndef JOHNSON_H
#define JOHNSON_H

#include <vector>

class Johnson {
 public:
  Johnson(int vertex_count, const std::vector<std::vector<int>>& edges);
  std::vector<std::vector<int>> Run();

 private:
  bool BellmanFord(std::vector<int>& distances);
  std::vector<int> Dijkstra(int source,
                            const std::vector<std::vector<std::pair<int, int>>>& graph);
  int vertex_count_;
  std::vector<std::vector<int>> edges_;
};

#endif