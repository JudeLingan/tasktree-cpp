cmake_minimum_required(VERSION 3.23)
project(Tasktree CXX C)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Qml Quick REQUIRED)
add_library(sqlite3 STATIC third-party/sqlite/sqlite3.c)
target_include_directories(sqlite3 PUBLIC third-party/sqlite)

qt_standard_project_setup(REQUIRES 6.5)

# adding executables
qt_add_executable(Tasktree)
add_executable(cli)

#add_executable(Test)

add_subdirectory(src)
