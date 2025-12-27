#include <gtest/gtest.h>

#include <vector>

#include "max_flow.hpp"

TEST(TestMaxFlow, SimpleCase) {
  MaxFlow mf(2);
  mf.addEdge(0, 1, 5);
  ASSERT_EQ(mf.edmondsKarp(0, 1), 5);
}

TEST(TestMaxFlow, SmallGraph) {
  MaxFlow mf(4);
  mf.addEdge(0, 1, 100);
  mf.addEdge(0, 2, 100);
  mf.addEdge(1, 2, 1);
  mf.addEdge(1, 3, 100);
  mf.addEdge(2, 3, 100);
  ASSERT_EQ(mf.edmondsKarp(0, 3), 200);
}

TEST(TestMaxFlow, NoFlow) {
  MaxFlow mf(3);
  ASSERT_EQ(mf.edmondsKarp(0, 2), 0);
}

TEST(TestMaxFlow, ParallelEdges) {
  MaxFlow mf(2);
  mf.addEdge(0, 1, 3);
  mf.addEdge(0, 1, 4);
  ASSERT_EQ(mf.edmondsKarp(0, 1), 7);
}

TEST(TestMaxFlow, Chain) {
  MaxFlow mf(5);
  mf.addEdge(0, 1, 10);
  mf.addEdge(1, 2, 5);
  mf.addEdge(2, 3, 7);
  mf.addEdge(3, 4, 10);
  ASSERT_EQ(mf.edmondsKarp(0, 4), 5);
}

TEST(TestMaxFlow, Complex_Graph) {
  MaxFlow mf(7);
  mf.addEdge(0, 1, 13);
  mf.addEdge(1, 5, 22);
  mf.addEdge(1, 6, 7);
  mf.addEdge(6, 2, 14);
  mf.addEdge(0, 4, 3);
  mf.addEdge(4, 3, 16);
  mf.addEdge(3, 2, 9);
  mf.addEdge(3, 6, 10);
  mf.addEdge(5, 4, 8);
  mf.addEdge(5, 3, 7);
  mf.addEdge(5, 6, 2);
  mf.addEdge(5, 2, 6);
  ASSERT_EQ(mf.edmondsKarp(0, 2), 16);
}
