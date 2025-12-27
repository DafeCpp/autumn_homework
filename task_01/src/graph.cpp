#include "graph.h"

#include <algorithm>
#include <iostream>


using std::cout;
using std::min;
using std::sort;
using std::vector;
using std::max;

Graph::Graph(int vertex) : n_(vertex), adj_(vertex) {}

void Graph::AddEdge(int u, int v) {
  if (u < 0 || u >= n_ || v < 0 || v >= n_) return;
  adj_[u].push_back(v);
  adj_[v].push_back(u);
}

void Graph::FindCriticalComponents() {
  articulation_points_.assign(n_, 0);
  bridges_.clear();
  
  vector<bool> visited(n_, false);
  vector<int> tin(n_, -1);
  vector<int> low(n_, -1);
  timer_ = 0;
  
  for (int i = 0; i < n_; i++) {
    if (!visited[i]) {
      Dfs(i, -1, &visited, &tin, &low);
    }
  }
  
  sort(bridges_.begin(), bridges_.end());
}

void Graph::Dfs(int v, int p, vector<bool>* visited, vector<int>* tin,
                vector<int>* low) {
  if (v < 0 || v >= n_) return;
  
  (*visited)[v] = true;
  (*tin)[v] = (*low)[v] = timer_++;
  int children = 0;
  
  for (int to : adj_[v]) {
    if (to < 0 || to >= n_) continue;
    if (to == p) continue;
    
    if ((*visited)[to]) {
      (*low)[v] = min((*low)[v], (*tin)[to]);
    } else {
      Dfs(to, v, visited, tin, low);
      (*low)[v] = min((*low)[v], (*low)[to]);
      
      if ((*low)[to] > (*tin)[v]) {
        bridges_.push_back({min(v, to), max(v, to)});
      }
      
      if ((*low)[to] >= (*tin)[v] && p != -1) {
        articulation_points_[v] = 1;
      }
      children++;
    }
  }
  
  if (p == -1 && children > 1) {
    articulation_points_[v] = 1;
  }
}

void Graph::PrintResults() {
  FindCriticalComponents();
  
  
  int ap_count = 0;
  vector<int> ap_list;
  for (int i = 0; i < n_; i++) {
    if (articulation_points_[i]) {
      ap_count++;
      ap_list.push_back(i + 1);  // +1 для 1-based вывода
    }
  }
  cout << ap_count << "\n";
  
  
  if (ap_count == 0) {
    cout << "-\n";
  } else {
    
    for (size_t i = 0; i < ap_list.size(); i++) {
      if (i > 0) cout << " ";
      cout << ap_list[i];
    }
    cout << "\n";
  }
  

  cout << bridges_.size() << "\n";
  

  if (bridges_.empty()) {
    cout << "-\n";
  } else {
    for (size_t i = 0; i < bridges_.size(); i++) {
      // +1 для 1-based вывода
      cout << (bridges_[i].first + 1) << " " << (bridges_[i].second + 1);
      if (i != bridges_.size() - 1) {
        cout << "; ";
      }
    }
    cout << "\n";
  }
}