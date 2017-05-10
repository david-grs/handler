cmake_minimum_required(VERSION 2.8)

add_definitions("-std=c++1z")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_definitions("-Weverything -Wno-c++98-compat")
endif()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(subscription subscription.cc)
target_link_libraries(subscription ${GTEST_BOTH_LIBRARIES} pthread)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_link_libraries(subscription stdc++)
endif()
