#include "additional_roads.hpp"

void AdditionalRoads::GetSCC() {
  for (int i = 0; i < graf.size(); ++i) {
    if (tin[i] == 2e9) DFS(i);
  }
}

void AdditionalRoads::DFS(int u) {
  tin[u] = ++timer;
  low[u] = tin[u];
  stack.push(u);
  on_stack[u] = true;

  for (int v : graf[u]) {
    if (tin[v] == 2e9) {
      DFS(v);
      low[u] = std::min(low[u], low[v]);
    } else if (on_stack[v]) {
      low[u] = std::min(low[u], tin[v]);
    }
  }
  if (tin[u] == low[u]) {
    components.push_back({});
    while (stack.top() != u) {
      components.back().push_back(stack.top());
      on_stack[stack.top()] = false;
      stack.pop();
    }
    components.back().push_back(u);
    on_stack[u] = false;
    stack.pop();
  }
}

int AdditionalRoads::GetAmountNewRoads() {
  GetSCC();
  if (components.size() == 1) return 0;

  for (int i = 0; i < components.size(); ++i) {
    for (int u : components[i]) components_id[u] = i;
  }

  has_in.assign(components.size(), false);
  has_out.assign(components.size(), false);

  for (int u = 0; u < graf.size(); ++u) {
    for (int v : graf[u]) {
      if (components_id[u] != components_id[v]) {
        has_out[components_id[u]] = true;
        has_in[components_id[v]] = true;
      }
    }
  }

  int not_in{0}, not_out{0};
  for (int i = 0; i < components.size(); ++i) {
    if (!has_in[i]) ++not_in;
    if (!has_out[i]) ++not_out;
  }

  return (std::max(not_in, not_out));
}