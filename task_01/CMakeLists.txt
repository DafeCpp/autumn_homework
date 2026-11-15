cmake_minimum_required(VERSION 3.14)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" PROJECT_NAME ${PROJECT_NAME})
project(${PROJECT_NAME} C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lib ${CMAKE_CURRENT_BINARY_DIR}/lib)

file(GLOB_RECURSE test_source_list CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*test.cpp")

file(GLOB_RECURSE source_list CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

list(REMOVE_ITEM source_list ${test_source_list})

set(main_cpp "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
set(lib_sources ${source_list})
list(REMOVE_ITEM lib_sources "${main_cpp}")

add_executable(${PROJECT_NAME} ${source_list})
target_link_libraries(${PROJECT_NAME} PRIVATE Utils)

enable_testing()
find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME}_tests ${test_source_list} ${lib_sources})
target_link_libraries(${PROJECT_NAME}_tests PRIVATE Utils GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests)