#pragma once
#include <cmath>
#include <functional>
#include <limits>
#include <queue>
#include <vector>

class CoordinatedGraph {
 public:
  struct Edge {
    int to;
    float weight;
  };

  CoordinatedGraph(std::vector<std::vector<Edge>> adjList,
                   std::vector<std::pair<float, float>> coords)
      : Description(adjList), Coordinates(coords) {}

  std::vector<Edge> Neighbors(int vertex) { return Description[vertex]; }

  std::pair<float, float> GetCoordinates(int vertex) {
    return Coordinates[vertex];
  }

  int len() { return Description.size(); }

 private:
  std::vector<std::vector<Edge>> Description;
  std::vector<std::pair<float, float>> Coordinates;
};

class Astar {
 public:
  Astar(CoordinatedGraph graph, int begin_point, int end_point)
      : graph_(graph), from_(begin_point), to_(end_point) {
    int n = graph_.len();
    gscore.assign(n, std::numeric_limits<float>::infinity());
    fscore.assign(n, std::numeric_limits<float>::infinity());
    came_from.assign(n, -1);
    closed.assign(n, false);

    gscore[from_] = 0;
    fscore[from_] = EvristicFunction(from_);
    Queue.push({fscore[from_], from_});
  }

  std::vector<int> FindPath();

 private:
  CoordinatedGraph graph_;
  int from_, to_;

  float EvristicFunction(int vertex);

  std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>,
                      std::greater<std::pair<float, int>>>
      Queue;

  std::vector<float> gscore;
  std::vector<float> fscore;
  std::vector<int> came_from;
  std::vector<bool> closed;
};
