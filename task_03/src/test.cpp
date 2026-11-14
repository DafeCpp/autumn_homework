#include <gtest/gtest.h>

#include "network.hpp"

TEST(NetworkAnalyzerTest, ExampleFromPrompt) {
  NetworkAnalyzer analyzer(2);
  analyzer.addEdge(1, 2, 0);
  NetworkResult result = analyzer.solve();

  ASSERT_EQ(result.cut_vertices.size(), 0u);
  ASSERT_EQ(result.bridges.size(), 1u);
  EXPECT_EQ(result.bridges[0], std::make_pair(1, 2));
}

TEST(NetworkAnalyzerTest, TriangleNoCutsNoBridges) {
  NetworkAnalyzer analyzer(3);
  analyzer.addEdge(1, 2, 0);
  analyzer.addEdge(2, 3, 1);
  analyzer.addEdge(3, 1, 2);
  NetworkResult result = analyzer.solve();

  ASSERT_EQ(result.cut_vertices.size(), 0u);
  ASSERT_EQ(result.bridges.size(), 0u);
}

TEST(NetworkAnalyzerTest, LineGraph) {
  NetworkAnalyzer analyzer(4);
  analyzer.addEdge(1, 2, 0);
  analyzer.addEdge(2, 3, 1);
  analyzer.addEdge(3, 4, 2);
  NetworkResult result = analyzer.solve();

  ASSERT_EQ(result.cut_vertices.size(), 2u);
  EXPECT_EQ(result.cut_vertices[0], 2);
  EXPECT_EQ(result.cut_vertices[1], 3);

  ASSERT_EQ(result.bridges.size(), 3u);
  EXPECT_EQ(result.bridges[0], std::make_pair(1, 2));
  EXPECT_EQ(result.bridges[1], std::make_pair(2, 3));
  EXPECT_EQ(result.bridges[2], std::make_pair(3, 4));
}

TEST(NetworkAnalyzerTest, DisconnectedGraph) {
  NetworkAnalyzer analyzer(4);
  analyzer.addEdge(1, 2, 0);
  analyzer.addEdge(3, 4, 1);
  NetworkResult result = analyzer.solve();

  ASSERT_EQ(result.cut_vertices.size(), 0u);
  ASSERT_EQ(result.bridges.size(), 2u);
  EXPECT_EQ(result.bridges[0], std::make_pair(1, 2));
  EXPECT_EQ(result.bridges[1], std::make_pair(3, 4));
}

TEST(NetworkAnalyzerTest, GraphWithSelfLoopIgnored) {
  NetworkAnalyzer analyzer(2);
  analyzer.addEdge(1, 1, 0);
  analyzer.addEdge(1, 2, 1);
  NetworkResult result = analyzer.solve();

  ASSERT_EQ(result.cut_vertices.size(), 0u);
  ASSERT_EQ(result.bridges.size(), 1u);
  EXPECT_EQ(result.bridges[0], std::make_pair(1, 2));
}

TEST(NetworkAnalyzerTest, ComplexGraphLexicographicalOrder) {
  NetworkAnalyzer analyzer(5);
  analyzer.addEdge(3, 4, 0);
  analyzer.addEdge(1, 2, 1);
  analyzer.addEdge(2, 3, 2);
  analyzer.addEdge(4, 5, 3);
  NetworkResult result = analyzer.solve();

  ASSERT_EQ(result.bridges.size(), 4u);
  EXPECT_EQ(result.bridges[0], std::make_pair(1, 2));
  EXPECT_EQ(result.bridges[1], std::make_pair(2, 3));
  EXPECT_EQ(result.bridges[2], std::make_pair(3, 4));
  EXPECT_EQ(result.bridges[3], std::make_pair(4, 5));
}

TEST(NetworkAnalyzerTest, BridgeReverseOrderNormalization) {
  NetworkAnalyzer analyzer(3);
  analyzer.addEdge(3, 1, 0);
  NetworkResult result = analyzer.solve();

  ASSERT_EQ(result.bridges.size(), 1u);
  EXPECT_EQ(result.bridges[0], std::make_pair(1, 3));
}

TEST(NetworkAnalyzerTest, StarGraphArticulationPoint) {
  NetworkAnalyzer analyzer(4);
  analyzer.addEdge(1, 2, 0);
  analyzer.addEdge(1, 3, 1);
  analyzer.addEdge(1, 4, 2);
  NetworkResult result = analyzer.solve();

  ASSERT_EQ(result.cut_vertices.size(), 1u);
  EXPECT_EQ(result.cut_vertices[0], 1);

  ASSERT_EQ(result.bridges.size(), 3u);
}
