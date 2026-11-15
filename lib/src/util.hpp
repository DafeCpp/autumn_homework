#pragma once
#include <vector>
#include <utility>

std::vector<int> topological_sort(int n,
                                  const std::vector<std::pair<int,int>>& edges);