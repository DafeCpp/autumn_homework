#include <gtest/gtest.h>

#include <stdexcept>
#include <vector>

#include "bfs.hpp"

namespace bfs_example {
TEST(Bfs, SingleVertex) { EXPECT_EQ(Bfs(1, {}), (std::vector<int>{-1, 0})); }

TEST(Bfs, NoEdgesLeavesOtherVerticesUnreachable) {
  EXPECT_EQ(Bfs(4, {}), (std::vector<int>{-1, 0, -1, -1, -1}));
}

TEST(Bfs, Chain) {
  EXPECT_EQ(Bfs(4, {{1, 2}, {2, 3}, {3, 4}}),
            (std::vector<int>{-1, 0, 1, 2, 3}));
}

TEST(Bfs, TraversesEdgesInBothDirections) {
  EXPECT_EQ(Bfs(4, {{2, 1}, {3, 2}, {4, 3}}),
            (std::vector<int>{-1, 0, 1, 2, 3}));
}

TEST(Bfs, CycleChoosesTheShortestRoute) {
  EXPECT_EQ(Bfs(4, {{1, 2}, {2, 3}, {3, 4}, {4, 1}}),
            (std::vector<int>{-1, 0, 1, 2, 1}));
}

TEST(Bfs, MultipleShortestPaths) {
  EXPECT_EQ(Bfs(5, {{1, 2}, {1, 3}, {2, 4}, {3, 4}, {4, 5}}),
            (std::vector<int>{-1, 0, 1, 1, 2, 3}));
}

TEST(Bfs, DisconnectedComponentStaysUnreachable) {
  EXPECT_EQ(Bfs(5, {{1, 2}, {3, 4}, {4, 5}}),
            (std::vector<int>{-1, 0, 1, -1, -1, -1}));
}

TEST(Bfs, SelfLoopsAndParallelEdgesDoNotChangeDistances) {
  EXPECT_EQ(Bfs(3, {{1, 1}, {1, 2}, {1, 2}, {2, 2}, {2, 3}}),
            (std::vector<int>{-1, 0, 1, 2}));
}

TEST(Bfs, EdgeOrderDoesNotAffectDistances) {
  EXPECT_EQ(Bfs(4, {{3, 4}, {2, 4}, {1, 3}, {1, 2}}),
            (std::vector<int>{-1, 0, 1, 1, 2}));
}

TEST(Bfs, LongChainDoesNotNeedRecursiveTraversal) {
  constexpr int kVertices = 20000;
  std::vector<Edge> edges;
  for (int v = 1; v < kVertices; ++v) edges.push_back({v, v + 1});
  const auto distance = Bfs(kVertices, edges);
  ASSERT_EQ(distance.size(), kVertices + 1);
  for (int v = 1; v <= kVertices; ++v) ASSERT_EQ(distance[v], v - 1);
}

TEST(Bfs, RejectsGraphsWithoutTheSourceVertex) {
  EXPECT_THROW(Bfs(0, {}), std::invalid_argument);
  EXPECT_THROW(Bfs(-1, {}), std::invalid_argument);
}

TEST(Bfs, RejectsInvalidEndpoints) {
  EXPECT_THROW(Bfs(3, {{0, 1}}), std::invalid_argument);
  EXPECT_THROW(Bfs(3, {{1, 4}}), std::invalid_argument);
  EXPECT_THROW(Bfs(3, {{-1, 2}}), std::invalid_argument);
}
}  // namespace bfs_example
