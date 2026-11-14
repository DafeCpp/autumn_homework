#include <gtest/gtest.h>
#include <vector>
#include "task2.h"

using std::vector;

static int Solve(int n, const vector<std::pair<int, int>>& edges) {
  vector<int> arr;
  arr.reserve(edges.size() * 2);
  for (auto& [a, b] : edges) {
    arr.push_back(a);
    arr.push_back(b);
  }
  return task2(n, static_cast<int>(edges.size()), arr);
}


TEST(Examples, Example1) {
  EXPECT_EQ(1, Solve(2, {{1, 2}}));
}

TEST(Examples, Example2) {
  EXPECT_EQ(2, Solve(4, {{1, 2}, {3, 2}, {2, 4}}));
}

TEST(Examples, Example3) {
  EXPECT_EQ(2, Solve(5, {{1, 2}, {3, 2}, {2, 4}, {2, 5}}));
}


TEST(Edge, SingleVertexNoEdges) {
  EXPECT_EQ(0, Solve(1, {}));
}

TEST(Edge, SingleVertexSelfLoop) {
  EXPECT_EQ(0, Solve(1, {{1, 1}}));
}

TEST(Edge, TwoIsolatedVertices) {
  EXPECT_EQ(2, Solve(2, {}));
}

TEST(Edge, ThreeIsolatedVertices) {
  EXPECT_EQ(3, Solve(3, {}));
}

TEST(Edge, IsolatedVerticesWithSelfLoops) {
  EXPECT_EQ(3, Solve(3, {{1, 1}, {2, 2}, {3, 3}}));
}


TEST(StronglyConnected, TwoVerticesBothDirections) {
  EXPECT_EQ(0, Solve(2, {{1, 2}, {2, 1}}));
}

TEST(StronglyConnected, SimpleCycle) {
  EXPECT_EQ(0, Solve(3, {{1, 2}, {2, 3}, {3, 1}}));
}

TEST(StronglyConnected, CycleWithChordsAndSelfLoops) {
  EXPECT_EQ(0, Solve(4, {{1, 2}, {2, 3}, {3, 4}, {4, 1}, {1, 3}, {2, 2}, {4, 2}}));
}


TEST(Structure, Path) {
  EXPECT_EQ(1, Solve(4, {{1, 2}, {2, 3}, {3, 4}}));
}

TEST(Structure, OutStar) {
  EXPECT_EQ(3, Solve(4, {{1, 2}, {1, 3}, {1, 4}}));
}

TEST(Structure, InStar) {
  EXPECT_EQ(3, Solve(4, {{2, 1}, {3, 1}, {4, 1}}));
}

TEST(Structure, Diamond) {
  EXPECT_EQ(1, Solve(4, {{1, 2}, {1, 3}, {2, 4}, {3, 4}}));
}

TEST(Structure, TwoDisjointEdges) {
  EXPECT_EQ(2, Solve(4, {{1, 2}, {3, 4}}));
}


TEST(SCC, TwoCyclesConnectedOneWay) {
  EXPECT_EQ(1, Solve(4, {{1, 2}, {2, 1}, {2, 3}, {3, 4}, {4, 3}}));
}

TEST(SCC, CycleAndIsolatedVertex) {
  EXPECT_EQ(2, Solve(4, {{1, 2}, {2, 3}, {3, 1}}));
}

TEST(SCC, ChainOfCyclesMiddleHasInAndOut) {
  EXPECT_EQ(1, Solve(6, {{1, 2}, {2, 1}, {2, 3}, {3, 4}, {4, 3}, {4, 5}, {5, 6}, {6, 5}}));
}

TEST(SCC, SourcesMoreThanSinks) {
  EXPECT_EQ(2, Solve(5, {{1, 3}, {2, 3}, {3, 4}, {4, 5}, {5, 4}}));
}

TEST(SCC, SinksMoreThanSources) {
  EXPECT_EQ(3, Solve(5, {{1, 2}, {1, 3}, {1, 4}, {4, 5}, {5, 4}}));
}


TEST(Multi, DuplicateEdges) {
  EXPECT_EQ(1, Solve(2, {{1, 2}, {1, 2}, {1, 2}}));
}

TEST(Multi, SelfLoopsDoNotAffectAnswer) {
  EXPECT_EQ(1, Solve(2, {{1, 1}, {1, 2}, {2, 2}}));
}


TEST(Order, ReversedPath) {
  EXPECT_EQ(1, Solve(4, {{4, 3}, {3, 2}, {2, 1}}));
}

TEST(Order, DisconnectedMixed) {
  EXPECT_EQ(3, Solve(5, {{1, 2}, {2, 1}, {3, 4}}));
}
