#include "solution.hpp"

int main() {
    Graph graph;
    int result = get_min_weight(graph);
    if (result == -1) {
        std::cout << "Невозможно построить остовное дерево с заданным ограничением степени" << std::endl;
    }
    else {
        std::cout << result << std::endl;
    }
}
