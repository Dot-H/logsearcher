cmake_minimum_required (VERSION 3.6)
project (logSearcher)

set(CMAKE_C_COMPILER "g++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-W -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

include_directories(src/)

add_executable(logsearcher src/main.cpp
        src/cli.cpp)
