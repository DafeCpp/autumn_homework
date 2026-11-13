
cmake_minimum_required(VERSION 3.10)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" PROJECT_NAME ${PROJECT_NAME})
project(${PROJECT_NAME} C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE source_list "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
file(GLOB test_source_list "${CMAKE_CURRENT_SOURCE_DIR}/src/*test.cpp")

list(REMOVE_ITEM source_list ${test_source_list})

include_directories(${PROJECT_NAME} PUBLIC src)

list(REMOVE_ITEM source_list "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
add_library(${PROJECT_NAME}_algorithms ${source_list})
target_include_directories(${PROJECT_NAME}_algorithms PUBLIC src)
target_link_libraries(${PROJECT_NAME}_algorithms PUBLIC Utils)
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_algorithms)

# Locate GTest
enable_testing()
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC Utils)

# Link runTests with what we want to test and the GTest and pthread library
add_executable(${PROJECT_NAME}_tests ${test_source_list})
target_link_libraries(
  ${PROJECT_NAME}_tests
  GTest::gtest_main
  ${PROJECT_NAME}_algorithms
  Utils
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests)
