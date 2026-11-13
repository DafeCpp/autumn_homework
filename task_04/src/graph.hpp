#pragma once

#include <limits>
#include <utility>
#include <vector>

const long long INF = 1000000000000000000LL;
using Graph = std::vector<std::vector<std::pair<int, long long>>>;
using DistanceMatrix = std::vector<std::vector<long long>>;

struct Edge {
  int a, b;
  long long weight;
};
