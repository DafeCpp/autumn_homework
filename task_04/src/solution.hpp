#pragma once

#include "graph.hpp"

using Matrix = std::vector<std::vector<long long>>;
const long long INF = 4e18;

Matrix johnson(const Graph& graph);
