#include "solution.hpp"

#include <algorithm>

int get_min_edges(Graph graph) {
    std::vector<int> comp;
    int cnt_comp = graph.get_scc(comp);
    if (cnt_comp == 1) {
        return 0;
    }
    std::vector<bool> has_in(cnt_comp, false), has_out(cnt_comp, false);
    const std::vector<std::vector<int> >& adj = graph.get_adjacency();
    for (int v = 0; v < graph.size(); ++v) {
        for (int u : adj[v]) {
            if (comp[v] != comp[u]) {
                has_out[comp[v]] = true;
                has_in[comp[u]] = true;
            }
        }
    }
    int sources = 0, sinks = 0;
    for (int c = 0; c < cnt_comp; ++c) {
        if (!has_in[c]) ++sources;
        if (!has_out[c]) ++sinks;
    }
    return std::max(sources, sinks);
}
