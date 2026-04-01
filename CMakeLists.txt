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

add_subdirectory(src)

# add google test as dependency
include(FetchContent)

FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG 52eb8108c5bdec04579160ae17225d66034bd723 # v1.7.0
)

# fixes windows problem apparently
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# add test executable
enable_testing()

add_executable(tests)

target_sources(tests
	PRIVATE test/test.cpp
)

target_link_libraries(
	tests
	PRIVATE GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)

add_subdirectory(test)
