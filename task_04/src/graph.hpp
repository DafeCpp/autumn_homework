#pragma once

#include <cmath>
#include <optional>
#include <vector>

struct Edge {
  int from;
  int to;
  double weight;
};

struct Graph {
  Graph() {
    // d.assign(graph.size(), std::vector<double>(INFINITY));
    // distances_ford.assign(graph.size(), INFINITY);
  }

 private:
  // std::vector<std::vector<double>> d;
  std::vector<Edge> edges;
  std::vector<std::vector<std::pair<int, double>>> graph;
  // std::vector<double> distances_ford;
  std::optional<std::vector<double>> BellmanFord();
  std::vector<double> Dijkstra(int start);
  void Reweigh(const std::vector<double>& distances);
  bool Johnson();
  std::vector<std::vector<std::pair<int, double>>> reweighted_graph;
};