cmake_minimum_required (VERSION 3.6)
project (logSearcher)

set(CMAKE_C_COMPILER "g++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-W -Wall -Wextra -O3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS}")

find_package(Threads REQUIRED)
find_package(benchmark REQUIRED)

include_directories(src/ src/sys/)

add_library(searcher 
        src/cli.cpp
        src/commands.cpp
        src/logblock.cpp
        src/logfile.cpp
        src/logtime.cpp
        src/ordered-logfile.cpp
        src/unordered-logfile.cpp)

add_executable(logsearcher src/main.cpp)
target_link_libraries(logsearcher searcher)

add_executable(bench
        src/bench.cpp
        src/cli.cpp
        src/commands.cpp
        src/logblock.cpp
        src/logfile.cpp
        src/logtime.cpp
        src/ordered-logfile.cpp
        src/unordered-logfile.cpp
        )

target_link_libraries(bench benchmark::benchmark)
