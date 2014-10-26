cmake_minimum_required(VERSION 3.0)
project(sqlitemm)


# set(Boost_USE_STATIC_LIBS ON)
# find_package(Boost COMPONENTS system thread REQUIRED)
# link_directories(${Boost_LIBRARY_DIR})
# include_directories(${Boost_INCLUDE_DIR})

link_directories(${GLIBMM_LIBRARY_DIRS})
include_directories(${GLIBMM_INCLUDE_DIRS})

link_directories(${GIOMM_LIBRARY_DIRS})
include_directories(${GIOMM_INCLUDE_DIRS})

set(SQLITEMM_LIBRARIES sqlitemm ${SQLITE} ${GLIBMM_LIBRARIES}
                                ${GIOMM_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
set(SQLITEMM_LIBRARIES ${SQLITEMM_LIBRARIES} PARENT_SCOPE)

add_library(sqlitemm STATIC error connection statement row value private/sigc_fix.hh)

add_subdirectory(tests)
