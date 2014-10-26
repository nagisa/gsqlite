cmake_minimum_required(VERSION 3.0)
project(sqlitemm_tests)

set(TESTS_LIBRARIES ${SQLITEMM_LIBRARIES})

test_it(testing)
test_it(connection1)
test_it(connection2)
test_it(connection_prepare1)
test_it(connection_prepare2)
test_it(row1)
test_it(row2)
test_it(row3)
