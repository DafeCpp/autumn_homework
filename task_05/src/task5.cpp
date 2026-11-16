#include "task5.hpp"

#include <algorithm>
#include <vector>

namespace {

struct Dsu {
  std::vector<int> parent;

  explicit Dsu(int n) {
    parent.resize(n + 1);
    for (int i = 0; i <= n; ++i) {
      parent[i] = i;
    }
  }

  int find(int i) {
    if (parent[i] == i) {
      return i;
    }
    return parent[i] = find(parent[i]);
  }

  bool unite(int i, int j) {
    int root_i = find(i);
    int root_j = find(j);

    if (root_i != root_j) {
      parent[root_i] = root_j;
      return true;
    }
    return false;
  }
};

void dfs(int cur_v, int p,
         const std::vector<std::vector<std::pair<int, int>>>& adj,
         std::vector<int>& path_adges, int target,
         std::vector<int>& final_path, bool& found) {
  if (cur_v == target) {
    final_path = path_adges;
    found = true;
    return;
  }

  for (size_t i = 0; i < adj[cur_v].size(); ++i) {
    int neighbor = adj[cur_v][i].first;
    int edge_id = adj[cur_v][i].second;

    if (neighbor != p && !found) {
      path_adges.push_back(edge_id);
      dfs(neighbor, cur_v, adj, path_adges, target, final_path, found);
      path_adges.pop_back();
    }
  }
}

bool CompareEdges(const Edge& a, const Edge& b) { return a.w < b.w; }

}  // namespace

Res SolvedMST(int n, int m, int d, const std::vector<Edge>& edges) {
  std::vector<Edge> sorted_edges = edges;
  for (int i = 0; i < m; ++i) {
    sorted_edges[i].id = i;
  }

  std::sort(sorted_edges.begin(), sorted_edges.end(), CompareEdges);

  Dsu dsu(n);
  std::vector<bool> in_mst(m, false);
  std::vector<int> degree(n + 1, 0);
  int mst_weight = 0;
  int edges_count = 0;

  for (int i = 0; i < m; ++i) {
    if (sorted_edges[i].u == sorted_edges[i].v) continue;

    if (dsu.unite(sorted_edges[i].u, sorted_edges[i].v)) {
      in_mst[sorted_edges[i].id] = true;
      degree[sorted_edges[i].u]++;
      degree[sorted_edges[i].v]++;
      mst_weight += sorted_edges[i].w;
      edges_count++;
    }
  }

  if (edges_count < n - 1) {
    return {false, 0};
  }

  bool changed = true;
  while (changed) {
    changed = false;
    int max_violation = 0;
    int bad_vertex = -1;

    for (int i = 1; i <= n; ++i) {
      if (degree[i] > d && degree[i] > max_violation) {
        max_violation = degree[i];
        bad_vertex = i;
      }
    }

    if (bad_vertex == -1) break;

    int best_edge_to_add = -1;
    int best_edge_to_remove = -1;
    int min_weight_increase = 1e9;

    for (int i = 0; i < m; ++i) {
      if (in_mst[edges[i].id] || edges[i].u == edges[i].v) continue;

      if (degree[edges[i].u] >= d && edges[i].u != bad_vertex) continue;
      if (degree[edges[i].v] >= d && edges[i].v != bad_vertex) continue;

      std::vector<std::vector<std::pair<int, int>>> adj(n + 1);
      for (int j = 0; j < m; ++j) {
        if (in_mst[edges[j].id]) {
          adj[edges[j].u].push_back({edges[j].v, edges[j].id});
          adj[edges[j].v].push_back({edges[j].u, edges[j].id});
        }
      }

      std::vector<int> path_adges, final_path;
      bool found = false;
      dfs(edges[i].u, 0, adj, path_adges, edges[i].v, final_path, found);

      for (size_t k = 0; k < final_path.size(); ++k) {
        int edge_id = final_path[k];
        if (edges[edge_id].u == bad_vertex || edges[edge_id].v == bad_vertex) {
          int increase = edges[i].w - edges[edge_id].w;
          if (increase < min_weight_increase) {
            min_weight_increase = increase;
            best_edge_to_add = edges[i].id;
            best_edge_to_remove = edge_id;
          }
        }
      }
    }

    if (best_edge_to_add != -1 && best_edge_to_remove != -1) {
      in_mst[best_edge_to_add] = true;
      in_mst[best_edge_to_remove] = false;

      degree[edges[best_edge_to_add].u]++;
      degree[edges[best_edge_to_add].v]++;
      degree[edges[best_edge_to_remove].u]--;
      degree[edges[best_edge_to_remove].v]--;

      mst_weight += min_weight_increase;
      changed = true;
    }
  }

  for (int i = 1; i <= n; ++i) {
    if (degree[i] > d) {
      return {false, 0};
    }
  }

  return {true, mst_weight};
}
