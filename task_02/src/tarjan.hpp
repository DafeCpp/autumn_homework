#pragma once

#include <vector>

#include "graph_trace.hpp"

using GraphTrace = graph_trace::Recorder;

void tarjan(const std::vector<std::vector<int>>& graph,
            std::vector<int>& component, GraphTrace& trace, int& timer,
            int& component_counter);