cmake_minimum_required(VERSION 3.0)
project(gSQLite)

# Flag which tells cmake to output all compilation commands used.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -flto")

find_library(SQLITE sqlite3 REQUIRED)

add_library(libgsqlite gsqlite.cc interfaces.h)
add_executable(gsqlite gsqlite.cc)
target_link_libraries(gsqlite ${SQLITE} libgsqlite)

# tests
enable_testing()
add_subdirectory(tests)

