#include <gtest/gtest.h>
#include "../../lib/src/util.hpp"
#include <vector>
#include <utility>

TEST(NetworkVulnerabilities, ExampleFromStatement) {
    int n = 2;
    std::vector<std::pair<int, int>> edges = {{1, 2}};
    auto res = find_vulnerabilities(n, edges);
    
    EXPECT_TRUE(res.critical_routers.empty());
    ASSERT_EQ(res.critical_cables.size(), 1);
    EXPECT_EQ(res.critical_cables[0], std::make_pair(1, 2));
}

TEST(NetworkVulnerabilities, NoBridgesNoArticulation) {
    int n = 3;
    std::vector<std::pair<int, int>> edges = {{1, 2}, {2, 3}, {3, 1}};
    auto res = find_vulnerabilities(n, edges);
    
    EXPECT_TRUE(res.critical_routers.empty());
    EXPECT_TRUE(res.critical_cables.empty());
}

TEST(NetworkVulnerabilities, PathGraph) {
    int n = 4;
    std::vector<std::pair<int, int>> edges = {{1, 2}, {2, 3}, {3, 4}};
    auto res = find_vulnerabilities(n, edges);
    
    std::vector<int> expected_art = {2, 3};
    EXPECT_EQ(res.critical_routers, expected_art);
    
    std::vector<std::pair<int, int>> expected_bridges = {{1, 2}, {2, 3}, {3, 4}};
    EXPECT_EQ(res.critical_cables, expected_bridges);
}

TEST(NetworkVulnerabilities, DisconnectedGraph) {
    int n = 4;
    std::vector<std::pair<int, int>> edges = {{1, 2}, {3, 4}};
    auto res = find_vulnerabilities(n, edges);
    
    EXPECT_TRUE(res.critical_routers.empty());
    std::vector<std::pair<int, int>> expected_bridges = {{1, 2}, {3, 4}};
    EXPECT_EQ(res.critical_cables, expected_bridges);
}

TEST(NetworkVulnerabilities, StarGraph) {
    int n = 4;
    std::vector<std::pair<int, int>> edges = {{1, 2}, {1, 3}, {1, 4}};
    auto res = find_vulnerabilities(n, edges);
    
    std::vector<int> expected_art = {1};
    EXPECT_EQ(res.critical_routers, expected_art);
    
    std::vector<std::pair<int, int>> expected_bridges = {{1, 2}, {1, 3}, {1, 4}};
    EXPECT_EQ(res.critical_cables, expected_bridges);
}

TEST(NetworkVulnerabilities, MultiEdges) {
    int n = 2;
    std::vector<std::pair<int, int>> edges = {{1, 2}, {1, 2}};
    auto res = find_vulnerabilities(n, edges);
    
    EXPECT_TRUE(res.critical_routers.empty());
    EXPECT_TRUE(res.critical_cables.empty());
}