#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "task5.h"

using namespace std;

const string ERROR_MSG =
    "Невозможно построить остовное дерево с заданным ограничением степени";

TEST(Task5Test, SimpleValidMST) {
  int n = 3, m = 3, d = 2;
  vector<int> arr = {1, 2, 1, 2, 3, 2, 1, 3, 3};
  vector<string> expected = {"3"};
  EXPECT_EQ(task5(n, m, d, arr), expected);
}

TEST(Task5Test, ImpossibleDegreeConstraint) {
  int n = 4, m = 3, d = 2;
  vector<int> arr = {1, 2, 1, 1, 3, 1, 1, 4, 1};
  vector<string> expected = {ERROR_MSG};
  EXPECT_EQ(task5(n, m, d, arr), expected);
}

TEST(Task5Test, StarGraphDegreeAllowed) {
  int n = 4, m = 3, d = 3;
  vector<int> arr = {1, 2, 1, 1, 3, 1, 1, 4, 1};
  vector<string> expected = {"3"};
  EXPECT_EQ(task5(n, m, d, arr), expected);
}

TEST(Task5Test, DisconnectedGraph) {
  int n = 4, m = 2, d = 3;
  vector<int> arr = {1, 2, 5, 3, 4, 10};
  vector<string> expected = {ERROR_MSG};
  EXPECT_EQ(task5(n, m, d, arr), expected);
}

TEST(Task5Test, MultipleEdgesAndLoops) {
  int n = 2, m = 3, d = 2;
  vector<int> arr = {1, 2, 5, 1, 2, 1, 1, 1, 2};
  vector<string> expected = {"1"};
  EXPECT_EQ(task5(n, m, d, arr), expected);
}

TEST(Task5Test, ChainGraph) {
  int n = 5, m = 4, d = 2;
  vector<int> arr = {1, 2, 10, 2, 3, 20, 3, 4, 30, 4, 5, 40};
  vector<string> expected = {"100"};
  EXPECT_EQ(task5(n, m, d, arr), expected);
}

TEST(Task5Test, SingleVertex) {
  int n = 1, m = 0, d = 1;
  vector<int> arr = {};
  vector<string> expected = {"0"};
  EXPECT_EQ(task5(n, m, d, arr), expected);
}