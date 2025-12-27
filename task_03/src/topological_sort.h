#ifndef TOPOLOGICAL_SORT_H
#define TOPOLOGICAL_SORT_H

#include <vector>
#include <utility>

class TopologicalSorter {
 public:
  static bool Sort(int n, const std::vector<std::pair<int, int>>& edges,
                   std::vector<int>& result);
};

#endif