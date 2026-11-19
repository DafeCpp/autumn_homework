#include "mst.hpp"

#include <algorithm>
#include <queue>
#include <utility>

#include "dsu.hpp"

using namespace std;

// Этап 1. Строим минимальный остовный лес без specialVertex.
void buildForest(const vector<Edge>& edges, int specialVertex, DSU& components,
                 vector<bool>& inTree) {
  int edgeCount = edges.size();

  for (int edgeIndex = 0; edgeIndex < edgeCount; edgeIndex++) {
    Edge edge = edges[edgeIndex];

    bool isSpecialEdge = (edge.firstVertex == specialVertex) ||
                         (edge.secondVertex == specialVertex);

    if (isSpecialEdge) {
      continue;
    }

    if (components.unite(edge.firstVertex, edge.secondVertex)) {
      inTree[edgeIndex] = true;
    }
  }
}

// Этап 2. Считаем компоненты без specialVertex.
int countComponents(int vertexCount, int specialVertex, DSU& components) {
  int componentCount = 0;

  for (int vertex = 0; vertex < vertexCount; vertex++) {
    if (vertex == specialVertex) {
      continue;
    }

    if (components.findRoot(vertex) == vertex) {
      componentCount++;
    }
  }

  return componentCount;
}

// Этап 3. Находим самое дешёвое ребро из specialVertex в каждую компоненту.
vector<int> findCheapestEdges(int vertexCount, const vector<Edge>& edges,
                              int specialVertex, DSU& components) {
  int edgeCount = edges.size();
  vector<int> cheapestEdgeToComponent(vertexCount, -1);

  for (int edgeIndex = 0; edgeIndex < edgeCount; edgeIndex++) {
    Edge edge = edges[edgeIndex];

    int otherEnd;
    if (edge.firstVertex == specialVertex) {
      otherEnd = edge.secondVertex;
    } else if (edge.secondVertex == specialVertex) {
      otherEnd = edge.firstVertex;
    } else {
      continue;
    }

    int root = components.findRoot(otherEnd);
    int currentBest = cheapestEdgeToComponent[root];

    if (currentBest == -1 || edge.weight < edges[currentBest].weight) {
      cheapestEdgeToComponent[root] = edgeIndex;
    }
  }

  return cheapestEdgeToComponent;
}

// Этап 4. Подключаем каждую компоненту к specialVertex.
bool connectComponents(int vertexCount, int specialVertex, DSU& components,
                       const vector<int>& cheapestEdgeToComponent,
                       vector<bool>& inTree) {
  for (int vertex = 0; vertex < vertexCount; ++vertex) {
    if (vertex == specialVertex || components.findRoot(vertex) != vertex) {
      continue;
    }

    int edgeIndex = cheapestEdgeToComponent[vertex];

    if (edgeIndex == -1) {
      return false;
    }

    inTree[edgeIndex] = true;
  }

  return true;
}

// Этап 5.1. Строим список соседей текущего дерева.
void buildTreeAdjacency(const vector<Edge>& edges, const vector<bool>& inTree,
                        vector<vector<pair<int, int>>>& treeAdjacency) {
  int vertexCount = treeAdjacency.size();
  int edgeCount = edges.size();

  for (int vertex = 0; vertex < vertexCount; vertex++) {
    treeAdjacency[vertex].clear();
  }

  for (int edgeIndex = 0; edgeIndex < edgeCount; edgeIndex++) {
    if (!inTree[edgeIndex]) {
      continue;
    }

    Edge edge = edges[edgeIndex];

    treeAdjacency[edge.firstVertex].push_back(
        make_pair(edge.secondVertex, edgeIndex));

    treeAdjacency[edge.secondVertex].push_back(
        make_pair(edge.firstVertex, edgeIndex));
  }
}

// Этап 5.2. Обходом BFS находим самое тяжёлое ребро на пути от specialVertex до
// каждой вершины.
void findHeaviestEdges(const vector<Edge>& edges, int specialVertex,
                       const vector<vector<pair<int, int>>>& treeAdjacency,
                       vector<int>& heaviestEdgeOnPath, vector<bool>& visited) {
  int vertexCount = treeAdjacency.size();

  for (int vertex = 0; vertex < vertexCount; vertex++) {
    heaviestEdgeOnPath[vertex] = -1;
    visited[vertex] = false;
  }

  queue<int> bfsQueue;
  bfsQueue.push(specialVertex);
  visited[specialVertex] = true;

  while (!bfsQueue.empty()) {
    int currentVertex = bfsQueue.front();
    bfsQueue.pop();

    for (int i = 0; i < treeAdjacency[currentVertex].size(); i++) {
      int neighbour = treeAdjacency[currentVertex][i].first;
      int edgeIndex = treeAdjacency[currentVertex][i].second;

      if (visited[neighbour]) {
        continue;
      }

      visited[neighbour] = true;

      if (currentVertex == specialVertex) {
        heaviestEdgeOnPath[neighbour] = -1;
      } else {
        int heaviestBefore = heaviestEdgeOnPath[currentVertex];

        if (heaviestBefore == -1 ||
            edges[edgeIndex].weight > edges[heaviestBefore].weight) {
          heaviestEdgeOnPath[neighbour] = edgeIndex;
        } else {
          heaviestEdgeOnPath[neighbour] = heaviestBefore;
        }
      }

      bfsQueue.push(neighbour);
    }
  }
}

