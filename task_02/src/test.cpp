#include <gtest/gtest.h>

#include <vector>

#include "districts.hpp"

TEST(Test_MinEdgesToMakeOneQuarter, Test_SimpleTwoNodes) {
  Graph<int> graph(true);
  graph.AddEdge(0, 1);

  int result = MinEdgesToMakeOneDistrict(graph);
  ASSERT_EQ(result, 1);
}

TEST(Test_MinEdgesToMakeOneQuarter, Test_AlreadyOneSCC) {
  Graph<int> graph(true);
  graph.AddEdge(0, 1);
  graph.AddEdge(1, 2);
  graph.AddEdge(2, 0);

  int result = MinEdgesToMakeOneDistrict(graph);
  ASSERT_EQ(result, 0);
}

TEST(Test_MinEdgesToMakeOneQuarter, Test_LinearChain) {
  Graph<int> graph(true);
  for (int i = 0; i < 4; i++) graph.AddEdge(i, i + 1);

  int result = MinEdgesToMakeOneDistrict(graph);
  ASSERT_EQ(result, 1);
}

TEST(Test_MinEdgesToMakeOneQuarter, Test_SingleNode) {
  Graph<int> graph(true);
  graph.AddVertex(0);

  int result = MinEdgesToMakeOneDistrict(graph);
  ASSERT_EQ(result, 0);
}
