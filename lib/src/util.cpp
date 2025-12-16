#include "util.hpp"

#include <algorithm>
#include <queue>
#include <stdexcept>

int RMQ::Query(int left, int right) {
  int length = right - left + 1;
  int k = log[length];

  int leftMin = table[left][k];
  int rightMin = table[right - (1 << k) + 1][k];

  return std::min(leftMin, rightMin);
}

RMQ::RMQ(std::vector<int> array) {
  int n = array.size();

  log.resize(n + 1);
  log[1] = 0;
  for (int i = 2; i <= n; ++i) {
    log[i] = log[i / 2] + 1;
  }

  int maxLog = log[n];
  table.resize(n, std::vector<int>(maxLog + 1));

  for (int i = 0; i < n; ++i) {
    table[i][0] = array[i];
  }

  for (int k = 1; k <= maxLog; ++k) {
    for (int i = 0; i + (1 << k) <= n; ++i) {
      table[i][k] = std::min(table[i][k - 1], table[i + (1 << (k - 1))][k - 1]);
    }
  }
}

std::vector<int> Graph::TopologySort(int startVertex) {
  visited.clear();
  visited.resize(Description.size(), Color::white);
  path.clear();

  dfs(startVertex, -1, DfsMode::Topology);
  std::reverse(path.begin(), path.end());
  return path;
}

