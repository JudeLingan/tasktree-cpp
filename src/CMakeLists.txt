add_subdirectory(tasktree)
add_subdirectory(database)
add_subdirectory(ui)

#set sources and options
target_sources(Tasktree
	PRIVATE
		main.cpp
		backend.hpp
		backend.cpp
)

set_target_properties(Tasktree PROPERTIES CXX_STANDARD 20)

#link necessary libs
target_link_libraries(Tasktree
    PRIVATE Tasktreelib
	PRIVATE DatabaseUniqueSqlite
	PRIVATE sqlite3
	PRIVATE Util
	PRIVATE Qt6::Core
	PRIVATE Qt6::Gui
	PRIVATE Qt6::Qml
	PRIVATE Qt6::Quick
	PRIVATE TasktreeUI
)

#setup cli
set_target_properties(Tasktree PROPERTIES
	QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/android"
	CXX_STANDARD 20
)

target_link_libraries(cli
	PRIVATE Util
    PRIVATE Tasktreelib
)

#add util library
add_library(Util)

target_sources(Util
	PRIVATE
		util.cpp
	PUBLIC
		FILE_SET HEADERS
		FILES
			util.hpp
)

target_link_libraries(Util
	PUBLIC Qt6::Core
)

target_sources(cli
	PRIVATE
		cli.cpp
)
