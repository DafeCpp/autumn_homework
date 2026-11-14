cmake_minimum_required(VERSION 3.10)

# Динамически получаем имя текущей папки (например, task_01 или task_02)
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" PROJECT_NAME ${PROJECT_NAME})
project(${PROJECT_NAME} C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. ИЩЕМ ФАЙЛЫ
file(GLOB_RECURSE source_list "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
file(GLOB test_source_list "${CMAKE_CURRENT_SOURCE_DIR}/src/*test.cpp")

# 2. Исключаем файлы тестов из основной программы
list(REMOVE_ITEM source_list ${test_source_list})

# 3. СОЗДАЕМ ОСНОВНУЮ ПРОГРАММУ (имя будет task_01 или task_02)
add_executable(${PROJECT_NAME} ${source_list})
target_include_directories(${PROJECT_NAME} PUBLIC src)

# 4. НАСТРОЙКА ТЕСТОВ (Google Test)
enable_testing()
find_package(GTest REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC Utils)

# Исключаем main.cpp, чтобы он не конфликтовал с тестами
set(test_dependencies ${source_list})
list(REMOVE_ITEM test_dependencies "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp") 

# 5. СОЗДАЕМ ИСПОЛНЯЕМЫЙ ФАЙЛ ТЕСТОВ (имя будет task_01_tests или task_02_tests)
add_executable(${PROJECT_NAME}_tests ${test_source_list} ${test_dependencies})

target_link_libraries(
  ${PROJECT_NAME}_tests
  GTest::gtest_main
  Utils
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests)