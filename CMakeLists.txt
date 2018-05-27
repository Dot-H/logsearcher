cmake_minimum_required (VERSION 3.6)
project (logSearcher)

set(CMAKE_C_COMPILER "g++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-W -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

include_directories(src/ src/sys/)

add_executable(logsearcher
        src/logblock.cpp
        src/cli.cpp
        src/logfile.cpp
        src/logtime.cpp
        src/main.cpp)

add_executable(logfile-test
        src/logfile.cpp
        tests/logfile.cpp)

add_executable(logtime-test
        src/logtime.cpp
        tests/logtime.cpp)
