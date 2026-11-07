#include "solution.hpp"

int main() {
    Graph graph;
    std::optional<std::vector<std::vector<long long> > > result = johnson(graph);
    if (!result) {
        std::cout << "Negative cycle" << std::endl;
        return 0;
    }
    for (const std::vector<long long>& row : *result) {
        for (size_t i = 0; i < row.size(); ++i) {
            if (i) std::cout << " ";
            if (row[i] == INF) {
                std::cout << "INF";
            }
            else {
                std::cout << row[i];
            }
        }
        std::cout << std::endl;
    }
}
