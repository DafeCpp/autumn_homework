#include "solution.hpp"

std::vector<int> get_requirements(Graph graph) {
    return graph.get_top_sort();
}