#include <gtest/gtest.h>

#include "get_critical_elements.hpp"

TEST(CriticalElements, Cycle) {
  std::vector<std::set<int>> graf{{1, 3}, {0, 2}, {1, 3}, {0, 2}};
  AdditionalRoads elements(graf);
  EXPECT_TRUE(elements.critical_devices.empty());
  EXPECT_TRUE(elements.critical_wiring.empty());
}

TEST(CriticalElements, Path) {
  std::vector<std::set<int>> graf{{1}, {0, 2}, {1, 3}, {2}};
  AdditionalRoads elements(graf);
  ASSERT_EQ(elements.critical_devices, (std::set<int>{1, 2}));
  ASSERT_EQ(elements.critical_wiring.size(), 3);
  EXPECT_TRUE(elements.critical_wiring.count({0, 1}));
  EXPECT_TRUE(elements.critical_wiring.count({1, 2}));
  EXPECT_TRUE(elements.critical_wiring.count({2, 3}));
}

TEST(CriticalElements, Star) {
  std::vector<std::set<int>> graf{{1, 2, 3}, {0}, {0}, {0}};
  AdditionalRoads elements(graf);
  ASSERT_EQ(elements.critical_devices, (std::set<int>{0}));
  ASSERT_EQ(elements.critical_wiring.size(), 3);
  EXPECT_TRUE(elements.critical_wiring.count({0, 1}));
  EXPECT_TRUE(elements.critical_wiring.count({0, 2}));
  EXPECT_TRUE(elements.critical_wiring.count({0, 3}));
}

TEST(CriticalElements, Bow) {
  std::vector<std::set<int>> graf{{1, 2}, {0, 2}, {0, 1, 3, 4}, {2, 4}, {2, 3}};
  AdditionalRoads elements(graf);
  ASSERT_EQ(elements.critical_devices, (std::set<int>{2}));
  EXPECT_TRUE(elements.critical_wiring.empty());
}

TEST(CriticalElements, SingleEdge) {
  std::vector<std::set<int>> graf{{1}, {0}};
  AdditionalRoads elements(graf);
  EXPECT_TRUE(elements.critical_devices.empty());
  ASSERT_EQ(elements.critical_wiring.size(), 1);
  EXPECT_TRUE(elements.critical_wiring.count({0, 1}));
}

TEST(CriticalElements, TwoPaths) {
  std::vector<std::set<int>> graf{{1}, {0, 2}, {1}, {4}, {3, 5}, {4}};
  AdditionalRoads elements(graf);
  ASSERT_EQ(elements.critical_devices, (std::set<int>{1, 4}));
  ASSERT_EQ(elements.critical_wiring.size(), 4);
  EXPECT_TRUE(elements.critical_wiring.count({0, 1}));
  EXPECT_TRUE(elements.critical_wiring.count({1, 2}));
  EXPECT_TRUE(elements.critical_wiring.count({3, 4}));
  EXPECT_TRUE(elements.critical_wiring.count({4, 5}));
}

TEST(CriticalElements, SingleVertex) {
  std::vector<std::set<int>> graf{{}};
  AdditionalRoads elements(graf);
  EXPECT_TRUE(elements.critical_devices.empty());
  EXPECT_TRUE(elements.critical_wiring.empty());
}

TEST(CriticalElements, TwoIsolatedVertices) {
  std::vector<std::set<int>> graf{{}, {}};
  AdditionalRoads elements(graf);
  EXPECT_TRUE(elements.critical_devices.empty());
  EXPECT_TRUE(elements.critical_wiring.empty());
}