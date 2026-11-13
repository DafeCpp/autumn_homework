#pragma once

#include "graph.hpp"

bool bellman(int vertexCount, const std::vector<Edge>& edges, int start,
             std::vector<long long>& distance);
