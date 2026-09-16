#pragma once

#include <vector>

#include "graph_trace.hpp"

namespace scc_example {
struct Edge {
  int from, to;
};
// Вершины 1..n; результат размера n+1, элемент 0 не используется.
// Метка компоненты — минимальный номер её вершины, независимо от алгоритма.
// Оба алгоритма: O(V+E) времени и памяти, без рекурсии.
std::vector<int> Kosaraju(int n, const std::vector<Edge>& edges,
                          graph_trace::Recorder* trace = nullptr);
std::vector<int> Tarjan(int n, const std::vector<Edge>& edges,
                        graph_trace::Recorder* trace = nullptr);
}  // namespace scc_example
