#include <gtest/gtest.h>

#include "kosaraju.h"

TEST(KosarajuTest, Example1) {
  KosarajuSolver solver;
  solver.init(2, {{1, 2}});
  EXPECT_EQ(solver.solve(), 1);
}

TEST(KosarajuTest, Example2) {
  KosarajuSolver solver;
  solver.init(4, {{1, 2}, {3, 2}, {2, 4}});
  EXPECT_EQ(solver.solve(), 2);
}

TEST(KosarajuTest, Example3) {
  KosarajuSolver solver;
  solver.init(5, {{1, 2}, {3, 2}, {2, 4}, {2, 5}});
  EXPECT_EQ(solver.solve(), 2);
}

TEST(KosarajuTest, AlreadyStronglyConnected) {
  KosarajuSolver solver;
  solver.init(3, {{1, 2}, {2, 3}, {3, 1}});
  EXPECT_EQ(solver.solve(), 0);
}

TEST(KosarajuTest, SingleVertex) {
  KosarajuSolver solver;
  solver.init(1, {});
  EXPECT_EQ(solver.solve(), 0);
}

TEST(KosarajuTest, TwoIsolatedVertices) {
  KosarajuSolver solver;
  solver.init(2, {});
  EXPECT_EQ(solver.solve(), 2);
}

TEST(KosarajuTest, GraphWithSelfLoop) {
  KosarajuSolver solver;
  solver.init(3, {{1, 1}, {1, 2}, {2, 3}, {3, 2}});
  EXPECT_EQ(solver.solve(), 1);
}

TEST(KosarajuTest, LinearChain) {
  KosarajuSolver solver;
  solver.init(4, {{1, 2}, {2, 3}, {3, 4}});
  EXPECT_EQ(solver.solve(), 1);
}

TEST(KosarajuTest, TwoSCCs) {
  KosarajuSolver solver;
  solver.init(6, {{1, 2}, {2, 1}, {3, 4}, {4, 5}, {5, 3}, {3, 6}});
  EXPECT_EQ(solver.solve(), 2);
}

TEST(KosarajuTest, AllIsolated) {
  KosarajuSolver solver;
  solver.init(5, {});
  EXPECT_EQ(solver.solve(), 5);
}

TEST(KosarajuTest, StarGraph) {
  KosarajuSolver solver;
  solver.init(5, {{1, 2}, {1, 3}, {1, 4}, {1, 5}});
  EXPECT_EQ(solver.solve(), 4);
}

TEST(KosarajuTest, ReverseStar) {
  KosarajuSolver solver;
  solver.init(5, {{2, 1}, {3, 1}, {4, 1}, {5, 1}});
  EXPECT_EQ(solver.solve(), 4);
}

TEST(KosarajuTest, ComplexCase) {
  KosarajuSolver solver;
  solver.init(7,
              {{1, 2}, {2, 1}, {2, 3}, {3, 4}, {4, 3}, {4, 5}, {5, 6}, {6, 7}});
  EXPECT_EQ(solver.solve(), 1);
}

TEST(KosarajuTest, CompleteGraph) {
  KosarajuSolver solver;
  solver.init(4, {{1, 2},
                  {1, 3},
                  {1, 4},
                  {2, 1},
                  {2, 3},
                  {2, 4},
                  {3, 1},
                  {3, 2},
                  {3, 4},
                  {4, 1},
                  {4, 2},
                  {4, 3}});
  EXPECT_EQ(solver.solve(), 0);
}

TEST(KosarajuTest, MultipleEdges) {
  KosarajuSolver solver;
  solver.init(3, {{1, 2}, {1, 2}, {2, 3}, {2, 3}, {3, 1}});
  EXPECT_EQ(solver.solve(), 0);
}
