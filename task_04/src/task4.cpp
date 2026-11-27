#include "task4.h"

#include <queue>
#include <string>
#include <vector>
using namespace std;

const long long INF = 1e18;

bool bellman_ford(int n, const vector<Edge>& edges, vector<long long>& h) {
  h.assign(n + 1, 0);

  for (int i = 1; i <= n; ++i) {
    bool changed = false;
    for (const auto& edge : edges) {
      if (h[edge.u] + edge.w < h[edge.v]) {
        h[edge.v] = h[edge.u] + edge.w;
        changed = true;
      }
    }
    if (!changed) {
      return false;
    }
    if (i == n && changed) {
      return true;
    }
  }
  return false;
}

vector<long long> dijkstra(int start, int n,
                           const vector<vector<pair<int, long long>>>& adj) {
  vector<long long> dist(n + 1, INF);
  dist[start] = 0;

  priority_queue<pair<long long, int>, vector<pair<long long, int>>,
                 greater<pair<long long, int>>>
      pq;

  pq.push({0, start});

  while (!pq.empty()) {
    long long d = pq.top().first;
    int u = pq.top().second;
    pq.pop();

    if (d > dist[u]) continue;

    for (const auto& edge : adj[u]) {
      int v = edge.first;
      long long weight = edge.second;

      if (dist[u] + weight < dist[v]) {
        dist[v] = dist[u] + weight;
        pq.push({dist[v], v});
      }
    }
  }
  return dist;
}

vector<string> task4(int n, int m, vector<Edge>& edges) {
  vector<long long> h;

  if (bellman_ford(n, edges, h)) {
    return {"NEGATIVE CYCLE"};
  }

  vector<vector<pair<int, long long>>> adj(n + 1);
  for (int i = 0; i < m; ++i) {
    int u = edges[i].u;
    int v = edges[i].v;
    long long w = edges[i].w;
    long long new_w = w + h[u] - h[v];
    adj[u].push_back({v, new_w});
  }

  vector<string> result;

  for (int start = 1; start <= n; ++start) {
    vector<long long> dist = dijkstra(start, n, adj);

    string line = "";
    for (int j = 1; j <= n; ++j) {
      if (dist[j] == INF) {
        line += "INF";
      } else {
        long long real_dist = dist[j] - h[start] + h[j];
        line += to_string(real_dist);
      }
      if (j < n) line += " ";
    }
    result.push_back(line);
  }

  return result;
}