#ifndef JOHNSON_H_
#define JOHNSON_H_

#include <tuple>
#include <vector>

constexpr long long kInf = 1'000'000'000'000'000'000LL;

struct Edge {
  int to;
  long long weight;
};

struct Graph {
  int n;
  std::vector<std::vector<Edge>> adj;
};

Graph ReadGraph(int n, int m);
std::vector<long long> BellmanFord(const Graph& g);
std::vector<long long> Dijkstra(const Graph& g, int start,
                                const std::vector<long long>& h);
std::vector<std::vector<long long>> Johnson(const Graph& g,
                                            bool& has_negative_cycle);
void PrintResult(const std::vector<std::vector<long long>>& dist,
                 bool has_negative_cycle);

#endif  // JOHNSON_H_
