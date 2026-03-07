cmake_minimum_required(VERSION 3.23)
project(Tasktree CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Qml Quick REQUIRED)
find_package(SQLite3 REQUIRED)

# adding executables
add_executable(Tasktree)

#add_executable(Test)

add_subdirectory(src)
