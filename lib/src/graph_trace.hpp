#pragma once

#include <cstddef>
#include <fstream>
#include <string>
#include <string_view>

namespace graph_trace {
// Set GRAPH_TRACE to record steps to a JSONL file without changing stdout.
class Recorder {
 public:
  Recorder();

  bool IsEnabled() const;

  void Graph(int count, bool directed = false, int first = 1);
  // IDs are zero-based positions of edges in the input, including parallel
  // edges.
  void AddEdge(int id, int from, int to);

  void Node(int id, std::string_view state);
  void Edge(int id, std::string_view state);
  void Value(int id, std::string_view name, int value);
  void Step(std::string_view message);
  void Finish();

 private:
  void State(std::string_view type, int id, std::string_view state);
  void Write(const std::string& line);

  std::ofstream stream_;
  unsigned events_ = 0;
  std::size_t bytes_ = 0;
  bool truncated_ = false;
};
}  // namespace graph_trace
