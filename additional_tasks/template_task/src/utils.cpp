#include "utils.hpp"

#include <cmath>
#include <stdexcept>

float Astar::EvristicFunction(int vertex) {
  float x1 = graph_.GetCoordinates(vertex).first;
  float y1 = graph_.GetCoordinates(vertex).second;
  float x2 = graph_.GetCoordinates(to_).first;
  float y2 = graph_.GetCoordinates(to_).second;
  return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

std::vector<int> Astar::FindPath() {
  while (!Queue.empty()) {
    auto [f_current, v] = Queue.top();
    Queue.pop();

    if (closed[v]) continue;
    closed[v] = true;

    if (v == to_) break;

    for (auto edge : graph_.Neighbors(v)) {
      int u = edge.to;
      if (closed[u]) continue;

      float tentative_g = gscore[v] + edge.weight;

      if (tentative_g < gscore[u]) {
        came_from[u] = v;
        gscore[u] = tentative_g;
        fscore[u] = tentative_g + EvristicFunction(u);
        Queue.push({fscore[u], u});
      }
    }
  }

  if (came_from[to_] == -1) {
    throw std::runtime_error("Path not found");
  }

  std::vector<int> path;
  for (int v = to_; v != -1; v = came_from[v]) path.push_back(v);

  std::reverse(path.begin(), path.end());

  return path;
}