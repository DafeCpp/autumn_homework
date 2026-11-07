#include "solution.hpp"

int main() {
    Graph graph;
    std::vector<int> res = get_requirements(graph);
    for (int v : res) {
        std::cout << v << " ";
    }
}