#include "topological_sort.h"

#include <queue>
#include <vector>

bool TopologicalSorter::Sort(int n,
                             const std::vector<std::pair<int, int>>& edges,
                             std::vector<int>& result) {
  result.clear();
  std::vector<std::vector<int>> graph(n + 1);
  std::vector<int> indegree(n + 1, 0);

  for (const auto& edge : edges) {
    graph[edge.first].push_back(edge.second);
    indegree[edge.second]++;
  }

  std::queue<int> q;
  for (int i = 1; i <= n; ++i) {
    if (indegree[i] == 0) {
      q.push(i);
    }
  }

  while (!q.empty()) {
    int current = q.front();
    q.pop();
    result.push_back(current);

    for (int neighbor : graph[current]) {
      indegree[neighbor]--;
      if (indegree[neighbor] == 0) {
        q.push(neighbor);
      }
    }
  }

  return result.size() == n;
}