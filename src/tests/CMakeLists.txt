cmake_minimum_required(VERSION 3.0)
project(gSQLite_tests)

# Our own testing target
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

macro("test_it" n)
    add_executable(${n} ${n})
    target_link_libraries(${n} ${CORE_LIBRARIES})
    add_dependencies(check ${n})
    add_test(NAME ${n} COMMAND ${n})
endmacro("test_it")

test_it(testing)
test_it(connection1)
test_it(connection2)
test_it(connection_prepare1)
test_it(connection_prepare2)
test_it(row1)
test_it(row2)
test_it(row3)
