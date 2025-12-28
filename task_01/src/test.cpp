#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <vector>

#include "find_bridges.hpp"
#include "find_critical_points.hpp"
#include "graph.hpp"

std::string RunNetworkCriticalTest(const std::string& input) {
  std::istringstream in(input);
  std::ostringstream out;

  int n, m;
  in >> n >> m;

  Graph<int> graph(false);
  for (int i = 0; i < n; i++) graph.AddVertex(i + 1);

  for (int i = 0; i < m; i++) {
    int u, v;
    in >> u >> v;
    graph.AddEdge(u, v);
  }

  auto joints = FindJointVertices(graph);
  auto bridges = FindBridges(graph);

  out << joints.size() << "\n";
  if (!joints.empty()) {
    for (size_t i = 0; i < joints.size(); i++) {
      if (i > 0) out << " ";
      out << joints[i];
    }
  } else {
    out << "-";
  }
  out << "\n";

  out << bridges.size() << "\n";
  if (!bridges.empty()) {
    for (size_t i = 0; i < bridges.size(); i++) {
      if (i > 0) out << "; ";
      out << bridges[i].v1 << " " << bridges[i].v2;
    }
  } else {
    out << "-";
  }
  out << "\n";

  return out.str();
}

// ------------------ Тесты ------------------

TEST(TestNetworkCritical, MinimalGraph) {
  std::string input = "2 1\n1 2\n";
  std::string expected = "0\n-\n1\n1 2\n";
  ASSERT_EQ(RunNetworkCriticalTest(input), expected);
}

TEST(TestNetworkCritical, LinearGraph) {
  std::string input = "4 3\n1 2\n2 3\n3 4\n";
  std::string expected = "2\n2 3\n3\n1 2; 2 3; 3 4\n";
  ASSERT_EQ(RunNetworkCriticalTest(input), expected);
}

TEST(TestNetworkCritical, StarGraph) {
  std::string input = "4 3\n1 2\n1 3\n1 4\n";
  std::string expected = "1\n1\n3\n1 2; 1 3; 1 4\n";
  ASSERT_EQ(RunNetworkCriticalTest(input), expected);
}

TEST(TestNetworkCritical, CycleGraph) {
  std::string input = "4 4\n1 2\n2 3\n3 4\n4 1\n";
  std::string expected = "0\n-\n0\n-\n";
  ASSERT_EQ(RunNetworkCriticalTest(input), expected);
}

TEST(TestNetworkCritical, ComplexGraph) {
  std::string input =
      "10 12\n0 1\n0 2\n1 2\n1 3\n2 4\n3 4\n3 5\n4 6\n5 7\n6 8\n7 8\n8 9\n";
  std::string expected = "1\n8\n1\n8 9\n";
  ASSERT_EQ(RunNetworkCriticalTest(input), expected);
}

TEST(TestNetworkCritical, EmptyGraph) {
  std::string input = "0 0\n";
  std::string expected = "0\n-\n0\n-\n";
  ASSERT_EQ(RunNetworkCriticalTest(input), expected);
}

TEST(TestNetworkCritical, SingleVertex) {
  std::string input = "1 0\n";
  std::string expected = "0\n-\n0\n-\n";
  ASSERT_EQ(RunNetworkCriticalTest(input), expected);
}

TEST(TestNetworkCritical, LongLinearGraph) {
  std::string input = "10 9\n1 2\n2 3\n3 4\n4 5\n5 6\n6 7\n7 8\n8 9\n9 10\n";
  std::string expected =
      "8\n2 3 4 5 6 7 8 9\n9\n1 2; 2 3; 3 4; 4 5; 5 6; 6 7; 7 8; 8 9; 9 10\n";
  ASSERT_EQ(RunNetworkCriticalTest(input), expected);
}

TEST(TestNetworkCritical, UndoundGraph) {
  std::string input = "5 3\n1 2\n2 3\n4 5\n";
  std::string expected = "1\n2\n3\n1 2; 2 3; 4 5\n";
  ASSERT_EQ(RunNetworkCriticalTest(input), expected);
}
