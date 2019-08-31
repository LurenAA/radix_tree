cmake_minimum_required(VERSION 3.0.0)
project(radix_tree VERSION 0.1.0)

include(CTest)
enable_testing()
add_definitions("-Wall -g")
add_definitions("-D RADIX_DEBUG_MSG")
add_library(queue simpleQueue.c)
add_library(radix_lib radix_tree.c)
add_executable(radix_tree main.c)
target_link_libraries(radix_tree radix_lib)
target_link_libraries(radix_tree queue)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
