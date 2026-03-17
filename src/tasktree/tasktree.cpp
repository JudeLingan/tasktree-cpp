#include <chrono>
#include <iostream>
#include <stdexcept>
#include <sqlite3.h>
#include "tasktree.hpp"
#include "unique-sqlite.hpp"

using namespace std;
using namespace database;


namespace tasktree {

#define THROW_SQL_ERROR throw runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ":" + std::string(sqlite3_errmsg(db.get())));

	// --- constructors ---

	TaskTree::TaskTree(const string& path) : db(path) {
		//create table if not exists
		const string sql_tablegen = "CREATE TABLE IF NOT EXISTS tasks (\
									 id INTEGER PRIMARY KEY NOT NULL CHECK (id != 0),\
									 parent INTEGER NOT NULL, creation_time INTEGER NOT NULL,\
									 completed INTEGER NOT NULL DEFAULT 0,\
									 name TEXT)";

		db.exec(sql_tablegen, nullptr, nullptr);

		//load all column names and ids
		UniqueSqliteStmt stmt(db.get(), "PRAGMA table_info(tasks)");
		int rc = stmt.step();
		while (rc == SQLITE_ROW) {
			column_ids.insert(make_pair<string, sqlite3_int64>((char*)sqlite3_column_text(stmt.get(), 1), sqlite3_column_int64(stmt.get(), 0)));
			rc = stmt.step();
		}

		//find all children of root
		load_child_tasks(head);
	}

	// --- private interface ---

	void TaskTree::load_child_tasks(Task& parent) {
		UniqueSqliteStmt stmt(db.get(), "SELECT * FROM tasks WHERE parent = ?");

		stmt.bind(1, parent.id);

		int err = stmt.step();

		//load all tasks from the database
		while(err == SQLITE_ROW) {
			parent.add_child(Task(
						stmt.column_string(column_ids.at("name")),
						stmt.column_int(column_ids.at("creation_time")),
						stmt.column_int(column_ids.at("id")),
						stmt.column_int(column_ids.at("completed")),
						&parent
						));

			err = stmt.step();
		}

		for (int i = 0; i < parent.get_child_count(); ++i) {
			load_child_tasks(parent.get_child(i));
		}

		//throw error if sqlite fails
		if (err != SQLITE_DONE) THROW_SQL_ERROR;
	}

	// --- public interface ---
	Task& TaskTree::add_child(Task& parent, const std::string& name) {
		UniqueSqliteStmt stmt(db.get(), "INSERT INTO tasks (parent, creation_time, name) VALUES (?, ?, ?)");
		time_t creation_time = chrono::system_clock::to_time_t(chrono::system_clock::now());

		stmt.bind(1, parent.id);
		stmt.bind(2, (sqlite3_int64)creation_time);
		stmt.bind(3, name);

		int rc = stmt.step();
		if (rc != SQLITE_DONE && rc != SQLITE_ROW) THROW_SQL_ERROR;

		Task& out = parent.add_child(Task(
					name,
					creation_time,
					sqlite3_last_insert_rowid(db.get()),
					false,
					&parent
					));

		return out;
	}

	void TaskTree::remove(Task& task) {
		if (task.id == 0) {
			throw invalid_argument("cannot remove head task");
		}

		for (int i = 0; i < task.get_child_count(); ++i) {
			remove(task.get_child(i));
		}

		UniqueSqliteStmt stmt(db.get(), "DELETE FROM tasks WHERE id = ?");
		stmt.bind(1, task.id);

		int rc = stmt.step();
		if (rc != SQLITE_DONE) {
			throw runtime_error(sqlite3_errmsg(db.get()));
		}

		if (task.parent == nullptr) {
			throw runtime_error("parent cannot be null. are you trying to remove the root task?");
		}

		for (int i = 0; i < task.parent->get_child_count(); ++i) {
			if (task.parent->get_child(i).get_id() == task.get_id()) {
				task.parent->children.erase(task.parent->children.begin() + i);
			}
		}
	}

	void TaskTree::set_task_name(Task& task, const std::string& name) {
		if (task.id == 0) {
			throw invalid_argument("head task's name should not be set");
		}

		UniqueSqliteStmt stmt(db.get(), "UPDATE tasks SET name=? WHERE id=?");
		stmt.bind(1, name);
		stmt.bind(2, task.id);

		if (stmt.step() != SQLITE_DONE) {
			throw runtime_error(sqlite3_errmsg(db.get()));
		}

		task.name = name;
	}

	void TaskTree::toggle_task_completed(Task& task) {
		UniqueSqliteStmt stmt(db.get(), "UPDATE tasks SET completed=? WHERE id=?");
		stmt.bind(1, (sqlite3_int64)!task.completed);
		stmt.bind(2, task.id);

		if (stmt.step() != SQLITE_DONE) {
			throw runtime_error(sqlite3_errmsg(db.get()));
		}

		task.completed = !task.completed;
	}

	Task* TaskTree::get_by_id(sqlite3_int64 id) {
		tasktree::Task& task = get_head();

		return head.get_child_by_id(id);
	}

# undef THROW_SQL_ERROR

	void TaskTree::print() {
		for (int i = 0; i < head.get_child_count(); ++i) {
			cout << head.get_child(i).id << ": " << head.get_child(i).name
				<< endl;
		}
	}
} // namespace tasktree
