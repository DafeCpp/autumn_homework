#include "task2.h"

#include <vector>
using namespace std;

enum Color { WHITE = 0, GRAY = 1, BLACK = 2 };

void DFS1(int u, vector<Color>& color, vector<vector<int>>& adj,
          vector<int>& order) {
  color[u] = GRAY;
  for (int v : adj[u]) {
    if (color[v] == WHITE) DFS1(v, color, adj, order);
  }
  order.push_back(u);
}

void DFS2(int u, vector<Color>& color, vector<vector<int>>& adj,
          int current_quarter, vector<int>& scc_id) {
  color[u] = GRAY;
  for (int v : adj[u]) {
    if (color[v] == WHITE) DFS2(v, color, adj, current_quarter, scc_id);
  }
  scc_id[u] = current_quarter;
}

int task2(int n, int m, vector<int>& arr) {
  vector<Color> color(n + 1, WHITE);
  vector<vector<int>> adj(n + 1);
  vector<vector<int>> adj_revers(n + 1);
  vector<int> order = {};

  for (int i = 0; i < 2 * m; i += 2) {
    int a = arr[i], b = arr[i + 1];

    adj[a].push_back(b);
    adj_revers[b].push_back(a);
  }

  for (int i = 1; i < n + 1; i++) {
    if (color[i] == WHITE) {
      DFS1(i, color, adj, order);
    }
  }
  for (int i = 0; i < color.size(); i++) {
    color[i] = WHITE;
  }
  vector<int> scc_id(n + 1, 0);
  int current_quarter = 0;
  for (int i = order.size() - 1; i >= 0; i--) {
    if (color[order[i]] == WHITE) {
      current_quarter++;
      DFS2(order[i], color, adj_revers, current_quarter, scc_id);
    }
  }

  vector<bool> has_in(current_quarter + 1, false);
  vector<bool> has_out(current_quarter + 1, false);

  for (int u = 1; u <= n; u++) {
    for (int v : adj[u]) {
      if (scc_id[u] != scc_id[v]) {
        has_out[scc_id[u]] = true;
        has_in[scc_id[v]] = true;
      }
    }
  }
  int zero_in = 0;
  int zero_out = 0;
  for (int i = 1; i < current_quarter + 1; i++) {
    if (!has_in[i]) zero_in++;
    if (!has_out[i]) zero_out++;
  }
  if (current_quarter == 1) return 0;
  return max(zero_in, zero_out);
}
