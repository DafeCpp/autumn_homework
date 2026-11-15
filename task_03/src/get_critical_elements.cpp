#include "get_critical_elements.hpp"

void AdditionalRoads::DFS(int u, int from) {
  visited[u] = true;
  tin[u] = ++timer;
  low[u] = tin[u];

  int children_amount = 0;

  for (int to : graf[u]) {
    if (to == from) continue;
    if (visited[to]) {
      low[u] = std::min(low[u], tin[to]);
    } else {
      DFS(to, u);
      ++children_amount;
      low[u] = std::min(low[u], low[to]);

      if (low[to] > tin[u])
        result.critical_wiring.insert({std::min(u, to), std::max(u, to)});
      if (from != -1 && low[to] >= tin[u]) result.critical_devices.insert(u);
    }
  }
  if (from == -1 && children_amount >= 2) result.critical_devices.insert(u);
}

CriticalElements AdditionalRoads::GetCriticalElements() {
  for (int i = 0; i < graf.size(); ++i) {
    if (!visited[i]) DFS(i, -1);
  }

  return result;
}