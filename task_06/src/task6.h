#pragma once
#include <vector>

using namespace std;

const int INF = 1e9;
struct Edge {
    int to;
    int cap;
    int flow;
    int rev; 
};

long long task6(int n, int m, const vector<vector<int>>& edges_input, int source, int sink);