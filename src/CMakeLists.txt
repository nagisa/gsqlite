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

pkg_check_modules(GLIBMM glibmm-2.4)
link_directories(${GLIBMM_LIBRARY_DIRS})
include_directories(${GLIBMM_INCLUDE_DIRS})

set(CORE_LIBRARIES ${SQLITE} gsqlitel ${GLIBMM_LIBRARIES})
set(ALL_LIBRARIES ${CORE_LIBRARIES} gsqliteui ${GTKMM_LIBRARIES})

add_library(gsqlitel STATIC interfaces connection sqlite_common statement row value)
add_library(gsqliteui STATIC window)
add_executable(gsqlite gsqlite.cc)
target_link_libraries(gsqlite ${ALL_LIBRARIES})

# tests
enable_testing()
add_subdirectory(tests)

# documentation

find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif(DOXYGEN_FOUND)
