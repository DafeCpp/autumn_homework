#include <algorithm>
#include <iostream>
#include <vector>

#include "tarjan.hpp"

int main() {
  int n, m;
  std::cin >> n >> m;

  std::vector<std::vector<int>> graph(n);

  GraphTrace trace;

  trace.Graph(n, true, 1);

  for (int edge = 0; edge < m; ++edge) {
    int from, to;
    std::cin >> from >> to;

    trace.AddEdge(edge, from, to);

    graph[from - 1].push_back(to - 1);
  }

  std::vector<int> component;

  int timer = 0;
  int component_counter = 0;

  tarjan(graph, component, trace, timer, component_counter);

  trace.Step("Алгоритм завершён");
  trace.Finish();

  if (component_counter == 1) {
    std::cout << 0 << '\n';
    return 0;
  }

  std::vector<bool> has_incoming(component_counter, false);
  std::vector<bool> has_outgoing(component_counter, false);

  for (int vertex = 0; vertex < n; ++vertex) {
    for (int next : graph[vertex]) {
      int from_component = component[vertex];
      int to_component = component[next];

      if (from_component != to_component) {
        has_outgoing[from_component] = true;
        has_incoming[to_component] = true;
      }
    }
  }

  int source_count = 0;
  int sink_count = 0;

  for (int current = 0; current < component_counter; ++current) {
    if (!has_incoming[current]) {
      ++source_count;
    }

    if (!has_outgoing[current]) {
      ++sink_count;
    }
  }

  std::cout << std::max(source_count, sink_count);
  return 0;
}