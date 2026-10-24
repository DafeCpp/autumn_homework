#include <gtest/gtest.h>

#include "topology_sort.hpp"

// Здесь функцию IsOrderCorrect я использую для проверки тоько тех случаев,
// когда у нас можно составить порядок.

bool IsOrderCorrect(const std::vector<std::vector<int>> &graf,
                    std::vector<int> result) {
  if (result == std::vector<int>{-1}) return false;
  if (result.size() != graf.size()) return false;

  std::vector<int> positions(graf.size(), -1);
  for (int i = 0; i < result.size(); ++i) {
    int v = result[i];
    if (v < 0 || v >= graf.size()) return false;
    if (positions[v] != -1) return false;
    positions[v] = i;
  }

  for (int a = 0; a < graf.size(); ++a) {
    for (int b : graf[a]) {
      if (positions[b] >= positions[a]) return false;
    }
  }
  return true;
}

TEST(Test, Simple) {
  std::vector<std::vector<int>> graf = {{}, {0}, {1}};
  EXPECT_TRUE(IsOrderCorrect(graf, TopologySort(graf)));

  std::vector<std::vector<int>> graf2 = {{1}, {2}, {0}, {1}};
  ASSERT_EQ(TopologySort(graf2), std::vector<int>{-1});
}

TEST(Test, WithLoop) {
  std::vector<std::vector<int>> graf = {{1}, {2}, {1}};
  ASSERT_EQ(TopologySort(graf), std::vector<int>{-1});
}

TEST(Test, SingleVertex) {
  std::vector<std::vector<int>> graf = {{}};
  EXPECT_TRUE(IsOrderCorrect(graf, TopologySort(graf)));
}

TEST(Test, NoEdges) {
  std::vector<std::vector<int>> graf = {{}, {}, {}, {}};
  EXPECT_TRUE(IsOrderCorrect(graf, TopologySort(graf)));
}

TEST(Test, OneDependsOnMany) {
  std::vector<std::vector<int>> graf = {{1, 2, 3}, {}, {}, {}};
  EXPECT_TRUE(IsOrderCorrect(graf, TopologySort(graf)));
}

TEST(Test, ManyDependOnOne) {
  std::vector<std::vector<int>> graf = {{}, {0}, {0}, {0}};
  EXPECT_TRUE(IsOrderCorrect(graf, TopologySort(graf)));
}

TEST(Test, TwoIndependentChains) {
  std::vector<std::vector<int>> graf = {{1}, {2}, {}, {4}, {}};
  EXPECT_TRUE(IsOrderCorrect(graf, TopologySort(graf)));
}

TEST(Test, SelfLoop) {
  std::vector<std::vector<int>> graf = {{0}};
  ASSERT_EQ(TopologySort(graf), std::vector<int>{-1});
}

TEST(Test, ManyComponentsWithCycle) {
  std::vector<std::vector<int>> graf = {{1}, {2}, {}, {4}, {5}, {3}, {}};
  ASSERT_EQ(TopologySort(graf), std::vector<int>{-1});
}