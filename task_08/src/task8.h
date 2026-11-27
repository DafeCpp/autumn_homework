#ifndef TASK8_H
#define TASK8_H

#include <utility>
#include <vector>

class LCA {
public:
    LCA(int n, const std::vector<std::pair<int, int>>& edges, int root = 1);
    int get_lca(int u, int v) const;

private:
    std::vector<int> depth;
    std::vector<int> first;
    std::vector<int> euler;
    std::vector<int> log2_table;
    std::vector<std::vector<int>> st;
};

#endif