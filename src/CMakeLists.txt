add_subdirectory(tasktree)

#set sources and options
target_sources(Tasktree PRIVATE main.cpp)
set_target_properties(Tasktree PROPERTIES CXX_STANDARD 23)

#link necessary libs
target_link_libraries(Tasktree
	PRIVATE unofficial::sqlite3::sqlite3
	PRIVATE Tasktreelib
		
)
