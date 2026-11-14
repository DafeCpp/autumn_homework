#ifndef TASK_03
#define TASK_03

#include <vector>

struct NetworkResult {
  std::vector<int> critical_routers;
  std::vector<std::pair<int, int>> critical_cables;
};

NetworkResult FindWeakPoints(int n,
                             const std::vector<std::pair<int, int>>& edges);

#endif
