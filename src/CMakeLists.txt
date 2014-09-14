cmake_minimum_required(VERSION 3.0)
project(gSQLite)

# Flag which tells cmake to output all compilation commands used.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wpedantic -Wno-deprecated-register")
set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -flto")

find_package(PkgConfig)
find_library(SQLITE sqlite3 REQUIRED)

pkg_check_modules(GTKMM gtkmm-3.0)
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

set(ALL_LIBRARIES ${SQLITE} gsqlitel ${GTKMM_LIBRARIES})

add_library(gsqlitel STATIC gsqlite interfaces connection sqlite_common statement row)
add_executable(gsqlite gsqlite.cc)
target_link_libraries(gsqlite ${ALL_LIBRARIES})

# tests
enable_testing()
add_subdirectory(tests)

