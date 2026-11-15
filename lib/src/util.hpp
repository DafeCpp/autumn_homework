#pragma once
#include <vector>
#include <utility>

std::vector<int> topological_sort(int n,
                                  const std::vector<std::pair<int,int>>& edges);
                                  
int min_edges_to_make_strongly_connected(int n, 
                                         const std::vector<std::pair<int,int>>& edges);