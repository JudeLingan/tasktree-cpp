#pragma once

#include <string>
#include <unordered_map>
#include <sqlite3.h>
#include "unique-sqlite.hpp"
#include "task.hpp"

namespace tasktree {
	class TaskTree {
		private:
			// --- private variables ---

			Task head = Task();
			database::UniqueSqlite db;        
			std::unordered_map<std::string, sqlite3_int64> column_ids = {}; //contains pairs of columns and int ids
			std::vector<std::string> column_names = {"id", "parent", "name", "creation time"}; //

			// --- tree functions ---

			//load all of a task's children into memory
			void load_child_tasks(Task& parent);

			// --- database functions ---

			//get column id or return -1 if it doesn't exist
			sqlite3_int64 column_id(std::string name) noexcept { return column_ids.count(name) ? column_ids.at(name) : -1; }

			//prints a vector string containing all column names
			std::vector<std::string> get_column_names() const noexcept;

		public:

			// --- public interface ---

			//initialize tasktree and load database at path
			explicit TaskTree(const std::string& path);

			//get head task
			Task& get_head() noexcept { return head; }

			//add child with name to parent, updating db
			Task& add_child(Task& parent, const std::string& name);

			//remove task from database and memory
			void remove(Task& task);

			//change name of tsk
			void set_task_name(Task& task, const std::string& name);

			//mark task as complete in database
			void complete(Task&);

			//print all child tasks
			void print();

			//get first task loaded into memory with this id
			//returns null if not found
			Task* get_by_id(sqlite3_int64 id);
	};
}
