#include <chrono>
#include "task.hpp"

using namespace std;
using sys_clock = chrono::system_clock;

namespace tasktree {
	Task::Task(const string& name, time_t creation_time, sqlite3_int64 id, bool completed, Task* parent) noexcept:
		name(name), creation_time(creation_time), id(id), completed(completed), parent(parent) {}

	Task::Task(const string name, sqlite3_int64 id, Task* parent, bool completed) noexcept :
		Task(name, sys_clock::to_time_t(sys_clock::now()), id, completed, parent) {}

	Task::Task() noexcept :
		Task("", sys_clock::to_time_t(sys_clock::now()), 0, false, nullptr) {}

	Task& Task::add_child(Task child) {
		children.push_back(child);
		children.back().parent = this;
		return children.back();
	}

	void Task::remove_child(int i) {
		children.erase(children.begin() + i);
	}

	Task* Task::get_child_by_id(sqlite3_int64 id) {
		for (int i = 0; i < get_child_count(); ++i) {
			if (get_child(i).get_id() == id) {
				return &get_child(i);
			}
		}
		return nullptr;
	}
} // namespace tasktree
