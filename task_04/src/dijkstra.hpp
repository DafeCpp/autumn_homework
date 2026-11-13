#pragma once

#include "graph.hpp"

std::vector<long long> dijkstra_simple(
    const std::vector<std::vector<std::pair<int, long long>>>& graph,
    int start);
std::vector<long long> dijkstra(
    const std::vector<std::vector<std::pair<int, long long>>>& graph,
    int start);
