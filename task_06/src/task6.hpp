#ifndef TASK_06
#define TASK_06

#include <vector>

struct Edge {
  int u;
  int v;
  int w;   // вес
  int id;  // номер
};

struct Res {
  int max_flow;
};

Res Solve(int n, int m, const std::vector<Edge>& edges);

#endif
