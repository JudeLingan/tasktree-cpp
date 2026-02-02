cmake_minimum_required(VERSION 3.23)
project(Tasktree CXX)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
#find_package(unofficial-cunit CONFIG REQUIRED)

# adding executables
add_executable(Tasktree)

#add_executable(Test)

add_subdirectory(src)
