cmake_minimum_required(VERSION 3.0)
project(gSQLite_tests)

add_executable(test1 test1)
target_link_libraries(test1 ${SQLITE} gsqlitel)
add_test(NAME "TestsWorking" COMMAND test1)

add_executable(connection1 connection1)
target_link_libraries(connection1 ${SQLITE} gsqlitel)
add_test(NAME "Connection1" COMMAND connection1)

add_executable(connection2 connection2)
target_link_libraries(connection2 ${SQLITE} gsqlitel)
add_test(NAME "Connection2" COMMAND connection2)
