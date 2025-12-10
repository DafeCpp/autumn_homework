#include <iostream>
#include <util.hpp>

int main() { 
    std :: vector <std :: vector<int>> list_smezhnosti = {
        {1,3},
        {2,3},
        {6},
        {2,4},
        {},
        {0},
        {}
    };

    Graph g(list_smezhnosti);
    auto ans = g.TopologySort(5);
    for (auto m : ans)
        std :: cout << m << " ";
 }
