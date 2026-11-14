#include <gtest/gtest.h>

#include <vector>

#include "task1.h"
using namespace std;

namespace {

// Ответ - перестановка 0..n-1, и для каждого ребра (a b) b стоит раньше a
bool IsValidOrder(int n, const vector<int>& arr, const vector<int>& order) {
  if (static_cast<int>(order.size()) != n) return false;
  vector<int> pos(n, -1);
  for (int i = 0; i < n; ++i) {
    int v = order[i];
    if (v < 0 || v >= n || pos[v] != -1) return false;
    pos[v] = i;
  }
  for (size_t i = 0; i + 1 < arr.size(); i += 2) {
    if (pos[arr[i + 1]] >= pos[arr[i]]) return false;
  }
  return true;
}

void ExpectOrder(int n, vector<int> arr) {
  int m = static_cast<int>(arr.size()) / 2;
  vector<int> res = task1(n, m, arr);
  EXPECT_TRUE(IsValidOrder(n, arr, res));
}

void ExpectCycle(int n, vector<int> arr) {
  int m = static_cast<int>(arr.size()) / 2;
  vector<int> res = task1(n, m, arr);
  EXPECT_EQ(res, vector<int>{-1});
}

}  // namespace

// ---------- Примеры из условия ----------

TEST(Test, example1) {
  vector<int> arr = {0, 1, 1, 2};
  vector<int> right = {2, 1, 0};
  ASSERT_EQ(task1(3, 2, arr), right);  // порядок единственный
}

TEST(Test, example2) {
  ExpectCycle(4, {0, 1, 1, 2, 2, 0, 3, 1});
}

// ---------- Минимальные случаи ----------

TEST(Test, oneVertexNoEdges) {
  vector<int> arr = {};
  vector<int> right = {0};
  ASSERT_EQ(task1(1, 0, arr), right);
}

TEST(Test, oneVertexSelfLoop) {
  ExpectCycle(1, {0, 0});
}

TEST(Test, twoVerticesNoEdges) {
  ExpectOrder(2, {});
}

TEST(Test, twoVerticesOneEdge) {
  vector<int> arr = {0, 1};
  vector<int> right = {1, 0};
  ASSERT_EQ(task1(2, 1, arr), right);
}

TEST(Test, twoVerticesReverseEdge) {
  vector<int> arr = {1, 0};
  vector<int> right = {0, 1};
  ASSERT_EQ(task1(2, 1, arr), right);
}

TEST(Test, twoVerticesCycle) {
  ExpectCycle(2, {0, 1, 1, 0});
}

// ---------- Циклы ----------

TEST(Test, selfLoopAmongOthers) {
  ExpectCycle(5, {0, 1, 2, 2, 3, 4});
}

TEST(Test, cycleInSeparateComponent) {
  // 0 -> 1 - нормальная цепочка, 2 -> 3 -> 4 -> 2 - цикл
  ExpectCycle(5, {0, 1, 2, 3, 3, 4, 4, 2});
}

TEST(Test, cycleReachableFromOutside) {
  // 0 зависит от цикла 1 -> 2 -> 1
  ExpectCycle(3, {0, 1, 1, 2, 2, 1});
}

TEST(Test, cycleAtTheEndOfLongPath) {
  ExpectCycle(6, {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 3});
}

// ---------- Корректные графы ----------

TEST(Test, noEdges) {
  ExpectOrder(10, {});
}

TEST(Test, chain) {
  vector<int> arr = {0, 1, 1, 2, 2, 3, 3, 4};
  vector<int> right = {4, 3, 2, 1, 0};
  ASSERT_EQ(task1(5, 4, arr), right);
}

TEST(Test, reverseChain) {
  vector<int> arr = {4, 3, 3, 2, 2, 1, 1, 0};
  vector<int> right = {0, 1, 2, 3, 4};
  ASSERT_EQ(task1(5, 4, arr), right);
}

TEST(Test, diamond) {
  // 0 зависит от 1 и 2, оба зависят от 3
  ExpectOrder(4, {0, 1, 0, 2, 1, 3, 2, 3});
}

TEST(Test, star_oneDependsOnMany) {
  ExpectOrder(6, {0, 1, 0, 2, 0, 3, 0, 4, 0, 5});
}

TEST(Test, star_manyDependOnOne) {
  ExpectOrder(6, {1, 0, 2, 0, 3, 0, 4, 0, 5, 0});
}

TEST(Test, twoIndependentChains) {
  ExpectOrder(6, {0, 1, 1, 2, 3, 4, 4, 5});
}

TEST(Test, chainsWithIsolatedVertices) {
  ExpectOrder(8, {0, 1, 1, 2, 5, 6});
}

TEST(Test, duplicateEdges) {
  ExpectOrder(3, {0, 1, 0, 1, 0, 1, 1, 2, 1, 2});
}

TEST(Test, transitiveEdgeRedundant) {
  // 0 -> 1 -> 2 и лишнее ребро 0 -> 2
  ExpectOrder(3, {0, 1, 1, 2, 0, 2});
}

TEST(Test, shuffledLabels) {
  // нумерация не совпадает с порядком установки
  ExpectOrder(6, {3, 5, 5, 0, 0, 4, 4, 1, 1, 2});
}

TEST(Test, completeDag) {
  // i зависит от всех j > i
  int n = 30;
  vector<int> arr;
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      arr.push_back(i);
      arr.push_back(j);
    }
  }
  ExpectOrder(n, arr);
}
