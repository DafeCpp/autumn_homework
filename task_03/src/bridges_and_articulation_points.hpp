#pragma once
#include <vector>

#include "graph_trace.hpp"

struct Edge {
  int to;
  int id;
};

using GraphTrace = graph_trace::Recorder;

using Graph = std::vector<std::vector<Edge>>;

void dfs(int v, int parentEdge, const Graph& graph, std::vector<int>& tin,
         std::vector<int>& low, std::vector<bool>& isCut,
         std::vector<std::pair<int, int>>& bridges, int& timer,
         GraphTrace& trace);
