#include <gtest/gtest.h>

#include <algorithm>
#include <graphs.hpp>
#include <vector>

namespace {

struct Edge {
  int from;
  int to;
};

bool IsValidTopologicalOrder(const std::vector<int>& order,
                             const std::vector<Edge>& edges) {
  if (order.empty()) return true;

  std::vector<int> position(order.size());
  for (size_t i = 0; i < order.size(); ++i) {
    position[order[i]] = static_cast<int>(i);
  }

  for (const auto& edge : edges) {
    if (position[edge.from] > position[edge.to]) {
      return false;
    }
  }

  return true;
}

TEST(GraphTest, SimpleGraph) {
  Graph g(3);
  g.AddEdge(0, 1);
  g.AddEdge(1, 2);

  auto order = g.TopologicalSort();
  ASSERT_TRUE(order.has_value());
  ASSERT_EQ(order->size(), 3u);

  std::vector<Edge> edges = {{0, 1}, {1, 2}};
  EXPECT_TRUE(IsValidTopologicalOrder(*order, edges));
}

TEST(GraphTest, GraphWithCycle) {
  Graph g(4);
  g.AddEdge(0, 1);
  g.AddEdge(1, 2);
  g.AddEdge(2, 0);
  g.AddEdge(3, 1);

  auto order = g.TopologicalSort();
  EXPECT_FALSE(order.has_value());
}

TEST(GraphTest, EmptyGraph) {
  Graph g(5);
  auto order = g.TopologicalSort();

  ASSERT_TRUE(order.has_value());
  ASSERT_EQ(order->size(), 5u);

  std::vector<int> sorted_order = *order;
  std::sort(sorted_order.begin(), sorted_order.end());
  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(sorted_order[i], i);
  }
}

TEST(GraphTest, SingleVertex) {
  Graph g(1);
  auto order = g.TopologicalSort();

  ASSERT_TRUE(order.has_value());
  ASSERT_EQ(order->size(), 1u);
  EXPECT_EQ((*order)[0], 0);
}

TEST(GraphTest, SelfLoop) {
  Graph g(3);
  g.AddEdge(0, 0);

  auto order = g.TopologicalSort();
  EXPECT_FALSE(order.has_value());
}

TEST(GraphTest, MultipleComponents) {
  Graph g(6);
  g.AddEdge(0, 1);
  g.AddEdge(1, 2);
  g.AddEdge(3, 4);

  auto order = g.TopologicalSort();
  ASSERT_TRUE(order.has_value());
  ASSERT_EQ(order->size(), 6u);

  std::vector<Edge> edges = {{0, 1}, {1, 2}, {3, 4}};
  EXPECT_TRUE(IsValidTopologicalOrder(*order, edges));
}

TEST(GraphTest, ComplexDAG) {
  Graph g(7);
  g.AddEdge(0, 1);
  g.AddEdge(0, 2);
  g.AddEdge(1, 3);
  g.AddEdge(2, 3);
  g.AddEdge(3, 4);
  g.AddEdge(3, 5);
  g.AddEdge(4, 6);
  g.AddEdge(5, 6);

  auto order = g.TopologicalSort();
  ASSERT_TRUE(order.has_value());
  ASSERT_EQ(order->size(), 7u);

  std::vector<Edge> edges = {{0, 1}, {0, 2}, {1, 3}, {2, 3},
                             {3, 4}, {3, 5}, {4, 6}, {5, 6}};
  EXPECT_TRUE(IsValidTopologicalOrder(*order, edges));
}

TEST(GraphTest, LargeCycle) {
  int n = 100;
  Graph g(n);
  for (int i = 0; i < n; ++i) {
    g.AddEdge(i, (i + 1) % n);
  }

  auto order = g.TopologicalSort();
  EXPECT_FALSE(order.has_value());
}

TEST(GraphTest, LongChain) {
  int n = 100;
  Graph g(n);
  for (int i = 0; i < n - 1; ++i) {
    g.AddEdge(i, i + 1);
  }

  auto order = g.TopologicalSort();
  ASSERT_TRUE(order.has_value());
  ASSERT_EQ(order->size(), static_cast<size_t>(n));

  for (int i = 0; i < n; ++i) {
    EXPECT_EQ((*order)[i], n - 1 - i);
  }
}

TEST(GraphTest, InvalidEdges) {
  Graph g(3);
  g.AddEdge(0, 1);

  EXPECT_THROW(g.AddEdge(-1, 2), std::out_of_range);
  EXPECT_THROW(g.AddEdge(0, 5), std::out_of_range);

  g.AddEdge(1, 2);
  auto order = g.TopologicalSort();

  ASSERT_TRUE(order.has_value());
  ASSERT_EQ(order->size(), 3u);

  std::vector<Edge> edges = {{0, 1}, {1, 2}};
  EXPECT_TRUE(IsValidTopologicalOrder(*order, edges));
}

}  // namespace

int main(int argc, char** argv) {
  try {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
  } catch (const std::exception& e) {
    std::cerr << "Fatal error: " << e.what() << "\n";
    return 1;
  } catch (...) {
    std::cerr << "Unknown fatal error\n";
    return 1;
  }
}
