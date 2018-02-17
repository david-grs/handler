cmake_minimum_required(VERSION 2.8)

add_definitions("-std=c++1z")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_definitions("-Weverything -Wno-c++98-compat")
endif()

include(gtest.cmake)

add_executable(handler handler.cc)
target_link_libraries(handler gtest pthread)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_link_libraries(handler stdc++)
endif()
