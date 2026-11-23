#include "solution.hpp"

int main() {
  Graph graph;
  Matrix result = johnson(graph);
  if (result.empty()) {
    std::cout << "NEGATIVE CYCLE" << std::endl;
    return 0;
  }
  for (const auto& row : result) {
    for (size_t i = 0; i < row.size(); ++i) {
      if (i) std::cout << " ";
      if (row[i] == INF) {
        std::cout << "INF";
      } else {
        std::cout << row[i];
      }
    }
    std::cout << std::endl;
  }
}
