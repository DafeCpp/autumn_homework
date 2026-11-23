#include "solution.hpp"

#include <numeric>
#include <utility>

class Solver {
 public:
  Solver(const Graph& graph) : n(graph.size()), d(graph.get_max_degree()) {
    int s = graph.get_special_vertex();
    for (const Edge& e : graph.get_edges()) {
      if (e.u == s || e.v == s) {
        incident.push_back(e);
      } else {
        other.push_back(e);
      }
    }
  }

  int solve() {
    // At the upper bound all other edges come before edges incident to s.
    int left = 0, right = other.empty() ? 0 : other.back().w;
    while (left < right) {
      int mid = left + (right - left) / 2;
      if (kruskal(mid).degree <= d) {
        right = mid;
      } else {
        left = mid + 1;
      }
    }
    auto tree = kruskal(left);
    if (tree.weight == -1 || tree.degree > d) return -1;
    // At the first positive feasible penalty, tied MSTs cover degree d:
    // exchanging equal-weight edges changes the degree by at most one.
    return tree.weight - left * d;
  }

 private:
  struct Result {
    int weight, degree;
  };

  int find(std::vector<int>& parent, int v) {
    while (parent[v] != v) {
      parent[v] = parent[parent[v]];
      v = parent[v];
    }
    return v;
  }

  Result kruskal(int penalty) {
    std::vector<int> parent(n), size(n, 1);
    std::iota(parent.begin(), parent.end(), 0);
    size_t i = 0, j = 0;
    int weight = 0, degree = 0, used = 0;
    while (used < n - 1 && (i < incident.size() || j < other.size())) {
      // Prefer other edges on a tie to obtain the smallest possible degree.
      bool take_incident =
          i < incident.size() &&
          (j == other.size() || incident[i].w + penalty < other[j].w);
      const Edge& e = take_incident ? incident[i++] : other[j++];
      int a = find(parent, e.u), b = find(parent, e.v);
      if (a == b) continue;
      if (size[a] < size[b]) std::swap(a, b);
      parent[b] = a;
      size[a] += size[b];
      ++used;
      weight += e.w;
      if (take_incident) {
        weight += penalty;
        ++degree;
      }
    }
    return {used == n - 1 ? weight : -1, degree};
  }

  int n, d;
  std::vector<Edge> incident, other;
};

int get_min_weight(Graph graph) {
  Solver solver(graph);
  return solver.solve();
}
