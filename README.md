# TaskTree C++
This repo is intended to replace my C repo, Tasktree.
It currently has a database connection and QT Quick UI.
An initial release for mobile will be available once the goals are complete.
It also needs a better name because TaskTree is taken.

## What is this for?
I find standard task management programs to be fairly limiting because the way they handle subtasks feels unelegant to me.
The goal is to be able to cleanly have subtasks of subtasks of subtasks of subtasks etc.
On mobile, this will be done by being able to tap on a task to see its subtasks.
Tapping and holding on a task will let you change its name.
When in this subui, there will be a title with the name of the task that is currently "focused" and a "go back" button.

## Initial Goals
- [X] db connection (I know this table layout works because it worked on the C version, which has recursion)
- [X] adding tasks
- [X] deleting tasks
- [X] task creation time property
- [ ] completing tasks
- [X] basic ui
- [ ] subtasks of subtasks of subtasks!

## Building
* Make sure cmake, a c compiler, sqlite, and qt6 are installed
* Run `cmake --preset default` to generate your build configuration
* run `cmake --build build` to make the binary
* there should be a binary at $PWD/build/Tasktree
