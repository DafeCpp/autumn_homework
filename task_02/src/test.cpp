#include <gtest/gtest.h>
#include "../../lib/src/util.hpp"


TEST(KosarajuTest, Example1) {
  std::vector<std::vector<int>> adjList(2);
  adjList[0].push_back(1);

  Graph g(adjList);
  int answer = g.MinEdgesToMakeStronglyConnected();

  ASSERT_EQ(answer, 1);
}


TEST(KosarajuTest, Example2) {
  std::vector<std::vector<int>> adjList(4);
  adjList[0].push_back(1);
  adjList[2].push_back(1);
  adjList[1].push_back(3);

  Graph g(adjList);
  int answer = g.MinEdgesToMakeStronglyConnected();

  ASSERT_EQ(answer, 2);
}


TEST(KosarajuTest, Example3) {
  std::vector<std::vector<int>> adjList(5);
  adjList[0].push_back(1);
  adjList[2].push_back(1);
  adjList[1].push_back(3);
  adjList[1].push_back(4);

  Graph g(adjList);
  int answer = g.MinEdgesToMakeStronglyConnected();

  ASSERT_EQ(answer, 2);
}

TEST(KosarajuTest, Example4) {
  std::vector<std::vector<int>> adjList(6);
  adjList[0].push_back(1);
  adjList[1].push_back(2);
  adjList[2].push_back(0);
  adjList[3].push_back(4);
  adjList[4].push_back(5);
  adjList[5].push_back(3);

  Graph g(adjList);
  int answer = g.MinEdgesToMakeStronglyConnected();

  ASSERT_EQ(answer, 2);
}
TEST(KosarajuTest, Example5) {
  std::vector<std::vector<int>> adjList(7);
  adjList[0].push_back(1);
  adjList[1].push_back(2);
  adjList[2].push_back(0);
  adjList[3].push_back(4);
  adjList[4].push_back(5);
  adjList[5].push_back(3);
  adjList[6].push_back(5);

  Graph g(adjList);
  int answer = g.MinEdgesToMakeStronglyConnected();

  ASSERT_EQ(answer, 2);
}
