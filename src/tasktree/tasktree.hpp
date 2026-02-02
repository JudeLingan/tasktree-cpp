#pragma once

#include <memory>
#include <sqlite3.h>
#include <string>
#include <vector>

namespace tasktree {
	class TaskTree;

	class Task {
		private:
			friend class TaskTree;

			std::string name;
			time_t creation_time;
			sqlite3_int64 id;
			Task* parent;
			std::vector<std::unique_ptr<Task>> children;

			//constructor that sets all vars to their respective values
			Task(const std::string& name, time_t creation_time, sqlite3_int64 id, Task* parent);

			//changes name variable
			void set_name(const std::string& name);

			//changes parent variable
			void set_parent(Task* parent);

			//adds a child task and returns a reference to it
			Task& add_child(Task child);

			//removes a child task
			void remove_child(int i);

		public:

			//returns name
			std::string get_name() const noexcept { return name; }

			//returns id
			sqlite3_int64 get_id() const noexcept { return id; }

			//returns creation_time
			time_t get_creation_time() const noexcept { return creation_time; }

			//returns number of children
			int get_child_count() const { return children.size(); }

			//returns reference to child at index i
			const Task& get_child(int i) const;
	};

	class TaskTree {
		private:
			std::unique_ptr<sqlite3, decltype(&sqlite3_close)> db;
			Task head;

		public:
			explicit TaskTree(const char* path);

			//add child with name to parent, updating db
			const Task& add_child(Task& parent, const std::string& name);

			//get head task
			const Task& get_head();

			//remove task from database and memory
			void remove(Task& task);
	};
}
