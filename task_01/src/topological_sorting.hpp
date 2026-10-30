#pragma once

#include <graph_trace.hpp>
#include <vector>

using GraphTrace = graph_trace::Recorder;

enum class Color { WHITE = -1, GRAY = 0, BLACK = 1 };

void dfs(int vertex, std::vector<Color>& color,
         const std::vector<std::vector<int>>& graph, std::vector<int>& order,
         GraphTrace& trace);