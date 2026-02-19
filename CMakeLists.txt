cmake_minimum_required(VERSION 3.23)
project(Tasktree CXX)

find_package(SQLite3 REQUIRED)

# adding executables
add_executable(Tasktree)

#add_executable(Test)

add_subdirectory(src)
