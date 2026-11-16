#ifndef TASK_05
#define TASK_05

#include <vector>

struct Edge {
  int u;
  int v;
  int w;   // вес
  int id;  // номер
};

struct Res {
  bool success;
  int total_weight;
};

Res SolvedMST(int n, int m, int d, const std::vector<Edge>& edges);

#endif  
