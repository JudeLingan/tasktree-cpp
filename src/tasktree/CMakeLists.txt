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
	PRIVATE unofficial::sqlite3::sqlite3
)
