#include <gtest/gtest.h>

#include "max_flow_solver.hpp"

TEST(MaxFlow, SimpleGraph) {
  MaxFlowSolver solver(3);
  solver.addEdge(1, 2, 10);
  solver.addEdge(2, 3, 5);
  EXPECT_EQ(5, solver.maxFlow(1, 3));
}

TEST(MaxFlow, ParallelPaths) {
  MaxFlowSolver solver(4);
  solver.addEdge(1, 2, 10);
  solver.addEdge(2, 4, 10);
  solver.addEdge(1, 3, 15);
  solver.addEdge(3, 4, 15);
  EXPECT_EQ(25, solver.maxFlow(1, 4));
}

TEST(MaxFlow, DiamondGraph) {
  MaxFlowSolver solver(4);
  solver.addEdge(1, 2, 10);
  solver.addEdge(1, 3, 10);
  solver.addEdge(2, 4, 5);
  solver.addEdge(3, 4, 5);
  solver.addEdge(2, 3, 5);
  EXPECT_EQ(10, solver.maxFlow(1, 4));
}

TEST(MaxFlow, NoPath) {
  MaxFlowSolver solver(4);
  solver.addEdge(1, 2, 10);
  solver.addEdge(3, 4, 10);
  EXPECT_EQ(0, solver.maxFlow(1, 4));
}

TEST(MaxFlow, SingleEdge) {
  MaxFlowSolver solver(2);
  solver.addEdge(1, 2, 100);
  EXPECT_EQ(100, solver.maxFlow(1, 2));
}

TEST(MaxFlow, BottleneckInMiddle) {
  MaxFlowSolver solver(4);
  solver.addEdge(1, 2, 100);
  solver.addEdge(2, 3, 1);
  solver.addEdge(3, 4, 100);
  EXPECT_EQ(1, solver.maxFlow(1, 4));
}

TEST(MaxFlow, MultipleEdgesBetweenSameVertices) {
  MaxFlowSolver solver(3);
  solver.addEdge(1, 2, 5);
  solver.addEdge(1, 2, 10);
  solver.addEdge(2, 3, 20);
  EXPECT_EQ(15, solver.maxFlow(1, 3));
}

TEST(MaxFlow, ClassicExample) {
  MaxFlowSolver solver(6);
  solver.addEdge(1, 2, 16);
  solver.addEdge(1, 3, 13);
  solver.addEdge(2, 3, 10);
  solver.addEdge(2, 4, 12);
  solver.addEdge(3, 2, 4);
  solver.addEdge(3, 5, 14);
  solver.addEdge(4, 3, 9);
  solver.addEdge(4, 6, 20);
  solver.addEdge(5, 4, 7);
  solver.addEdge(5, 6, 4);
  EXPECT_EQ(23, solver.maxFlow(1, 6));
}

TEST(MaxFlow, ZeroCapacity) {
  MaxFlowSolver solver(3);
  solver.addEdge(1, 2, 0);
  solver.addEdge(2, 3, 10);
  EXPECT_EQ(0, solver.maxFlow(1, 3));
}

TEST(MaxFlow, LargeCapacity) {
  MaxFlowSolver solver(2);
  solver.addEdge(1, 2, 1000000000LL);
  EXPECT_EQ(1000000000LL, solver.maxFlow(1, 2));
}