// Этап 5.3. Ищем самую выгодную замену.
void findBestReplacement(const vector<Edge>& edges, int specialVertex,
                         const vector<bool>& inTree,
                         const vector<int>& heaviestEdgeOnPath, int& edgeToAdd,
                         int& edgeToRemove) {
  int edgeCount = edges.size();

  long long bestGain = 0;
  edgeToAdd = -1;
  edgeToRemove = -1;

  for (int edgeIndex = 0; edgeIndex < edgeCount; edgeIndex++) {
    if (inTree[edgeIndex]) {
      continue;
    }

    Edge edge = edges[edgeIndex];

    int otherEnd;
    if (edge.firstVertex == specialVertex) {
      otherEnd = edge.secondVertex;
    } else if (edge.secondVertex == specialVertex) {
      otherEnd = edge.firstVertex;
    } else {
      continue;
    }

    int candidateToRemove = heaviestEdgeOnPath[otherEnd];

    if (candidateToRemove == -1) {
      continue;
    }

    long long gain = edges[candidateToRemove].weight;
    gain -= edge.weight;

    if (gain > bestGain) {
      bestGain = gain;
      edgeToAdd = edgeIndex;
      edgeToRemove = candidateToRemove;
    }
  }
}

// Этап 5. Удешевляем дерево, пока позволяет степень specialVertex.
void improveTree(int vertexCount, const vector<Edge>& edges, int specialVertex,
                 int maxDegree, int specialVertexDegree, vector<bool>& inTree) {
  vector<vector<pair<int, int>>> treeAdjacency(vertexCount);
  vector<int> heaviestEdgeOnPath(vertexCount, -1);
  vector<bool> visited(vertexCount, false);

  while (specialVertexDegree < maxDegree) {
    // 5.1. Список соседей текущего дерева.
    buildTreeAdjacency(edges, inTree, treeAdjacency);

    // 5.2. Самые тяжёлые разрешённые к удалению рёбра на путях.
    findHeaviestEdges(edges, specialVertex, treeAdjacency, heaviestEdgeOnPath,
                      visited);

    // 5.3. Самая выгодная замена.
    int edgeToAdd = -1;
    int edgeToRemove = -1;

    findBestReplacement(edges, specialVertex, inTree, heaviestEdgeOnPath,
                        edgeToAdd, edgeToRemove);

    if (edgeToAdd == -1) {
      break;
    }

    // 5.4. Применяем замену.
    inTree[edgeToRemove] = false;
    inTree[edgeToAdd] = true;
    specialVertexDegree++;
  }
}

// Этап 6. Считаем вес итогового дерева.
long long calculateTreeWeight(const vector<Edge>& edges,
                              const vector<bool>& inTree) {
  int edgeCount = edges.size();
  long long totalWeight = 0;

  for (int edgeIndex = 0; edgeIndex < edgeCount; edgeIndex++) {
    if (inTree[edgeIndex]) {
      totalWeight += edges[edgeIndex].weight;
    }
  }

  return totalWeight;
}

long long constrainedMst(int vertexCount, vector<Edge> edges, int specialVertex,
                         int maxDegree) {
  int edgeCount = edges.size();

  if (vertexCount == 1) {
    return 0;
  }

  vector<bool> inTree(edgeCount, false);

  sort(edges.begin(), edges.end(),
       [](Edge a, Edge b) { return a.weight < b.weight; });

  DSU components(vertexCount);

  // Этап 1. Строим лес без specialVertex.
  buildForest(edges, specialVertex, components, inTree);

  // Этап 2. Считаем компоненты и проверяем ограничение степени.
  int componentCount = countComponents(vertexCount, specialVertex, components);

  if (componentCount > maxDegree) {
    return -1;
  }

  // Этап 3. Находим самое дешёвое подключение каждой компоненты.
  vector<int> cheapestEdgeToComponent =
      findCheapestEdges(vertexCount, edges, specialVertex, components);

  // Этап 4. Подключаем компоненты к specialVertex.
  if (!connectComponents(vertexCount, specialVertex, components,
                         cheapestEdgeToComponent, inTree)) {
    return -1;
  }

  // Этап 5. Улучшаем полученное дерево заменами рёбер.
  int specialVertexDegree = componentCount;

  improveTree(vertexCount, edges, specialVertex, maxDegree, specialVertexDegree,
              inTree);

  // Этап 6. Возвращаем суммарный вес дерева.
  return calculateTreeWeight(edges, inTree);
}