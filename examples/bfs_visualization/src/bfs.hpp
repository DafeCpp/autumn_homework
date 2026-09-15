#pragma once

#include <memory>
#include <vector>

namespace graph_trace {
class Recorder;
}

namespace bfs_example {
struct Edge {
  int from;
  int to;
};

// Distances from vertex 1 in an undirected graph. Vertices are numbered 1..n;
// result[0] is unused and unreachable vertices have distance -1.
// Throws std::invalid_argument for n <= 0 or endpoints outside 1..n.
// Recording is optional; the caller finishes the recording after the call.
std::vector<int> Bfs(int n, const std::vector<Edge>& edges,
                     const std::shared_ptr<graph_trace::Recorder>& trace = {});
}  // namespace bfs_example
