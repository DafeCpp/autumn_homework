#include <gtest/gtest.h>
#include "task6.h"
#include <vector>

using namespace std;

TEST(MaxFlowTest, SimpleLinearPath) {
    int n = 3, m = 2;
    vector<vector<int>> edges = {
        {1, 2, 10}, 
        {2, 3, 5}  
    };
    EXPECT_EQ(task6(n, m, edges, 1, n), 5);
}

TEST(MaxFlowTest, ParallelPaths) {
    int n = 4, m = 4;
    vector<vector<int>> edges = {
        {1, 2, 10}, {2, 4, 10}, 
        {1, 3, 5},  {3, 4, 5}
    };
    EXPECT_EQ(task6(n, m, edges, 1, n), 15);
}

TEST(MaxFlowTest, DiamondGraphWithCrossEdge) {
    int n = 4, m = 5;
    vector<vector<int>> edges = {
        {1, 2, 1000}, {1, 3, 1000},
        {2, 4, 1000}, {3, 4, 1000},
        {2, 3, 1} 
    };
    EXPECT_EQ(task6(n, m, edges, 1, n), 2000);
}

TEST(MaxFlowTest, DisconnectedGraph) {
    int n = 4, m = 2;
    vector<vector<int>> edges = {
        {1, 2, 10}, 
        {3, 4, 10}
    };
    EXPECT_EQ(task6(n, m, edges, 1, n), 0);
}

TEST(MaxFlowTest, GraphWithCycle) {
    int n = 4, m = 5;
    vector<vector<int>> edges = {
        {1, 2, 10},
        {2, 3, 10},
        {3, 2, 10},
        {2, 4, 5},
        {3, 4, 5}
    };
    EXPECT_EQ(task6(n, m, edges, 1, n), 10);
}

TEST(MaxFlowTest, MultipleEdges) {
    int n = 2, m = 3;
    vector<vector<int>> edges = {
        {1, 2, 5},
        {1, 2, 10},
        {1, 2, 15}
    };
    EXPECT_EQ(task6(n, m, edges, 1, n), 30);
}

TEST(MaxFlowTest, TwoVertices) {
    int n = 2, m = 1;
    vector<vector<int>> edges = {
        {1, 2, 42}
    };
    EXPECT_EQ(task6(n, m, edges, 1, n), 42);
}

TEST(MaxFlowTest, NoEdges) {
    int n = 3, m = 0;
    vector<vector<int>> edges = {};
    EXPECT_EQ(task6(n, m, edges, 1, n), 0);
}