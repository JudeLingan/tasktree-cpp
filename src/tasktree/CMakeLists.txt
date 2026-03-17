add_library(Task)

target_sources(Task
	PRIVATE
		task.cpp
	PUBLIC
		FILE_SET HEADERS
		FILES
			task.hpp
)

target_link_libraries(Task PRIVATE sqlite3)

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
	PUBLIC sqlite3
	PUBLIC DatabaseUniqueSqlite
)
