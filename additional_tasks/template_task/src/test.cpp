#include <gtest/gtest.h>

#include "utils.hpp"

TEST(AStar, LinearGraph) {
  CoordinatedGraph g({{{1, 1}}, {{2, 1}}, {{3, 1}}, {}},
                     {{0, 0}, {1, 0}, {2, 0}, {3, 0}});

  Astar a(g, 0, 3);
  auto path = a.FindPath();

  std::vector<int> expected = {0, 1, 2, 3};
  EXPECT_EQ(path, expected);
}

TEST(AStar, ChooseShortest) {
  CoordinatedGraph g({{{1, 1}, {2, 5}}, {{3, 1}}, {{3, 5}}, {}},
                     {{0, 0}, {1, 0}, {0, 1}, {2, 0}});

  Astar a(g, 0, 3);
  auto path = a.FindPath();
  std::vector<int> expected = {0, 1, 3};

  EXPECT_EQ(path, expected);
}

TEST(AStar, NoPath) {
  CoordinatedGraph g({{{1, 1}}, {}, {{3, 1}}, {}},
                     {{0, 0}, {1, 0}, {0, 1}, {1, 1}});

  Astar a(g, 0, 3);

  EXPECT_THROW(a.FindPath(), std::runtime_error);
}

TEST(AStar, Disconnected) {
  CoordinatedGraph g({{}, {}, {}, {}}, {{0, 0}, {1, 0}, {2, 0}, {3, 0}});

  Astar a(g, 0, 3);

  EXPECT_THROW(a.FindPath(), std::runtime_error);
}

TEST(AStar, Cycles) {
  CoordinatedGraph g({{{1, 1}}, {{2, 1}}, {{1, 1}, {3, 1}}, {}},
                     {{0, 0}, {1, 0}, {2, 0}, {3, 0}});

  Astar a(g, 0, 3);
  auto path = a.FindPath();

  EXPECT_EQ(path, (std::vector<int>{0, 1, 2, 3}));
}