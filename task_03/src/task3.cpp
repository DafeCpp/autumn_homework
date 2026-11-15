#include "task3.h"
#include <vector>
#include <set>
#include <algorithm>
#include <string>
using namespace std;

enum Color { WHITE = 0, GRAY = 1, BLACK = 2 };
vector<vector<int>> resultRibs = {};
set<int> resultVertexes = {};
int time = 0;

void multiDFS(int u, vector<vector<int>>& adj, vector<int>& disc, vector<int>& low, int parent, int& time) {
    disc[u] = time; low[u] = time; 
    time++;
    int children = 0;
    for (int v: adj[u]){
        if (disc[v] == 0){
            children++;
            multiDFS(v, adj, disc, low, u, time);
            low[u] = min(low[u], low[v]);
            if (low[v] > disc[u]){
                resultRibs.push_back({u,v});
            }
            if (parent == 0 && children>1){
              resultVertexes.insert(u);
            }
            if (parent != 0 && low[v] >= disc[u]){
              resultVertexes.insert(u);
            }

        } else if (v != parent){
            low[u] = min(low[u], disc[v]);
        }
    }
}

vector<string> task3(int n, int m, vector<int>& arr) {
  vector<vector<int>> adj(n+1);
  vector<int> disc(n+1, 0);
  vector<int> low(n+1);
  time = 1;
  resultRibs = {};
  resultVertexes = {};

  for (int i = 0; i < 2 * m; i += 2) {
    int a = arr[i], b = arr[i + 1];

    adj[a].push_back(b);
    adj[b].push_back(a);
  }

  for (int i = 1; i < n+1; i++) {
    if (disc[i] == 0) {
      multiDFS(i, adj, disc, low,  0, time);
    }
  }


  for (auto& edge : resultRibs) {
      if (edge[0] > edge[1]) {
          swap(edge[0], edge[1]);
      }
  }
  sort(resultRibs.begin(), resultRibs.end());

  vector<string> answer;

  answer.push_back(to_string(resultVertexes.size()));
  
  if (resultVertexes.empty()) {
      answer.push_back("-");
  } else {
      string vertex_str = "";
      bool first = true;
      for (int v : resultVertexes) {
          if (!first) vertex_str += " ";
          vertex_str += to_string(v);
          first = false;
      }
      answer.push_back(vertex_str);
  }

  answer.push_back(to_string(resultRibs.size()));
  
  if (resultRibs.empty()) {
      answer.push_back("-");
  } else {
      string ribs_str = "";
      bool first = true;
      for (const auto& edge : resultRibs) {
          if (!first) ribs_str += "; ";
          ribs_str += to_string(edge[0]) + " " + to_string(edge[1]);
          first = false;
      }
      answer.push_back(ribs_str);
  }

  return answer;


}
