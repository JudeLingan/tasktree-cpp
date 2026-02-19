#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <sqlite3.h>

namespace database {

	class UniqueSqliteStmt: public std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> { //raii wrapper for sqlite stmt
		private:
			friend class Table;

			UniqueSqliteStmt() noexcept : std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)>(nullptr, &sqlite3_finalize) {} //default constructor

		public:
			UniqueSqliteStmt(sqlite3* db, const std::string sql_code); //allows quick creation from db and sql code
	};

	class UniqueSqlite: public std::unique_ptr<sqlite3, decltype(&sqlite3_close)> {
		public:
			UniqueSqlite() noexcept : std::unique_ptr<sqlite3, decltype(&sqlite3_close)>(nullptr, &sqlite3_close) {}
			void exec(const std::string&);
	};

}
