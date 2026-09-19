cmake_minimum_required(VERSION 3.10)
project(bfs_visualization LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Reuse Utils in the full project and also support standalone example builds.
if(NOT TARGET Utils)
    add_subdirectory(../../lib "${CMAKE_CURRENT_BINARY_DIR}/lib")
endif()

add_library(bfs_algorithm src/bfs.cpp)
target_include_directories(bfs_algorithm PUBLIC src)
target_link_libraries(bfs_algorithm PUBLIC Utils)

add_executable(bfs_visualization src/main.cpp)
target_link_libraries(bfs_visualization PRIVATE bfs_algorithm)
add_executable(bfs_plain src/plain.cpp)
target_link_libraries(bfs_plain PRIVATE bfs_algorithm)

enable_testing()
find_package(GTest REQUIRED)
add_executable(bfs_visualization_tests src/test.cpp)
target_link_libraries(bfs_visualization_tests PRIVATE bfs_algorithm GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(bfs_visualization_tests)
