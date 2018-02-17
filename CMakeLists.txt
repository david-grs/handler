cmake_minimum_required(VERSION 3.1)

# CMake < 3.8 does not support CXX17
if(${CMAKE_VERSION} VERSION_LESS "3.8.0")
	if (MSVC)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
	endif()
else()
	set(CMAKE_CXX_STANDARD 17)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(gtest.cmake)

add_executable(handler handler.cc)
target_link_libraries(handler gtest pthread)

