#include <stdexcept>
#include "unique-sqlite.hpp"

using namespace std;

namespace database {
	UniqueSqliteStmt::UniqueSqliteStmt(sqlite3* db, const string sql_code): UniqueSqliteStmt() {
		//stop if db is null
		if (db == nullptr)
			throw runtime_error("db cannot be null");

		//end class
		sqlite3_stmt* new_stmt = nullptr;
		int rc = sqlite3_prepare(db, sql_code.c_str(), -1, &new_stmt, nullptr);
		reset(new_stmt);

		//throw when sql err
		if (rc != SQLITE_OK) {
			free(new_stmt);
			throw runtime_error(sqlite3_errmsg(db));
		}
	}
}
