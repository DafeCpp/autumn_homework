#include <gtest/gtest.h>
#include "../../lib/src/util.hpp"
#include <vector>

TEST(SCCMinEdges, Example1) {
    EXPECT_EQ(min_edges_to_make_strongly_connected(2, {{1, 2}}), 1);
}

TEST(SCCMinEdges, Example2) {
    EXPECT_EQ(min_edges_to_make_strongly_connected(4, {{1, 2}, {3, 2}, {2, 4}}), 2);
}

TEST(SCCMinEdges, Example3) {
    EXPECT_EQ(min_edges_to_make_strongly_connected(5, {{1, 2}, {3, 2}, {2, 4}, {2, 5}}), 2);
}

TEST(SCCMinEdges, EmptyGraph) {
    EXPECT_EQ(min_edges_to_make_strongly_connected(5, {}), 5);
}

TEST(SCCMinEdges, AlreadyStronglyConnected) {
    EXPECT_EQ(min_edges_to_make_strongly_connected(3, {{1, 2}, {2, 3}, {3, 1}}), 0);
}

TEST(SCCMinEdges, SingleNode) {
    EXPECT_EQ(min_edges_to_make_strongly_connected(1, {}), 0);
}

TEST(SCCMinEdges, SingleNodeWithLoop) {
    EXPECT_EQ(min_edges_to_make_strongly_connected(1, {{1, 1}}), 0);
}

TEST(SCCMinEdges, TreeIntoCenter) {
    int n = 4;
    std::vector<std::pair<int,int>> edges = {{1, 4}, {2, 4}, {3, 4}};
    EXPECT_EQ(min_edges_to_make_strongly_connected(n, edges), 3);
}