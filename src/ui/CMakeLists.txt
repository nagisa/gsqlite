cmake_minimum_required(VERSION 3.0)
project(gSQLite_ui)

find_package(PkgConfig)
find_library(SQLITE sqlite3 REQUIRED)

add_library(gsqliteui STATIC window)
