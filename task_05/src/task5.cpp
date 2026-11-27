#include <queue>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

vector<string> task5(int n, int m, int d, vector<int>& arr) {
  if (n == 1) {
    return {"0"};
  }

  vector<vector<pair<int, int>>> adj(n + 1);
  for (int i = 0; i < m * 3; i += 3) {
    int u = arr[i];
    int v = arr[i + 1];
    int w = arr[i + 2];
    adj[u].push_back({w, v});
    adj[v].push_back({w, u});
  }

  vector<bool> visited(n + 1, false);
  vector<int> degree(n + 1, 0);

  priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>,
                 greater<tuple<int, int, int>>>
      pq;

  int start_node = 1;
  visited[start_node] = true;
  for (const auto& edge : adj[start_node]) {
    int next_w = edge.first;
    int next_v = edge.second;
    pq.push({next_w, start_node, next_v});
  }

  long long mst_weight = 0;
  int edges_taken = 0;

  while (!pq.empty() && edges_taken < n - 1) {
    auto [w, u, v] = pq.top();
    pq.pop();

    if (visited[v]) continue;
    if (degree[u] >= d) continue;

    visited[v] = true;
    degree[u]++;
    degree[v]++;
    mst_weight += w;
    edges_taken++;

    for (auto edge : adj[v]) {
      int next_w = edge.first;
      int next_v = edge.second;
      if (!visited[next_v]) {
        pq.push({next_w, v, next_v});
      }
    }
  }

  if (edges_taken == n - 1) {
    return {to_string(mst_weight)};
  } else {
    return {
        "Невозможно построить остовное дерево с заданным ограничением степени"};
  }
}