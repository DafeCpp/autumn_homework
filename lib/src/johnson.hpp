#ifndef JOHNSON_HPP_
#define JOHNSON_HPP_

#include <vector>

constexpr long long kInf = 1'000'000'000'000'000'000LL;

struct Edge {
  int to;
  long long weight;
};

struct Graph {
  int n;
  std::vector<std::vector<Edge>> adj;

  explicit Graph(int n) : n(n), adj(n + 1) {}
  void AddEdge(int u, int v, long long w);
};

std::vector<long long> BellmanFord(const Graph& g);
std::vector<long long> Dijkstra(const Graph& g, int start,
                                const std::vector<long long>& h);
std::vector<std::vector<long long>> Johnson(const Graph& g,
                                            bool& has_negative_cycle);

#endif  // JOHNSON_HPP_