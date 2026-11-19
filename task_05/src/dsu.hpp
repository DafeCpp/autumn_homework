#pragma once

#include <utility>
#include <vector>

struct DSU {
  std::vector<int> parent;
  std::vector<int> rank;

  DSU(int n) : parent(n), rank(n, 0) {
    for (int i = 0; i < n; i++) {
      parent[i] = i;
    }
  }

  int findRoot(int vertex) {
    if (parent[vertex] == vertex) {
      return vertex;
    }

    parent[vertex] = findRoot(parent[vertex]);
    return parent[vertex];
  }

  bool unite(int first, int second) {
    int firstRoot = findRoot(first);
    int secondRoot = findRoot(second);

    if (firstRoot == secondRoot) {
      return false;
    }

    if (rank[firstRoot] < rank[secondRoot]) {
      std::swap(firstRoot, secondRoot);
    }
    parent[secondRoot] = firstRoot;

    if (rank[firstRoot] == rank[secondRoot]) {
      rank[firstRoot]++;
    }
    return true;
  }
};
