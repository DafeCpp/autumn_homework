#include "tarjan.hpp"

#include <algorithm>
#include <vector>

namespace {

void dfs(int vertex, const std::vector<std::vector<int>>& graph,
         std::vector<int>& tin, std::vector<int>& low,
         std::vector<int>& component, std::vector<int>& stack,
         std::vector<bool>& in_stack, GraphTrace& trace, int& timer,
         int& component_counter) {
  tin[vertex] = low[vertex] = timer++;

  stack.push_back(vertex);
  in_stack[vertex] = true;

  trace.Node(vertex + 1, "active");
  trace.Value(vertex + 1, "tin", tin[vertex]);
  trace.Value(vertex + 1, "low", low[vertex]);
  trace.Step("Начали обрабатывать вершину");

  for (int next : graph[vertex]) {
    if (tin[next] == -1) {
      dfs(next, graph, tin, low, component, stack, in_stack, trace, timer,
          component_counter);

      low[vertex] = std::min(low[vertex], low[next]);

      trace.Value(vertex + 1, "low", low[vertex]);
      trace.Step("Вернулись из рекурсивного вызова");
    } else if (in_stack[next]) {
      low[vertex] = std::min(low[vertex], tin[next]);

      trace.Value(vertex + 1, "low", low[vertex]);
      trace.Step("Нашли ребро в вершину из стека");
    }
  }

  if (low[vertex] == tin[vertex]) {
    while (true) {
      int current = stack.back();
      stack.pop_back();

      in_stack[current] = false;
      component[current] = component_counter;

      trace.Node(current + 1, "done");
      trace.Value(current + 1, "component", component_counter);
      trace.Step("Добавили вершину в компоненту");

      if (current == vertex) {
        break;
      }
    }

    ++component_counter;
  }
}

}  // namespace

void tarjan(const std::vector<std::vector<int>>& graph,
            std::vector<int>& component, GraphTrace& trace, int& timer,
            int& component_counter) {
  int vertex_count = graph.size();

  std::vector<int> tin(vertex_count, -1);
  std::vector<int> low(vertex_count, -1);
  std::vector<int> stack;
  std::vector<bool> in_stack(vertex_count, false);

  component.assign(vertex_count, -1);

  timer = 0;
  component_counter = 0;

  for (int vertex = 0; vertex < vertex_count; ++vertex) {
    if (tin[vertex] == -1) {
      dfs(vertex, graph, tin, low, component, stack, in_stack, trace, timer,
          component_counter);
    }
  }
}