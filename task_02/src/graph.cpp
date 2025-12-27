#include "graph.h"

#include <algorithm>
#include <queue>
#include <stack>
#include <vector>

using std::max;
using std::pair;
using std::queue;
using std::reverse;
using std::stack;
using std::vector;

static void Dfs1(int v, const vector<vector<int>>& adj,
                 vector<bool>* visited, vector<int>* order) {
  stack<pair<int, int>> s;
  s.push({v, 0});
  while (!s.empty()) {
    int current = s.top().first;
    int& index = s.top().second;
    if (index == 0) {
      (*visited)[current] = true;
    }
    if (index < adj[current].size()) {
      int u = adj[current][index];
      index++;
      if (!(*visited)[u]) {
        s.push({u, 0});
      }
    } else {
      order->push_back(current);
      s.pop();
    }
  }
}

static void Dfs2(int v, const vector<vector<int>>& rev_adj,
                 vector<bool>* visited, vector<int>* component) {
  queue<int> q;
  q.push(v);
  (*visited)[v] = true;
  component->push_back(v);
  while (!q.empty()) {
    int current = q.front();
    q.pop();
    for (int u : rev_adj[current]) {
      if (!(*visited)[u]) {
        (*visited)[u] = true;
        component->push_back(u);
        q.push(u);
      }
    }
  }
}

int MinEdgesToStronglyConnected(int n,
                                const vector<pair<int, int>>& edges) {
  vector<vector<int>> adj(n);
  vector<vector<int>> rev_adj(n);

  for (const auto& e : edges) {
    int a = e.first - 1;
    int b = e.second - 1;
    adj[a].push_back(b);
    rev_adj[b].push_back(a);
  }

  vector<bool> visited(n, false);
  vector<int> order;
  for (int i = 0; i < n; ++i) {
    if (!visited[i]) {
      Dfs1(i, adj, &visited, &order);
    }
  }

  std::fill(visited.begin(), visited.end(), false);
  vector<int> comp_id(n);
  int comp_count = 0;
  reverse(order.begin(), order.end());
  for (int v : order) {
    if (!visited[v]) {
      vector<int> component;
      Dfs2(v, rev_adj, &visited, &component);
      for (int u : component) {
        comp_id[u] = comp_count;
      }
      comp_count++;
    }
  }

  if (comp_count == 1) {
    return 0;
  }

  vector<bool> has_incoming(comp_count, false);
  vector<bool> has_outgoing(comp_count, false);
  for (int v = 0; v < n; ++v) {
    for (int u : adj[v]) {
      if (comp_id[v] != comp_id[u]) {
        has_outgoing[comp_id[v]] = true;
        has_incoming[comp_id[u]] = true;
      }
    }
  }

  int sources = 0;
  int sinks = 0;
  for (int i = 0; i < comp_count; ++i) {
    if (!has_incoming[i]) sources++;
    if (!has_outgoing[i]) sinks++;
  }

  return max(sources, sinks);
}