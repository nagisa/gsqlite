cmake_minimum_required(VERSION 3.0)
project(gSQLite)

# Flag which tells cmake to output all compilation commands used.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wpedantic")
set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -flto")

find_library(SQLITE sqlite3 REQUIRED)

add_library(gsqlitel STATIC gsqlite interfaces connection sqlite_common)
add_executable(gsqlite gsqlite.cc)
target_link_libraries(gsqlite ${SQLITE} gsqlitel)

# tests
enable_testing()
add_subdirectory(tests)

