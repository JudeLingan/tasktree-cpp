add_library(Tasktreelib)

target_sources(Tasktreelib
	PRIVATE
		tasktree.cpp
	PUBLIC
		FILE_SET HEADERS
		FILES
			tasktree.hpp
)
