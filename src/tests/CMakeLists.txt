cmake_minimum_required(VERSION 3.0)
project(gSQLite_tests)

add_executable(test1 test1.cc)
target_link_libraries(test1 ${SQLITE} libgsqlite)

add_test(NAME "TestsWorking" COMMAND test1)
