cmake_minimum_required (VERSION 3.6)
project (logSearcher)

set(CMAKE_C_COMPILER "g++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-W -Wall -Wextra -O3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS}")

include_directories(src/ src/sys/)

add_executable(logsearcher
        src/cli.cpp
        src/logblock.cpp
        src/logfile.cpp
        src/logtime.cpp
        src/main.cpp
        src/ordered-logfile.cpp
        src/unordered-logfile.cpp)

add_executable(logfile-test
        src/logblock.cpp
        src/logfile.cpp
        src/logtime.cpp
        src/ordered-logfile.cpp
        src/unordered-logfile.cpp
        tests/logfile.cpp)

add_executable(logtime-test
        src/logtime.cpp
        tests/logtime.cpp)
