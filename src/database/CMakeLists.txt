add_library(DatabaseUniqueSqlite)

target_sources(DatabaseUniqueSqlite
	PRIVATE
		unique-sqlite.cpp
	PUBLIC
		FILE_SET HEADERS
		FILES
			unique-sqlite.hpp
)

target_link_libraries(DatabaseUniqueSqlite
	PUBLIC sqlite3
)
