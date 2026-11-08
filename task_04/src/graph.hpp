#pragma once

#include <cmath>
#include <optional>
#include <vector>

struct Flow {
  int from;
  int to;
  double weight;
};

struct Graph {
  Graph(int n_, std::vector<Flow> graph)
      : n(n_),
        edges(std::move(graph)),
        potentials(n_, 0.0),
        reweighted_graph(n_) {
    distances = Johnson();
  }
  std::optional<std::vector<std::vector<double>>> distances;

 private:
  int n;
  std::vector<Flow> edges;
  std::vector<double> potentials;
  std::vector<std::vector<std::pair<int, double>>> reweighted_graph;

  std::optional<std::vector<double>> BellmanFord();
  std::vector<double> Dijkstra(int start);
  void Reweigh();
  void DistancesReconstruction(const int &start,
                               std::vector<double> &distances);
  std::optional<std::vector<std::vector<double>>> Johnson();
};