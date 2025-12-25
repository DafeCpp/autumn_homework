#pragma once
#include <cmath>
#include <vector>
#include <stdexcept>

class DSU {
 public:
  DSU(int size) : parent(size), rank(size, 0) {
    for (int i = 0; i < size; ++i) {
      parent[i] = i;
    }
  }

  int Find(int v);

  bool Unite(int a, int b);

 private:
  std::vector<int> parent;
  std::vector<int> rank;
};

class Graph {
 public:
  struct Edge {
    int to;
    int weight = 0;
  };
  Graph(std::vector<std ::vector<Edge>> adjList, bool weighted) {
    if (weighted) {
      weightedDescription = adjList;
    } else {
      throw std::runtime_error("Use other constructor for unweighted graph");
    }
    visited.resize(adjList.size());
  }

  Graph(std::vector<std ::vector<int>> adjList) : Description{adjList} {
    visited.resize(adjList.size());

    int vertexCount = adjList.size();
    transpose.resize(vertexCount);

    for (int u = 0; u < vertexCount; ++u) {
      for (int v : adjList[u]) {
        transpose[v].push_back(u);
      }
    }
  }

  std::vector<std::pair<int, int>> FindBridges();  // task 1

  int MinEdgesToMakeStronglyConnected();  // task 2

  std ::vector<int> TopologySort(int vartex);  // task 3

  std::vector<std::vector<long long>> Johnson();  // task 4

  long long DegreeConstrainedMST(int maxDegree);  // task 5

  int MaxFlow(int source, int sink);  // task 6

 protected:
  std::vector<std::vector<int>> Description;

 private:
  enum class Color { white = 0, grey = 1, black = 2 };
  enum class DfsMode { Topology, Bridges, Order, Component };

  std::vector<int> timeIn;
  std::vector<int> lowestTime;
  int currentTimer;

  std::vector<std::pair<int, int>> bridges;

  void dfs(int currentVertex, int parentVertex, DfsMode mode);
  std::vector<std::vector<Edge>> weightedDescription;

  std::vector<std::vector<int>> transpose;
  std ::vector<Color> visited;
  std ::vector<int> path;

  std::vector<int> order;
  std::vector<int> component;
  int currentComponent;

  std::vector<long long> BellmanFord(int source);
  void Dijkstra(int source, std::vector<std::vector<Edge>>& g,
                std::vector<long long>& dist);
};

class RMQ {
 public:
  RMQ(std::vector<int> array);

  int Query(int left, int right);

 private:
  std::vector<std::vector<int>> table;
  std::vector<int> log;
};

class LCA : public Graph {
 public:
  LCA(const std::vector<std::vector<int>> tree, int root = 0);

  int Query(int u, int v);

 private:
  int vertexCount;
  int logN;

  std::vector<std::vector<int>> up;
  std::vector<int> depth;

  void BuildLCA(int currentVertex, int parentVertex);
};