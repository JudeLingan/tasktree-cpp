add_subdirectory(tasktree)
add_subdirectory(database)
add_subdirectory(ui)

#set sources and options
target_sources(Tasktree PRIVATE 
    main.cpp
    backend.hpp
    backend.cpp
)

set_target_properties(Tasktree PROPERTIES CXX_STANDARD 20)

#link necessary libs
target_link_libraries(Tasktree
    PRIVATE Tasktreelib
    PRIVATE DatabaseUniqueSqlite
    PRIVATE Qt6::Core
    PRIVATE Qt6::Gui
    PRIVATE Qt6::Qml
    PRIVATE Qt6::Quick
)
