#include "task1.h"

#include <iostream>
#include <vector>
using namespace std;

enum Color { WHITE = 0, GRAY = 1, BLACK = 2 };

vector<int> result;

bool DFS(int u, vector<Color>& color, vector<vector<int>>& adj) {
  color[u] = GRAY;
  for (int v : adj[u]) {
    if (color[v] == WHITE) {
      if (DFS(v, color, adj)) return true;

    } else if (color[v] == GRAY)
      return true;
  }
  color[u] = BLACK;
  result.push_back(u);
  return false;
}

vector<int> task1(int n, int m, vector<int>& arr) {
  result.clear();
  vector<Color> color(n, WHITE);
  vector<vector<int>> adj(n);
  if (arr.size() == 1) return {-1};
  for (int i = 0; i < 2 * m; i += 2) {
    int a = arr[i], b = arr[i + 1];

    adj[a].push_back(b);
  }

  for (int i = 0; i < n; i++) {
    if (color[i] == WHITE) {
      if (DFS(i, color, adj)) {
        cout << -1;
        return {-1};
      }
    }
  }
  return result;
}