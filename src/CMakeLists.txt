add_subdirectory(tasktree)
add_subdirectory(database)

#set sources and options
target_sources(Tasktree PRIVATE main.cpp)
set_target_properties(Tasktree PROPERTIES CXX_STANDARD 20)

#link necessary libs
target_link_libraries(Tasktree
	PRIVATE Tasktreelib
	PRIVATE DatabaseUniqueSqlite
)