std::vector<std::pair<int, int>> Graph::FindBridges() {
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

void Graph::Dijkstra(int source, std::vector<std::vector<Edge>>& g,
                     std::vector<long long>& distination) {
  int allVertex = g.size();
  long long INF = 9000000000000000000;

  distination.clear();
  distination.resize(allVertex, INF);
  distination[source] = 0;

  std::priority_queue<std::pair<long long, int>,
                      std::vector<std::pair<long long, int>>,
                      std::greater<std::pair<long long, int>>>
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

std::vector<std::vector<long long>> Graph::Johnson() {
  int allVertex = weightedDescription.size();
  long long INF = 9000000000000000000;

  std::vector<std::vector<Edge>> weightedDescriptionCopy = weightedDescription;
  weightedDescriptionCopy.resize(allVertex + 1);
  for (int v = 0; v < allVertex; ++v) {
    Edge current_edge;
    current_edge.to = v;
    current_edge.weight = 0;
    weightedDescriptionCopy[allVertex].push_back(current_edge);
  }

  std::vector<std::vector<Edge>> weightedDescriptionOld = weightedDescription;
  weightedDescription = weightedDescriptionCopy;

  std::vector<long long> h = BellmanFord(allVertex);

  weightedDescription = weightedDescriptionOld;

  std::vector<std::vector<Edge>> reweightedDescription = weightedDescription;
  for (int u = 0; u < allVertex; ++u) {
    for (int k = 0; k < reweightedDescription[u].size(); ++k) {
      reweightedDescription[u][k].weight = reweightedDescription[u][k].weight +
                                           h[u] -
                                           h[reweightedDescription[u][k].to];
    }
  }

  std::vector<std::vector<long long>> distination(
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

int Graph::MaxFlow(int source, int sink) {
  int vertexCount = weightedDescription.size();

  std::vector<std::vector<int>> capacity(vertexCount,
                                         std::vector<int>(vertexCount, 0));

  for (int u = 0; u < vertexCount; ++u) {
    for (int k = 0; k < weightedDescription[u].size(); ++k) {
      Edge currentEdge = weightedDescription[u][k];
      capacity[u][currentEdge.to] += currentEdge.weight;
    }
  }

  int maxFlow = 0;

  while (true) {
    std::vector<int> parent(vertexCount, -1);
    std::queue<int> bfsQueue;

    bfsQueue.push(source);
    parent[source] = source;

    while (!bfsQueue.empty() && parent[sink] == -1) {
      int currentVertex = bfsQueue.front();
      bfsQueue.pop();

      for (int nextVertex = 0; nextVertex < vertexCount; ++nextVertex) {
        if (parent[nextVertex] == -1 &&
            capacity[currentVertex][nextVertex] > 0) {
          parent[nextVertex] = currentVertex;
          bfsQueue.push(nextVertex);
        }
      }
    }

    if (parent[sink] == -1) break;

    int pathFlow = 1000000000;
    int v = sink;

    while (v != source) {
      int u = parent[v];
      if (capacity[u][v] < pathFlow) {
        pathFlow = capacity[u][v];
      }
      v = u;
    }

    v = sink;
    while (v != source) {
      int u = parent[v];
      capacity[u][v] -= pathFlow;
      capacity[v][u] += pathFlow;
      v = u;
    }

    maxFlow += pathFlow;
  }

  return maxFlow;
}

long long Graph::DegreeConstrainedMST(int maxDegree) {
  int vertexCount = weightedDescription.size();

  struct UndirectedEdge {
    int from;
    int to;
    int weight;
  };

  std::vector<UndirectedEdge> edges;

  for (int u = 0; u < vertexCount; ++u) {
    for (int i = 0; i < weightedDescription[u].size(); ++i) {
      Edge currentEdge = weightedDescription[u][i];
      if (u < currentEdge.to) {
        UndirectedEdge e;
        e.from = u;
        e.to = currentEdge.to;
        e.weight = currentEdge.weight;
        edges.push_back(e);
      }
    }
  }

  std::sort(edges.begin(), edges.end(),
            [](const UndirectedEdge& a, const UndirectedEdge& b) {
              return a.weight < b.weight;
            });

  DSU dsu(vertexCount);
  std::vector<int> degree(vertexCount, 0);

  long long totalWeight = 0;
  int usedEdges = 0;

  for (int i = 0; i < edges.size(); ++i) {
    int u = edges[i].from;
    int v = edges[i].to;

    if (dsu.Find(u) == dsu.Find(v)) {
      continue;
    }

    if (degree[u] >= maxDegree || degree[v] >= maxDegree) {
      continue;
    }

    dsu.Unite(u, v);
    ++degree[u];
    ++degree[v];
    totalWeight += edges[i].weight;
    ++usedEdges;

    if (usedEdges == vertexCount - 1) {
      break;
    }
  }

  if (usedEdges != vertexCount - 1) {
    throw std::runtime_error(
        "Cannot construct degree-constrained MST with given maxDegree");
  }

  return totalWeight;
}

LCA::LCA(const std::vector<std::vector<int>> tree, int root)
    : Graph(tree),
      vertexCount(tree.size()),
      logN(std::log2(tree.size()) + 1),
      up(vertexCount, std::vector<int>(logN, -1)),
      depth(vertexCount, 0) {
  BuildLCA(root, root);
}

void LCA::BuildLCA(int currentVertex, int parentVertex) {
  up[currentVertex][0] = parentVertex;

  for (int k = 1; k < logN; ++k) {
    up[currentVertex][k] = up[up[currentVertex][k - 1]][k - 1];
  }

  for (int nextVertex : Description[currentVertex]) {
    if (nextVertex == parentVertex) continue;

    depth[nextVertex] = depth[currentVertex] + 1;
    BuildLCA(nextVertex, currentVertex);
  }
}

int LCA::Query(int u, int v) {
  if (depth[u] < depth[v]) std::swap(u, v);

  for (int k = logN - 1; k >= 0; --k) {
    if (depth[u] - (1 << k) >= depth[v]) {
      u = up[u][k];
    }
  }

  if (u == v) return u;

  for (int k = logN - 1; k >= 0; --k) {
    if (up[u][k] != up[v][k]) {
      u = up[u][k];
      v = up[v][k];
    }
  }

  return up[u][0];
}

bool DSU::Unite(int a, int b) {
  a = Find(a);
  b = Find(b);
  if (a == b) {
    return false;
  }

  if (rank[a] < rank[b]) {
    parent[a] = b;
  } else if (rank[a] > rank[b]) {
    parent[b] = a;
  } else {
    parent[b] = a;
    ++rank[a];
  }
  return true;
}

int DSU::Find(int v) {
  if (parent[v] == v) {
    return v;
  }
  parent[v] = Find(parent[v]);
  return parent[v];
}