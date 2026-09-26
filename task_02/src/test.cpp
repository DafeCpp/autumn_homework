#include <iostream>
#include <string>
#include <vector>
#include "kosaraju.h"

struct TestCase {
    std::string name;
    int n;
    std::vector<Edge> edges;
    int expected;
};

int main() {
    std::vector<TestCase> tests = {
        {"Test 1: Example 1", 2, {{1, 2}}, 1},
        {"Test 2: Example 2", 4, {{1, 2}, {3, 2}, {2, 4}}, 2},
        {"Test 3: Example 3", 5, {{1, 2}, {3, 2}, {2, 4}, {2, 5}}, 2},
        {"Test 4: Already strongly connected", 3, {{1, 2}, {2, 3}, {3, 1}}, 0},
        {"Test 5: Single vertex", 1, {}, 0},
        {"Test 6: Two isolated vertices", 2, {}, 2},
        {"Test 7: Graph with self-loop", 3, {{1, 1}, {1, 2}, {2, 3}, {3, 2}}, 1},
        {"Test 8: Linear chain", 4, {{1, 2}, {2, 3}, {3, 4}}, 3},
        {"Test 9: Two SCCs", 6, {{1, 2}, {2, 1}, {3, 4}, {4, 5}, {5, 3}, {3, 6}}, 2},
        {"Test 10: All isolated", 5, {}, 5},
        {"Test 11: Star graph", 5, {{1, 2}, {1, 3}, {1, 4}, {1, 5}}, 4},
        {"Test 12: Reverse star", 5, {{2, 1}, {3, 1}, {4, 1}, {5, 1}}, 4},
        {"Test 13: Complex case", 7, {{1, 2}, {2, 1}, {2, 3}, {3, 4}, {4, 3}, {4, 5}, {5, 6}, {6, 7}}, 3},
        {"Test 14: Complete graph", 4, {{1, 2}, {1, 3}, {1, 4}, {2, 1}, {2, 3}, {2, 4}, {3, 1}, {3, 2}, {3, 4}, {4, 1}, {4, 2}, {4, 3}}, 0},
        {"Test 15: Multiple edges", 3, {{1, 2}, {1, 2}, {2, 3}, {2, 3}, {3, 1}}, 0}
    };

    int passed = 0;
    int total = tests.size();

    std::cout << "Running tests...\n\n";

    for (int i = 0; i < total; ++i) {
        const auto& test = tests[i];
        std::cout << test.name << "\n";

        KosarajuSolver solver;
        solver.init(test.n, test.edges);
        int result = solver.solve();

        if (result == test.expected) {
            std::cout << "  PASSED (answer: " << result << ")\n";
            passed++;
        } else {
            std::cout << "  FAILED (expected: " << test.expected
                      << ", got: " << result << ")\n";
        }
        std::cout << "\n";
    }

    std::cout << "========================================\n";
    std::cout << "Result: " << passed << "/" << total << " tests passed\n";

    if (passed == total) {
        std::cout << "All tests passed!\n";
        return 0;
    } else {
        std::cout << "Some tests failed.\n";
        return 1;
    }
}
