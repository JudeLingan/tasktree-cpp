#include <memory>
#include <stdexcept>
#include <cassert>
#include "unique-sqlite.hpp"

using namespace std;

namespace database {
	UniqueSqlite::UniqueSqlite(const std::string& path): std::unique_ptr<sqlite3, decltype(&sqlite3_close)>(nullptr, &sqlite3_close) {
		sqlite3* raw_db = nullptr;

		int rc = sqlite3_open(path.c_str(), &raw_db);

		if (raw_db == nullptr) {
			throw runtime_error("loading sqlite database failed");
		}
		if (rc != SQLITE_OK) {
			free(raw_db);
			throw runtime_error(sqlite3_errmsg(raw_db));
		}

		reset(raw_db);
		assert(get() != nullptr);
	}
	
	void UniqueSqlite::exec(std::string code, int (*callback)(void *, int, char **, char **), void* data) {
		char* err;
		sqlite3_exec(get(), code.c_str(), callback, data, &err);

		if (err != nullptr) {
			throw runtime_error(err);
			free(err);
		}
	}

	UniqueSqliteStmt::UniqueSqliteStmt(sqlite3* db, const string sql_code): UniqueSqliteStmt() {
		//stop if db is null
		if (db == nullptr)
			throw runtime_error("db cannot be null");

		//end class
		sqlite3_stmt* new_stmt = nullptr;
		int rc = sqlite3_prepare(db, sql_code.c_str(), -1, &new_stmt, nullptr);

		//throw when sql err
		if (rc != SQLITE_OK) {
			free(new_stmt);
			throw runtime_error(sqlite3_errmsg(db));
		}

		reset(new_stmt);
		this->db = db;
	}

#define THROW_SQL_ERROR throw runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ":" + std::string(sqlite3_errmsg(db)));
#define USQLITE_DO(func) assert(db != nullptr); if((func) != SQLITE_OK) THROW_SQL_ERROR

	void bind_check(int index, UniqueSqliteStmt& stmt) {
		if (index < 1) {
			throw out_of_range("index must be at least 1");
		}
		if (index > sqlite3_column_count(stmt.get())) {
			throw out_of_range("index must be less than or equal to column count");
		}
	}

	void column_check(int index, UniqueSqliteStmt& stmt) {
		if (index < 0) {
			throw out_of_range("index must be at least 0");
		}
		if (index >= sqlite3_column_count(stmt.get())) {
			throw out_of_range("index must be less than column count");
		}
	}

	int UniqueSqliteStmt::step() {
		return sqlite3_step(get());
	}

	void UniqueSqliteStmt::bind(int index, string value) {
		bind_check(index, *this);
		USQLITE_DO(sqlite3_bind_text(get(), index, value.c_str(), -1, SQLITE_TRANSIENT));
	}

	void UniqueSqliteStmt::bind(int index, double value) {
		bind_check(index, *this);
		USQLITE_DO(sqlite3_bind_double(get(), index, value));
	}

	void UniqueSqliteStmt::bind(int index, sqlite3_int64 value) {
		bind_check(index, *this);
		USQLITE_DO(sqlite3_bind_int64(get(), index, value));
	}

	string UniqueSqliteStmt::column_string(int index) {
		column_check(index, *this);
		const unsigned char* rawtxt = sqlite3_column_text(get(), index);

		if (rawtxt == nullptr) {
			USQLITE_DO(sqlite3_errcode(db));
			return string();
		}

		return string((char*)rawtxt);
	}

	sqlite3_int64 UniqueSqliteStmt::column_int(int index) {
		column_check(index, *this);
		return sqlite3_column_int64(get(), index);
	}

	double UniqueSqliteStmt::column_double(int index) {
		column_check(index, *this);
		return sqlite3_column_double(get(), index);
	}

#undef USQLITE_DO
}
