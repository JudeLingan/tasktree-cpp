#include <chrono>
#include <stdexcept>
#include <tasktree.hpp>

using namespace std;
using sys_clock = chrono::system_clock;

namespace tasktree
{
	Task::Task(const string& name, time_t creation_time, sqlite3_int64 id, Task* parent) noexcept
	{
		this->name = name;
		this->creation_time = creation_time;
		this->id = id;
		this->parent = parent;
	}

	Task::Task(const string& name, sqlite3_int64 id, Task* parent) noexcept :
		Task(name, sys_clock::to_time_t(sys_clock::now()), id, parent) {}

	TaskTree::TaskTree(const char* path) :
		head("", 0, nullptr),
		db(nullptr, &sqlite3_close)
	{
		sqlite3* raw_db = nullptr;
		int rc = sqlite3_open(path, &raw_db);
		if (rc == SQLITE_OK && raw_db != nullptr) {
			db.reset(raw_db);
		}
		else {
			throw runtime_error("Failed to open sqlite database");
		}
	}
}
