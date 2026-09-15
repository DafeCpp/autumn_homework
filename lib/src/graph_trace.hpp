#pragma once

#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <string>
#include <string_view>

// JSON Lines: each complete line survives even an interrupted program.
// Set GRAPH_TRACE to enable recording; stdout remains the solution's answer.
namespace graph_trace {
inline std::string Quote(std::string_view text) {
  std::string result = "\"";
  for (unsigned char ch : text) {
    if (ch == '"' || ch == '\\') {
      result += '\\';
      result += ch;
    } else if (ch < 32) {
      const char* digits = "0123456789abcdef";
      result += "\\u00";
      result += digits[ch >> 4];
      result += digits[ch & 15];
    } else {
      result += ch;
    }
  }
  return result + '"';
}

class Recorder {
 public:
  Recorder() {
    const char* path = std::getenv("GRAPH_TRACE");
    if (path && *path) stream_.open(path);
    Write("{\"type\":\"header\",\"version\":1}");
  }
  bool IsEnabled() const {
    return stream_.is_open() && stream_.good() && !truncated_;
  }
  void Graph(int count, bool directed = false, int first = 1) {
    if (!IsEnabled()) return;
    Write("{\"type\":\"graph\",\"n\":" + std::to_string(count) +
          ",\"first\":" + std::to_string(first) +
          ",\"directed\":" + (directed ? "true}" : "false}"));
  }
  // IDs are zero-based positions of edges in the input, also for parallel
  // edges.
  void AddEdge(int id, int from, int to) {
    if (!IsEnabled()) return;
    Write("{\"type\":\"add_edge\",\"id\":" + std::to_string(id) + ",\"from\":" +
          std::to_string(from) + ",\"to\":" + std::to_string(to) + "}");
  }
  void Node(int id, std::string_view state) { State("node", id, state); }
  void Edge(int id, std::string_view state) { State("edge", id, state); }
  void Value(int id, std::string_view name, int value) {
    if (!IsEnabled()) return;
    Write("{\"type\":\"value\",\"id\":" + std::to_string(id) + ",\"name\":" +
          Quote(name) + ",\"value\":" + std::to_string(value) + "}");
  }
  void Step(std::string_view message) {
    if (!IsEnabled()) return;
    Write("{\"type\":\"step\",\"message\":" + Quote(message) + "}");
  }
  void Finish() { Write("{\"type\":\"end\"}"); }

 private:
  void State(std::string_view type, int id, std::string_view state) {
    if (!IsEnabled()) return;
    Write("{\"type\":" + Quote(type) + ",\"id\":" + std::to_string(id) +
          ",\"state\":" + Quote(state) + "}");
  }
  void Write(const std::string& line) {
    if (!IsEnabled()) return;
    // Bound disk usage, including for accidental infinite loops.
    if (++events_ > 50000 ||
        bytes_ + line.size() > std::size_t{8} * 1024 * 1024) {
      stream_ << "{\"type\":\"truncated\"}\n" << std::flush;
      truncated_ = true;
      return;
    }
    stream_ << line << '\n' << std::flush;
    bytes_ += line.size() + 1;
  }
  std::ofstream stream_;
  unsigned events_ = 0;
  std::size_t bytes_ = 0;
  bool truncated_ = false;
};
}  // namespace graph_trace
