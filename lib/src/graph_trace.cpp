#include "graph_trace.hpp"

#include <cstdlib>

namespace graph_trace {
namespace {
// Заключает текст в кавычки JSON и экранирует кавычки, обратные слеши
// и управляющие символы, чтобы подпись шага не нарушала формат записи.
std::string Quote(std::string_view text) {
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
}  // namespace

Recorder::Recorder() {
  const char* path = std::getenv("GRAPH_TRACE");
  if (path && *path) stream_.open(path);
  Write("{\"type\":\"header\",\"version\":1}");
}
bool Recorder::IsEnabled() const {
  return stream_.is_open() && stream_.good() && !truncated_;
}
void Recorder::Graph(int count, bool directed, int first) {
  if (!IsEnabled()) return;
  Write("{\"type\":\"graph\",\"n\":" + std::to_string(count) +
        ",\"first\":" + std::to_string(first) +
        ",\"directed\":" + (directed ? "true}" : "false}"));
}
void Recorder::AddEdge(int id, int from, int to) {
  if (!IsEnabled()) return;
  Write("{\"type\":\"add_edge\",\"id\":" + std::to_string(id) + ",\"from\":" +
        std::to_string(from) + ",\"to\":" + std::to_string(to) + "}");
}
void Recorder::Node(int id, std::string_view state) {
  State("node", id, state);
}
void Recorder::Edge(int id, std::string_view state) {
  State("edge", id, state);
}
void Recorder::Value(int id, std::string_view name, int value) {
  if (!IsEnabled()) return;
  Write("{\"type\":\"value\",\"id\":" + std::to_string(id) + ",\"name\":" +
        Quote(name) + ",\"value\":" + std::to_string(value) + "}");
}
void Recorder::Step(std::string_view message) {
  if (!IsEnabled()) return;
  Write("{\"type\":\"step\",\"message\":" + Quote(message) + "}");
}
void Recorder::Finish() { Write("{\"type\":\"end\"}"); }

void Recorder::State(std::string_view type, int id, std::string_view state) {
  if (!IsEnabled()) return;
  Write("{\"type\":" + Quote(type) + ",\"id\":" + std::to_string(id) +
        ",\"state\":" + Quote(state) + "}");
}
void Recorder::Write(const std::string& line) {
  if (!IsEnabled()) return;
  // Ограничиваем объём записи, в том числе при случайном бесконечном цикле.
  if (++events_ > 50000 ||
      bytes_ + line.size() > std::size_t{8} * 1024 * 1024) {
    stream_ << "{\"type\":\"truncated\"}\n" << std::flush;
    truncated_ = true;
    return;
  }
  stream_ << line << '\n' << std::flush;
  bytes_ += line.size() + 1;
}
}  // namespace graph_trace
