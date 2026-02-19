#include <chrono>
#include "task.hpp"

using namespace std;
using sys_clock = chrono::system_clock;

namespace tasktree {
	Task::Task(const string& name, time_t creation_time, sqlite3_int64 id, Task* parent) noexcept {
		this->name = name;
		this->creation_time = creation_time;
		this->id = id;
		this->parent = parent;
	}

	Task::Task(const string name, sqlite3_int64 id, Task* parent) noexcept :
		Task(name, sys_clock::to_time_t(sys_clock::now()), id, parent) {}

	Task::Task() noexcept :
		Task("", sys_clock::to_time_t(sys_clock::now()), 0, nullptr) {}

	Task& Task::add_child(Task child) {
		children.push_back(child);
		return children.at(children.size() - 1);
	}

	void Task::remove_child(int i) {
		children.erase(children.begin() + i);
	}
}
using sys_clock = chrono::system_clock;
