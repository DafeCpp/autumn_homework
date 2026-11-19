#pragma once

#include <utility>
#include <vector>

#include "dsu.hpp"
#include "graph.hpp"

void buildForest(const std::vector<Edge>& edges, int specialVertex,
                 DSU& components, std::vector<bool>& inTree);

int countComponents(int vertexCount, int specialVertex, DSU& components);

std::vector<int> findCheapestEdges(int vertexCount,
                                   const std::vector<Edge>& edges,
                                   int specialVertex, DSU& components);

bool connectComponents(int vertexCount, int specialVertex, DSU& components,
                       const std::vector<int>& cheapestEdgeToComponent,
                       std::vector<bool>& inTree);

void buildTreeAdjacency(
    const std::vector<Edge>& edges, const std::vector<bool>& inTree,
    std::vector<std::vector<std::pair<int, int>>>& treeAdjacency);

void findHeaviestEdges(
    const std::vector<Edge>& edges, int specialVertex,
    const std::vector<std::vector<std::pair<int, int>>>& treeAdjacency,
    std::vector<int>& heaviestEdgeOnPath, std::vector<bool>& visited);

void findBestReplacement(const std::vector<Edge>& edges, int specialVertex,
                         const std::vector<bool>& inTree,
                         const std::vector<int>& heaviestEdgeOnPath,
                         int& edgeToAdd, int& edgeToRemove);

void improveTree(int vertexCount, const std::vector<Edge>& edges,
                 int specialVertex, int maxDegree, int specialVertexDegree,
                 std::vector<bool>& inTree);

long long calculateTreeWeight(const std::vector<Edge>& edges,
                              const std::vector<bool>& inTree);

long long constrainedMst(int vertexCount, std::vector<Edge> edges,
                         int specialVertex, int maxDegree);
