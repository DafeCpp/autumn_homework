#pragma once

#include <set>
#include <vector>

struct CriticalElements {
  std::set<int> critical_devices;
  std::set<std::pair<int, int>> critical_wiring;
};

class AdditionalRoads {
 public:
  std::set<int> critical_devices;
  std::set<std::pair<int, int>> critical_wiring;

  AdditionalRoads(const std::vector<std::set<int>>& g) {
    graf = g;
    visited.assign(graf.size(), false);
    tin.assign(graf.size(), 2e9);
    low.assign(graf.size(), 2e9);

    CriticalElements critical_elements = GetCriticalElements();
    critical_devices = critical_elements.critical_devices;
    critical_wiring = critical_elements.critical_wiring;
  };

 private:
  int timer{-1};
  std::vector<std::set<int>> graf;
  std::vector<bool> visited;
  std::vector<int> tin, low;
  CriticalElements result;

  void DFS(int u, int from);
  CriticalElements GetCriticalElements();
};
