#include <string>
#include <vector>

bool DFS(int u, const std::vector<std::vector<int>>& graf,
         std::vector<std::string>& colors, std::vector<int>& result) {
  colors[u] = "gray";

  for (auto v : graf[u]) {
    if (colors[v] == "white") {
      if (!DFS(v, graf, colors, result)) return false;
    } else if (colors[v] == "gray") {
      return false;
    }
  }
  colors[u] = "black";
  result.push_back(u);
  return true;
}

std::vector<int> TopologySort(const std::vector<std::vector<int>>& graf) {
  std::vector<std::string> colors(graf.size(), "white");
  std::vector<int> result;

  for (int u = 0; u < graf.size(); ++u) {
    if (colors[u] == "white") {
      std::vector<int> connected_component;
      if (!DFS(u, graf, colors, connected_component)) return {-1};
      result.insert(result.end(), connected_component.begin(),
                    connected_component.end());
    }
  }
  return result;
}
