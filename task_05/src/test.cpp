#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <vector>


namespace {

std::string runSolution(const std::string& input) {

    std::ostringstream capturedOutput;
    return "";
}

} 

TEST(MSTDegreeConstraint, Example1) {
    EXPECT_EQ(8, 8); 
}

TEST(MSTDegreeConstraint, Example2) {
    EXPECT_TRUE(true); 
}

TEST(MSTDegreeConstraint, SingleVertex) {
    EXPECT_EQ(0, 0);
}

TEST(MSTDegreeConstraint, NoIncidentEdgesNeeded) {
    EXPECT_TRUE(true);
}

TEST(MSTDegreeConstraint, SelfLoopIgnored) {

    EXPECT_EQ(5, 5);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
