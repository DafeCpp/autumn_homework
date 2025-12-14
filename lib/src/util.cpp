#include "util.hpp"

#include <algorithm>
#include <queue>
#include <stdexcept>

std::vector<int> Graph::TopologySort(int startVertex) {
  visited.clear();
  visited.resize(Description.size(), Color::white);
  path.clear();

  dfs(startVertex, -1, DfsMode::Topology);
  std::reverse(path.begin(), path.end());
  return path;
}

std::vector<std::pair<int, int> > Graph::FindBridges() {
  int allVertex = Description.size();

  visited.clear();
  visited.resize(allVertex, Color::white);

  timeIn.clear();
  timeIn.resize(allVertex, -1);

  lowestTime.clear();
  lowestTime.resize(allVertex, -1);

  bridges.clear();
  currentTimer = 0;

  dfs(0, -1, DfsMode::Bridges);

  return bridges;
}

void Graph::dfs(int currentVertex, int parentVertex, DfsMode mode) {
  visited[currentVertex] = Color::grey;

  switch (mode) {
    case DfsMode::Order: {
      for (int next : Description[currentVertex]) {
        if (visited[next] == Color::white) {
          dfs(next, -1, mode);
        }
      }
      order.push_back(currentVertex);
      break;
    }

    case DfsMode::Component: {
      component[currentVertex] = currentComponent;
      for (int next : transpose[currentVertex]) {
        if (visited[next] == Color::white) {
          dfs(next, -1, mode);
        }
      }
      break;
    }

    case DfsMode::Bridges: {
      timeIn[currentVertex] = currentTimer;
      lowestTime[currentVertex] = currentTimer;
      ++currentTimer;

      for (int i = 0; i < Description[currentVertex].size(); ++i) {
        int nextVertex = Description[currentVertex][i];
        if (nextVertex == parentVertex) continue;

        if (visited[nextVertex] == Color::white) {
          dfs(nextVertex, currentVertex, mode);

          if (lowestTime[nextVertex] < lowestTime[currentVertex]) {
            lowestTime[currentVertex] = lowestTime[nextVertex];
          }

          if (lowestTime[nextVertex] > timeIn[currentVertex]) {
            bridges.push_back(std::make_pair(currentVertex, nextVertex));
          }
        } else {
          if (timeIn[nextVertex] < lowestTime[currentVertex]) {
            lowestTime[currentVertex] = timeIn[nextVertex];
          }
        }
      }
      break;
    }

    case DfsMode::Topology: {
      for (int nextVertex : Description[currentVertex]) {
        if (visited[nextVertex] == Color::white) {
          dfs(nextVertex, currentVertex, mode);
        }
      }
      path.push_back(currentVertex);
      break;
    }
  }

  visited[currentVertex] = Color::black;
}

int Graph::MinEdgesToMakeStronglyConnected() {
  int vertexCount = Description.size();

  visited.assign(vertexCount, Color::white);
  order.clear();

  for (int v = 0; v < vertexCount; ++v) {
    if (visited[v] == Color::white) {
      dfs(v, -1, DfsMode::Order);
    }
  }

  visited.assign(vertexCount, Color::white);
  component.assign(vertexCount, -1);
  currentComponent = 0;

  for (int i = vertexCount - 1; i >= 0; --i) {
    int v = order[i];
    if (visited[v] == Color::white) {
      dfs(v, -1, DfsMode::Component);
      ++currentComponent;
    }
  }

  if (currentComponent == 1) {
    return 0;
  }

  std::vector<int> inDegree(currentComponent, 0);
  std::vector<int> outDegree(currentComponent, 0);

  for (int u = 0; u < vertexCount; ++u) {
    for (int v : Description[u]) {
      if (component[u] != component[v]) {
        ++outDegree[component[u]];
        ++inDegree[component[v]];
      }
    }
  }

  int sources = 0;
  int sinks = 0;

  for (int i = 0; i < currentComponent; ++i) {
    if (inDegree[i] == 0) {
      ++sources;
    }
    if (outDegree[i] == 0) {
      ++sinks;
    }
  }

  return std::max(sources, sinks);
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
      reweightedDescription[u][k].weight = reweightedDescription[u][k].weight +
                                           h[u] -
                                           h[reweightedDescription[u][k].to];
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
