#include "solution.hpp"

std::string get_critical(Graph graph) {
    std::vector<int> cut_vertices = graph.get_cut_vertices();
    std::vector<std::pair<int, int> > bridges = graph.get_bridges();

    std::string result = std::to_string(cut_vertices.size()) + "\n";
    if (cut_vertices.empty()) {
        result += "-";
    }
    for (size_t i = 0; i < cut_vertices.size(); ++i) {
        if (i) result += " ";
        result += std::to_string(cut_vertices[i] + 1);
    }

    result += "\n" + std::to_string(bridges.size()) + "\n";
    if (bridges.empty()) {
        result += "-";
    }
    for (size_t i = 0; i < bridges.size(); ++i) {
        if (i) result += "; ";
        result += std::to_string(bridges[i].first + 1) + " " + std::to_string(bridges[i].second + 1);
    }
    return result;
}
