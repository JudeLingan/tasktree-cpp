add_library(Task)

target_sources(Task
	PRIVATE
		task.cpp
	PUBLIC
		FILE_SET HEADERS
		FILES
			task.hpp
)

add_library(Tasktreelib)

target_sources(Tasktreelib
	PRIVATE
		tasktree.cpp
	PUBLIC
		FILE_SET HEADERS
		FILES
			tasktree.hpp
)

target_link_libraries(Tasktreelib
	PRIVATE Task
	PRIVATE DatabaseUniqueSqlite
)
