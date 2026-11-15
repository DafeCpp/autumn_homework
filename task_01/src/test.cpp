#include <gtest/gtest.h>
#include "../../lib/src/util.hpp"

#include <vector>
#include <utility>

static bool is_valid_order(int n,
                           const std::vector<std::pair<int,int>>& edges,
                           const std::vector<int>& order) {
    if (static_cast<int>(order.size()) != n) return false;

    std::vector<int> seen(n, 0);
    for (int v : order) {
        if (v < 0 || v >= n) return false;
        if (seen[v]++) return false;
    }

    std::vector<int> pos(n);
    for (int i = 0; i < n; ++i) pos[order[i]] = i;

    for (auto [a, b] : edges) {
        if (pos[b] >= pos[a]) return false;
    }
    return true;
}

TEST(TopologicalSort, EmptyGraph) {
    auto order = topological_sort(1, {});
    ASSERT_EQ(order.size(), 1u);
    EXPECT_EQ(order[0], 0);
}

TEST(TopologicalSort, NoEdges) {
    auto order = topological_sort(5, {});
    EXPECT_TRUE(is_valid_order(5, {}, order));
}

TEST(TopologicalSort, SimpleChain) {
    std::vector<std::pair<int,int>> edges = {{0,1},{1,2}};
    auto order = topological_sort(3, edges);
    EXPECT_EQ(order, (std::vector<int>{2,1,0}));
}

TEST(TopologicalSort, ExampleFromStatement1) {
    std::vector<std::pair<int,int>> edges = {{0,1},{1,2}};
    auto order = topological_sort(3, edges);
    EXPECT_TRUE(is_valid_order(3, edges, order));
}

TEST(TopologicalSort, ExampleFromStatement2) {
    std::vector<std::pair<int,int>> edges = {{0,1},{1,2},{2,0},{3,1}};
    auto order = topological_sort(4, edges);
    EXPECT_TRUE(order.empty());
}

TEST(TopologicalSort, SelfLoop) {
    auto order = topological_sort(1, {{0,0}});
    EXPECT_TRUE(order.empty());
}

TEST(TopologicalSort, SelfLoopAmongOthers) {
    std::vector<std::pair<int,int>> edges = {{0,1},{1,1}};
    auto order = topological_sort(3, edges);
    EXPECT_TRUE(order.empty());
}

TEST(TopologicalSort, TwoNodeCycle) {
    auto order = topological_sort(2, {{0,1},{1,0}});
    EXPECT_TRUE(order.empty());
}

TEST(TopologicalSort, ThreeNodeCycle) {
    auto order = topological_sort(3, {{0,1},{1,2},{2,0}});
    EXPECT_TRUE(order.empty());
}

TEST(TopologicalSort, CycleWithIsolatedVertices) {
    std::vector<std::pair<int,int>> edges = {{0,1},{1,0}};
    auto order = topological_sort(4, edges);
    EXPECT_TRUE(order.empty());
}

TEST(TopologicalSort, CycleInOneComponent) {
    std::vector<std::pair<int,int>> edges = {{0,1},{1,0},{2,3}};
    auto order = topological_sort(4, edges);
    EXPECT_TRUE(order.empty());
}

TEST(TopologicalSort, TwoIndependentChains) {
    std::vector<std::pair<int,int>> edges = {{0,1},{1,2},{3,4},{4,5}};
    auto order = topological_sort(6, edges);
    EXPECT_TRUE(is_valid_order(6, edges, order));
}

TEST(TopologicalSort, MultipleSourcesAndSinks) {
    std::vector<std::pair<int,int>> edges = {{0,2},{1,2},{3,4}};
    auto order = topological_sort(5, edges);
    EXPECT_TRUE(is_valid_order(5, edges, order));
}

TEST(TopologicalSort, DuplicateEdges) {
    std::vector<std::pair<int,int>> edges = {{0,1},{0,1},{1,2}};
    auto order = topological_sort(3, edges);
    EXPECT_TRUE(is_valid_order(3, edges, order));
}

TEST(TopologicalSort, StarIn) {
    std::vector<std::pair<int,int>> edges = {{0,1},{0,2},{0,3}};
    auto order = topological_sort(4, edges);
    EXPECT_TRUE(is_valid_order(4, edges, order));
    EXPECT_EQ(order.back(), 0);
}

TEST(TopologicalSort, StarOut) {
    std::vector<std::pair<int,int>> edges = {{1,0},{2,0},{3,0}};
    auto order = topological_sort(4, edges);
    EXPECT_TRUE(is_valid_order(4, edges, order));
    EXPECT_EQ(order.front(), 0);
}

TEST(TopologicalSort, LargeChain) {
    const int n = 1000;
    std::vector<std::pair<int,int>> edges;
    edges.reserve(n - 1);
    for (int i = 0; i + 1 < n; ++i) edges.push_back({i, i + 1});

    auto order = topological_sort(n, edges);
    EXPECT_TRUE(is_valid_order(n, edges, order));
    for (int i = 0; i < n; ++i) EXPECT_EQ(order[i], n - 1 - i);
}

TEST(TopologicalSort, LargeDagWithManyEdges) {
    const int n = 500;
    std::vector<std::pair<int,int>> edges;
    for (int a = 0; a < n; ++a)
        for (int b = 0; b < a; b += 7)
            edges.push_back({a, b});

    auto order = topological_sort(n, edges);
    EXPECT_TRUE(is_valid_order(n, edges, order));
}