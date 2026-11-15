#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "task3.h"

using namespace std;

// 1. Тест из условия задачи (простое ребро)
TEST(Task3Test, ExampleFromStatement) {
    int n = 2;
    int m = 1;
    vector<int> arr = {1, 2};
    vector<string> expected = {
        "0",
        "-",
        "1",
        "1 2"
    };
    EXPECT_EQ(task3(n, m, arr), expected);
}

// 2. Идеальное кольцо (треугольник) - нет ни мостов, ни точек сочленения
TEST(Task3Test, TriangleNoCriticals) {
    int n = 3;
    int m = 3;
    vector<int> arr = {1, 2, 2, 3, 3, 1};
    vector<string> expected = {
        "0",
        "-",
        "0",
        "-"
    };
    EXPECT_EQ(task3(n, m, arr), expected);
}

// 3. Звезда (один роутер в центре, к которому подключены остальные)
// Центральный роутер - точка сочленения, все провода - мосты
TEST(Task3Test, StarGraph) {
    int n = 4;
    int m = 3;
    // 1 в центре, 2, 3, 4 - по краям
    vector<int> arr = {1, 2, 1, 3, 1, 4};
    vector<string> expected = {
        "1",
        "1", // роутер 1 - критический
        "3",
        "1 2; 1 3; 1 4" // лексикографический порядок!
    };
    EXPECT_EQ(task3(n, m, arr), expected);
}

// 4. Две компоненты связности (граф изначально разорван)
// Проверяет, что твой цикл for(1 to n) if (disc[i] == 0) работает правильно
TEST(Task3Test, DisconnectedGraph) {
    int n = 4;
    int m = 2;
    vector<int> arr = {1, 2, 3, 4};
    vector<string> expected = {
        "0",
        "-",
        "2",
        "1 2; 3 4"
    };
    EXPECT_EQ(task3(n, m, arr), expected);
}

// 5. Гантеля (два треугольника, соединенные одним мостом 3-4)
TEST(Task3Test, DumbbellGraph) {
    int n = 6;
    int m = 7;
    // Треугольник 1-2-3, треугольник 4-5-6 и мост 3-4
    vector<int> arr = {
        1, 2, 2, 3, 3, 1, // первый треугольник
        4, 5, 5, 6, 6, 4, // второй треугольник
        3, 4              // мост
    };
    vector<string> expected = {
        "2",
        "3 4", // вершины 3 и 4 критические
        "1",
        "3 4"  // ребро 3-4 - единственный мост
    };
    EXPECT_EQ(task3(n, m, arr), expected);
}

// 6. Линейный граф (цепочка роутеров)
// Все внутренние роутеры - точки сочленения, все провода - мосты
TEST(Task3Test, LineGraph) {
    int n = 4;
    int m = 3;
    vector<int> arr = {1, 2, 2, 3, 3, 4};
    vector<string> expected = {
        "2",
        "2 3", // Внутренние вершины
        "3",
        "1 2; 2 3; 3 4"
    };
    EXPECT_EQ(task3(n, m, arr), expected);
}