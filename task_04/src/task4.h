#pragma once
#include <string>
#include <vector>

struct Edge {
    int u;
    int v;
    long long w;
};

std::vector<std::string> task4(int n, int m, std::vector<Edge>& arr);
