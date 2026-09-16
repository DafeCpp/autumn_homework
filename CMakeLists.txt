cmake_minimum_required(VERSION 3.10)

project(homeworks)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sandbox)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/additional_tasks)

# Only top-level task sources belong to the build. Generated recordings and
# nested build directories may also contain folders named task_03, etc.
file(GLOB tasks_dirs LIST_DIRECTORIES true "${CMAKE_CURRENT_SOURCE_DIR}/task_0[0-9]")
foreach(dir IN LISTS tasks_dirs)
    if(IS_DIRECTORY "${dir}")
        add_subdirectory("${dir}")
    endif()
endforeach()

add_subdirectory(examples/bfs_visualization)
add_subdirectory(examples/kosaraju_visualization)
add_subdirectory(examples/tarjan_visualization)
