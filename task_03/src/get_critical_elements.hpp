#include <set>
#include <vector>

class AdditionalRoads {
 public:
  std::set<int> critical_devices;
  std::set<std::pair<int, int>> critical_wiring;

  AdditionalRoads(const std::vector<std::set<int>>& g) {
    graf = g;
    visited.assign(graf.size(), false);
    tin.assign(graf.size(), 2e9);
    low.assign(graf.size(), 2e9);

    std::pair<std::set<int>, std::set<std::pair<int, int>>> critical_elements =
        GetCriticalElements();
    critical_devices = critical_elements.first;
    critical_wiring = critical_elements.second;
  };

 private:
  int timer{-1};
  std::vector<std::set<int>> graf;
  std::vector<bool> visited;
  std::vector<int> tin, low;

  void DFS(int u, int from);
  std::pair<std::set<int>, std::set<std::pair<int, int>>> GetCriticalElements();
};
