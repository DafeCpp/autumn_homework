#include <gtest/gtest.h>
#include "kosaraju.hpp" // Обновлён путь

TEST(KosarajuTest, Example1) {
  KosarajuSolver solver;
  solver.init(2, {{1, 2}});
  EXPECT_EQ(solver.solve(), 1);
}

// ... (остальные тесты без изменений) ...

TEST(KosarajuTest, MultipleSCCs) {
  KosarajuSolver solver;
  // Граф имеет 3 компоненты: {1,2}, {3,4,5} и изолированная {6}
  // Требуется добавить 2 ребра для связности компонент
  solver.init(6, {{1, 2}, {2, 1}, {3, 4}, {4, 5}, {5, 3}, {3, 6}});
  EXPECT_EQ(solver.solve(), 2);
}

// ... (остальные тесты без изменений) ...
