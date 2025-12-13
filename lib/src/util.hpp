#include <vector>

class Graph {
 public:
  struct Edge {
    int to;
    int weight = 0;
  };
  Graph(std::vector<std ::vector<Edge>> adjList, bool weighted) {
    if (weighted) {
      weightedDescription = adjList;
    } else {
      throw ("Use other constructor for unweighted graph");
    }
    visited.resize(adjList.size());
  }

  Graph(std::vector<std ::vector<int>> adjList): Description{adjList} {
    visited.resize(adjList.size());
  }

  std ::vector<int> TopologySort(int vartex);

  std::vector<std::vector<long long>> Johnson();

 private:
  std::vector<std::vector<Edge>> weightedDescription;
  std::vector<std::vector<int>> Description;
  enum class Color { white = 0, grey = 1, black = 2 };
  std ::vector<Color> visited;
  std ::vector<int> path;
  void dfs(int vartex);
  std::vector<long long> BellmanFord(int source);
  void Dijkstra(int source, std::vector<std::vector<Edge>>& g,
                std::vector<long long>& dist);
};