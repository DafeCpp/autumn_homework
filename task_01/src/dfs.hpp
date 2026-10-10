#pragma once

#include <graph_trace.hpp>
#include <vector>

using GraphTrace = graph_trace::Recorder;

enum class Color { kWhite = -1, kGray = 0, kBlack = 1 };

void dfs(int vertex, std::vector<Color>& color,
         const std::vector<std::vector<int>>& graph, std::vector<int>& parent,
         std::vector<int>& sorted_components, GraphTrace& trace);