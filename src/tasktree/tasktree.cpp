#include <iostream>
#include <stdexcept>
#include <sqlite3.h>
#include "tasktree.hpp"
#include "unique-sqlite.hpp"

using namespace std;
using namespace database;


namespace tasktree {

#define THROW_SQL_ERROR throw runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ":" + std::string(sqlite3_errmsg(db.get())));
#define HANDLE_SQL_ERROR(func) if((func) != SQLITE_OK) THROW_SQL_ERROR

	// --- constructors ---

	TaskTree::TaskTree(const string& path) {
		//set db
		sqlite3* raw_db;
		HANDLE_SQL_ERROR(sqlite3_open(path.c_str(), &raw_db))
		db.reset(raw_db);

		//create table if not exists
		string sql_tablegen = "CREATE TABLE IF NOT EXISTS tasks (id INTEGER PRIMARY KEY NOT NULL CHECK (id != 0), parent INTEGER, name TEXT, creation_time INTEGER)";
		HANDLE_SQL_ERROR(sqlite3_exec(db.get(), sql_tablegen.c_str(), nullptr, nullptr, nullptr));
		
		//load all column names and ids
		UniqueSqliteStmt stmt(db.get(), "PRAGMA table_info(tasks)");
		int rc = sqlite3_step(stmt.get());
		while (rc == SQLITE_ROW) {
			column_ids.insert(make_pair<string, sqlite3_int64>((char*)sqlite3_column_text(stmt.get(), 1), sqlite3_column_int64(stmt.get(), 0)));
			rc = sqlite3_step(stmt.get());
		}

		//find all children of root
		load_child_tasks(head);
	}

	// --- private interface ---

	void TaskTree::load_child_tasks(Task& parent) {
		UniqueSqliteStmt stmt = UniqueSqliteStmt(db.get(), "SELECT * FROM tasks WHERE id = ?");

		HANDLE_SQL_ERROR(sqlite3_bind_int64(stmt.get(), 1, parent.id))

		int err = sqlite3_step(stmt.get());

		//load all tasks from the database
		while(err == SQLITE_ROW) {
			err = sqlite3_step(stmt.get());

			parent.add_child(Task(
						(const char*)sqlite3_column_text(stmt.get(), column_ids.at("name")),
						(time_t)sqlite3_column_int64(stmt.get(), column_ids.at("creation_time")),
						sqlite3_column_int64(stmt.get(), column_ids.at("id")),
						&parent
			));

		}

		//throw error if sqlite fails
		if (err != SQLITE_DONE) THROW_SQL_ERROR;

		cout << "tasktree loaded" << endl;
	}

	// --- public interface ---

# undef HANDLE_SQL_ERROR

}
