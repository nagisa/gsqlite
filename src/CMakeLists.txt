cmake_minimum_required(VERSION 3.0)
project(gSQLite)

# Flag which tells cmake to output all compilation commands used.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wpedantic -Wno-deprecated-register")
set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -flto")

# tests
enable_testing()
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
macro("test_it" n)
    add_executable(${n} ${n})
    target_link_libraries(${n} ${TESTS_LIBRARIES})
    add_dependencies(check ${n})
    add_test(NAME ${n} COMMAND ${n})
endmacro("test_it")

# documentation

find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
                   ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif(DOXYGEN_FOUND)


# Dependencies
find_package(PkgConfig)
find_library(SQLITE sqlite3 REQUIRED)
find_package(Threads REQUIRED)
pkg_check_modules(GLIBMM glibmm-2.4)
pkg_check_modules(GIOMM giomm-2.4)
pkg_check_modules(GTKMM gtkmm-3.0)


add_subdirectory(sqlitemm)


add_executable(gsqlite gsqlite window table_view value_column query_result_widget)
target_link_libraries(gsqlite ${SQLITEMM_LIBRARIES} ${GTKMM_LIBRARIES})
target_include_directories(gsqlite PRIVATE ${GTKMM_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR})
