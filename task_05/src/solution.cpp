#include "solution.hpp"

#include <numeric>

const int FREE = 0;
const int FORCED = 1;
const int BANNED = 2;

class Solver {
  public:
    Solver(const Graph& graph)
        : edges(graph.get_edges()),
          n(graph.size()),
          d(graph.get_max_degree()),
          state(edges.size(), FREE),
          forced_degree(n, 0),
          best(-1) {}

    int solve() {
        if (n == 1) return 0;
        dfs();
        return best;
    }

  private:
    int find(std::vector<int>& parent, int v) {
        while (parent[v] != v) {
            parent[v] = parent[parent[v]];
            v = parent[v];
        }
        return v;
    }

    void dfs() {
        std::vector<int> parent(n);
        std::iota(parent.begin(), parent.end(), 0);
        std::vector<int> tree_degree(n, 0);
        std::vector<int> tree_edges;
        int total = 0;

        for (size_t i = 0; i < edges.size(); ++i) {
            if (state[i] != FORCED) continue;
            parent[find(parent, edges[i].u)] = find(parent, edges[i].v);
            ++tree_degree[edges[i].u];
            ++tree_degree[edges[i].v];
            tree_edges.push_back(i);
            total += edges[i].w;
        }
        for (size_t i = 0; i < edges.size(); ++i) {
            if (state[i] != FREE) continue;
            int u = edges[i].u, v = edges[i].v;
            if (forced_degree[u] == d || forced_degree[v] == d) continue;
            int a = find(parent, u), b = find(parent, v);
            if (a == b) continue;
            parent[a] = b;
            ++tree_degree[u];
            ++tree_degree[v];
            tree_edges.push_back(i);
            total += edges[i].w;
        }

        if ((int)tree_edges.size() != n - 1) return;
        if (best != -1 && total >= best) return;

        int bad = -1;
        for (int v = 0; v < n; ++v) {
            if (tree_degree[v] > d) {
                bad = v;
                break;
            }
        }
        if (bad == -1) {
            best = total;
            return;
        }

        int chosen = -1;
        for (int i : tree_edges) {
            if (state[i] != FREE) continue;
            if (edges[i].u != bad && edges[i].v != bad) continue;
            if (chosen == -1 || edges[i].w > edges[chosen].w) chosen = i;
        }

        state[chosen] = BANNED;
        dfs();

        state[chosen] = FORCED;
        ++forced_degree[edges[chosen].u];
        ++forced_degree[edges[chosen].v];
        dfs();
        --forced_degree[edges[chosen].u];
        --forced_degree[edges[chosen].v];

        state[chosen] = FREE;
    }

    const std::vector<Edge>& edges;
    int n, d;
    std::vector<int> state;
    std::vector<int> forced_degree;
    int best;
};

int get_min_weight(Graph graph) {
    Solver solver(graph);
    return solver.solve();
}
