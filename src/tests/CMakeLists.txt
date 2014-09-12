cmake_minimum_required(VERSION 3.0)
project(gSQLite_tests)

macro("test_it" n)
    add_executable(${n} ${n})
    target_link_libraries(${n} ${ALL_LIBRARIES})
    add_test(NAME ${n} COMMAND ${n})
endmacro("test_it")

test_it(connection1)
test_it(connection2)
