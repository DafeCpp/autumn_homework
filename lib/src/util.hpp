#include <vector>

class Graph {
public:
    Graph(std::vector<std :: vector<int>> adjList) : Description(adjList) {
        // for (int i =0; i<adjList.size(); ++i) {
        //     for (int j =0; j<adjList[i].size(); ++j) {
        //         Description[i].push_back({adjList[i][j], 0});
        //     }
        // }
        visited.resize(adjList.size());
    }


    std :: vector<int> TopologySort(int vartex);

    std::vector<int> Neighbors(int vertex) {
        return Description[vertex];
    }

private:
    struct Edge {
        int to;
        int weight = 0;
    };
    std::vector<std::vector <int>> Description;
    enum class Color {
        white = 0,
        grey = 1,
        black = 2
    };
    std :: vector <Color> visited;
    std :: vector <int> path;
    void dfs(int vartex);
};