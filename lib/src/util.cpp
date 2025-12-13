#include "util.hpp"

#include <queue>
#include <stdexcept>

std ::vector<int> Graph::TopologySort(int vartex) {
  dfs(vartex);
  return path;
}

void Graph::dfs(int vartex) {
  path.push_back(vartex);
  visited[vartex] = Color::black;
  for (auto v : Description[vartex]) {
    if (visited[v] == Color::white) dfs(v);
  }
}

std::vector<long long> Graph::BellmanFord(int source) {
  int allVertex = weightedDescription.size();
  long long INF = 9000000000000000000;

  std::vector<long long> distination(allVertex, INF);
  distination[source] = 0;

  for (int i = 0; i < allVertex - 1; ++i) {
    bool changed = false;
    for (int u = 0; u < allVertex; ++u) {
      if (distination[u] == INF) continue;
      for (int k = 0; k < weightedDescription[u].size(); k++) {
        Edge current_edge = weightedDescription[u][k];
        if (distination[u] + current_edge.weight <
            distination[current_edge.to]) {
          distination[current_edge.to] = distination[u] + current_edge.weight;
          changed = true;
        }
      }
    }
    if (!changed) break;
  }

  for (int u = 0; u < allVertex; ++u) {
    if (distination[u] == INF) continue;
    for (int k = 0; k < weightedDescription[u].size(); ++k) {
      Edge current_edge = weightedDescription[u][k];
      if (distination[u] + current_edge.weight < distination[current_edge.to]) {
        throw std::runtime_error("Bellman-Ford: negative cycle");
      }
    }
  }

  return distination;
}

void Graph::Dijkstra(int source, std::vector<std::vector<Edge> >& g,
                     std::vector<long long>& distination) {
  int allVertex = g.size();
  long long INF = 9000000000000000000;

  distination.clear();
  distination.resize(allVertex, INF);
  distination[source] = 0;

  std::priority_queue<std::pair<long long, int>,
                      std::vector<std::pair<long long, int> >,
                      std::greater<std::pair<long long, int> > >
      queue;

  queue.push(std::make_pair(0, source));

  while (!queue.empty()) {
    std::pair<long long, int> p = queue.top();
    queue.pop();
    long long d = p.first;
    int u = p.second;

    if (d != distination[u]) continue;

    for (int k = 0; k < g[u].size(); k++) {
      Edge current_edge = g[u][k];
      if (distination[u] + current_edge.weight < distination[current_edge.to]) {
        distination[current_edge.to] = distination[u] + current_edge.weight;
        queue.push(
            std::make_pair(distination[current_edge.to], current_edge.to));
      }
    }
  }
}

std::vector<std::vector<long long> > Graph::Johnson() {
  int allVertex = weightedDescription.size();
  long long INF = 9000000000000000000;

  std::vector<std::vector<Edge> > weightedDescriptionCopy = weightedDescription;
  weightedDescriptionCopy.resize(allVertex + 1);
  for (int v = 0; v < allVertex; ++v) {
    Edge current_edge;
    current_edge.to = v;
    current_edge.weight = 0;
    weightedDescriptionCopy[allVertex].push_back(current_edge);
  }

  std::vector<std::vector<Edge> > weightedDescriptionOld = weightedDescription;
  weightedDescription = weightedDescriptionCopy;

  std::vector<long long> h = BellmanFord(allVertex);

  weightedDescription = weightedDescriptionOld;

  std::vector<std::vector<Edge> > reweightedDescription = weightedDescription;
  for (int u = 0; u < allVertex; ++u) {
    for (int k = 0; k < reweightedDescription[u].size(); ++k) {
      reweightedDescription[u][k].weight =
          reweightedDescription[u][k].weight + h[u] - h[reweightedDescription[u][k].to];
    }
  }

  std::vector<std::vector<long long> > distination(
      allVertex, std::vector<long long>(allVertex, INF));

  for (int u = 0; u < allVertex; ++u) {
    Dijkstra(u, reweightedDescription, distination[u]);
  }

  for (int u = 0; u < allVertex; ++u) {
    for (int v = 0; v < allVertex; ++v) {
      if (distination[u][v] < INF)
        distination[u][v] = distination[u][v] - h[u] + h[v];
    }
  }

  return distination;
}
