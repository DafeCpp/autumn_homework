#ifndef TASK_04
#define TASK_04

#include <vector>

struct Edge {
  int from;
  int to;
  long long weight;
};

std::vector<std::vector<long long>> Johnson(int n,
                                            const std::vector<Edge>& edges);

#endif
