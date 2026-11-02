#pragma once

#include <optional>

#include "graph.hpp"

const long long INF = 4e18;

std::optional<std::vector<std::vector<long long> > > johnson(Graph graph);
