#include "scc.hpp"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <utility>

namespace scc_example {
namespace {
using Adjacency = std::vector<std::vector<std::pair<int, int>>>;
struct Frame {
  int v;
  std::size_t next = 0;
};
Adjacency Build(int n, const std::vector<Edge>& edges,
                graph_trace::Recorder* trace) {
  if (n < 1) throw std::invalid_argument("n must be positive");
  Adjacency graph(n + 1);
  for (int id = 0; id < static_cast<int>(edges.size()); ++id) {
    const auto [u, v] = edges[id];
    if (u < 1 || u > n || v < 1 || v > n)
      throw std::invalid_argument("vertex out of range");
    graph[u].push_back({v, id});
  }
  if (trace) {
    trace->Graph(n, true);
    for (int id = 0; id < static_cast<int>(edges.size()); ++id)
      trace->AddEdge(id, edges[id].from, edges[id].to);
  }
  return graph;
}
std::vector<int> Canonical(std::vector<int> component) {
  std::vector<int> minimum(component.size(),
                           static_cast<int>(component.size()));
  for (int v = 1; v < static_cast<int>(component.size()); ++v)
    minimum[component[v]] = std::min(minimum[component[v]], v);
  for (int v = 1; v < static_cast<int>(component.size()); ++v)
    component[v] = minimum[component[v]];
  return component;
}
}  // namespace

std::vector<int> Kosaraju(int n, const std::vector<Edge>& edges,
                          graph_trace::Recorder* trace) {
  const auto graph = Build(n, edges, trace);
  Adjacency reverse(n + 1);
  for (int id = 0; id < static_cast<int>(edges.size()); ++id)
    reverse[edges[id].to].push_back({edges[id].from, id});
  std::vector<int> seen(n + 1), order, component(n + 1);
  std::vector<Frame> dfs;
  if (trace) trace->Step("Косарайю: первый DFS на исходном графе");
  auto enter = [&](int v) {
    seen[v] = 1;
    dfs.push_back({v});
    if (trace) {
      trace->Node(v, "active");
      trace->Step("Первый проход: вход в вершину " + std::to_string(v));
    }
  };
  for (int root = 1; root <= n; ++root) {
    if (seen[root]) continue;
    enter(root);
    while (!dfs.empty()) {
      auto& frame = dfs.back();
      const int v = frame.v;
      if (frame.next < graph[v].size()) {
        const auto [to, id] = graph[v][frame.next++];
        if (!seen[to]) {
          if (trace) {
            trace->Edge(id, "tree");
            trace->Value(to, "parent", v);
          }
          enter(to);
        } else if (trace) {
          trace->Edge(id, seen[to] == 1 ? "back" : "non_tree");
          trace->Step("Ребро к уже посещённой вершине " + std::to_string(to));
        }
      } else {
        order.push_back(v);
        seen[v] = 2;
        dfs.pop_back();
        if (trace) {
          trace->Node(v, "done");
          trace->Value(v, "finish", static_cast<int>(order.size()));
          trace->Step("Выход из " + std::to_string(v) +
                      ": добавляем в порядок выхода");
        }
      }
    }
  }
  if (trace) {
    // Этот маркер переключает стрелки и очищает лес первого прохода.
    trace->Value(1, "transpose", 1);
    for (int v = 1; v <= n; ++v) trace->Node(v, "idle");
    trace->Step(
        "Разворачиваем все рёбра. Второй DFS: в обратном порядке выхода");
  }
  int count = 0;
  for (auto it = order.rbegin(); it != order.rend(); ++it) {
    if (component[*it]) continue;
    ++count;
    auto assign = [&](int v) {
      component[v] = count;
      dfs.push_back({v});
      if (trace) {
        trace->Node(v, "active");
        trace->Value(v, "component", count);
        trace->Step("Обратный граф: вершина " + std::to_string(v) +
                    " входит в КСС " + std::to_string(count));
      }
    };
    assign(*it);
    while (!dfs.empty()) {
      auto& frame = dfs.back();
      const int v = frame.v;
      if (frame.next < reverse[v].size()) {
        const auto [to, id] = reverse[v][frame.next++];
        if (!component[to]) {
          if (trace) {
            trace->Edge(id, "tree");
            trace->Value(to, "parent", v);
          }
          assign(to);
        }
      } else {
        dfs.pop_back();
        if (trace) {
          trace->Node(v, "done");
          trace->Step("Второй проход: выход из " + std::to_string(v));
        }
      }
    }
    if (trace) trace->Step("Завершена КСС " + std::to_string(count));
  }
  if (trace)
    trace->Step("Косарайю завершён. Компонент: " + std::to_string(count));
  return Canonical(std::move(component));
}

std::vector<int> Tarjan(int n, const std::vector<Edge>& edges,
                        graph_trace::Recorder* trace) {
  const auto graph = Build(n, edges, trace);
  std::vector<int> index(n + 1), low(n + 1), on_stack(n + 1), component(n + 1),
      stack;
  std::vector<Frame> dfs;
  int timer = 0, count = 0;
  auto enter = [&](int v) {
    index[v] = low[v] = ++timer;
    on_stack[v] = 1;
    stack.push_back(v);
    dfs.push_back({v});
    if (trace) {
      trace->Node(v, "active");
      trace->Value(v, "index", index[v]);
      trace->Value(v, "lowlink", low[v]);
      trace->Value(v, "on_stack", 1);
      trace->Value(v, "stack_position", static_cast<int>(stack.size()));
      trace->Step("Вход в " + std::to_string(v) +
                  ": index = lowlink, кладём в стек КСС");
    }
  };
  if (trace)
    trace->Step(
        "Тарьян: один DFS; стек КСС хранится отдельно от стека вызовов");
  for (int root = 1; root <= n; ++root) {
    if (index[root]) continue;
    enter(root);
    while (!dfs.empty()) {
      auto& frame = dfs.back();
      const int v = frame.v;
      if (frame.next < graph[v].size()) {
        const auto [to, id] = graph[v][frame.next++];
        if (!index[to]) {
          if (trace) {
            trace->Edge(id, "tree");
            trace->Value(to, "parent", v);
          }
          enter(to);
        } else {
          // Важно: к вершине в стеке берём index[to], а не low[to].
          if (on_stack[to]) low[v] = std::min(low[v], index[to]);
          if (trace) {
            trace->Edge(id, "non_tree");
            trace->Value(v, "lowlink", low[v]);
            trace->Step(
                on_stack[to]
                    ? "Ребро в стек: lowlink[" + std::to_string(v) +
                          "] = min(lowlink, index[" + std::to_string(to) + "])"
                    : "Ребро в уже выделенную КСС: lowlink не меняется");
          }
        }
      } else {
        dfs.pop_back();
        if (low[v] == index[v]) {
          ++count;
          if (trace)
            trace->Step("lowlink = index у " + std::to_string(v) +
                        ": найдена корневая вершина КСС");
          int to;
          do {
            to = stack.back();
            stack.pop_back();
            on_stack[to] = 0;
            component[to] = count;
            if (trace) {
              trace->Node(to, "done");
              trace->Value(to, "on_stack", 0);
              trace->Value(to, "stack_position", 0);
              trace->Value(to, "component", count);
              trace->Step("Снимаем " + std::to_string(to) + " со стека в КСС " +
                          std::to_string(count));
            }
          } while (to != v);
        } else if (trace) {
          trace->Node(v, "queued");
          trace->Step("Выход из " + std::to_string(v) +
                      ": вершина остаётся в стеке КСС");
        }
        if (!dfs.empty()) {
          const int parent = dfs.back().v;
          low[parent] = std::min(low[parent], low[v]);
          if (trace) {
            trace->Value(parent, "lowlink", low[parent]);
            trace->Step("Возврат из " + std::to_string(v) +
                        ": lowlink родителя " + std::to_string(parent) +
                        " = min(lowlink родителя, lowlink ребёнка)");
          }
        }
      }
    }
  }
  if (trace)
    trace->Step("Тарьян завершён. Компонент: " + std::to_string(count));
  return Canonical(std::move(component));
}
}  // namespace scc_example
