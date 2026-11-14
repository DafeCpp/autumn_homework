cmake_minimum_required(VERSION 3.10)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" PROJECT_NAME ${PROJECT_NAME})
project(${PROJECT_NAME} C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE all_source_list "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
file(GLOB test_source_list "${CMAKE_CURRENT_SOURCE_DIR}/src/*test.cpp")

list(REMOVE_ITEM all_source_list ${test_source_list})

set(main_file "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
list(REMOVE_ITEM all_source_list ${main_file})

add_library(${PROJECT_NAME}_lib STATIC ${all_source_list})
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# target_link_libraries(${PROJECT_NAME}_lib PUBLIC Utils)

add_executable(${PROJECT_NAME} ${main_file})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)
# target_link_libraries(${PROJECT_NAME} PRIVATE Utils)

enable_testing()
find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME}_tests ${test_source_list})

target_include_directories(${PROJECT_NAME}_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME}_tests PRIVATE 
    ${PROJECT_NAME}_lib    
    GTest::gtest_main      

)

target_compile_definitions(${PROJECT_NAME}_tests PRIVATE TASK_04_TESTING)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests)
