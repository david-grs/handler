cmake_minimum_required(VERSION 3.1)

set(cxx_compile_options "-g -Wall -Wextra -Wpedantic -Wconversion -Wswitch-enum -Wunreachable-code -Wwrite-strings -Wcast-align -Wshadow -Wundef -Wold-style-cast -Wshadow -Wdouble-promotion")

# CMake < 3.8 does not support CXX17
if(${CMAKE_VERSION} VERSION_LESS "3.8.0")
	if (MSVC)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 ${cxx_compile_options}")
	endif()
else()
	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${cxx_compile_options}")
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(gtest.cmake)

add_executable(handler handler.cc)
target_link_libraries(handler gtest pthread)

