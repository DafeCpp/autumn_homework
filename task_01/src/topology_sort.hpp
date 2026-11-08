#pragma once

#include <optional>
#include <vector>

std::optional<std::vector<int>> TopologySort(
    const std::vector<std::vector<int>>& graf);