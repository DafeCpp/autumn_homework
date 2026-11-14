#pragma once

#include <cstdint>
#include <optional>
#include <vector>

class Graph {
public:
  explicit Graph(int vertices);
  void AddEdge(int u, int v);
  std::optional<std::vector<int>> TopologicalSort() const;

private:
  int num_vertices_;
  std::vector<std::vector<int>> adj_list_;
  enum class Color : std::uint8_t { White, Gray, Black };
};
