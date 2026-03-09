add_subdirectory(tasktree)
add_subdirectory(database)

#set sources and options
target_sources(Tasktree PRIVATE 
    main.cpp
    backend.hpp
    backend.cpp
)

# Add QML resources
qt6_add_resources(Tasktree "qml"
    PREFIX "/"
    FILES main.qml
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
