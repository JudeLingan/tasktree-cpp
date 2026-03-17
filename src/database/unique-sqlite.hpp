#pragma once

#include <memory>
#include <string>
#include <sqlite3.h>

namespace database {
	class UniqueSqlite {
		private:
			std::unique_ptr<sqlite3, decltype(&sqlite3_close)> ptr = std::unique_ptr<sqlite3, decltype(&sqlite3_close)>(nullptr, &sqlite3_close);

		public:
			UniqueSqlite() noexcept : ptr(nullptr, &sqlite3_close) {}

			//simple constructor to open a sqlite file at a path
			explicit UniqueSqlite(const std::string& path);

			void exec(std::string code, int (*callback)(void *, int, char **, char **), void* data);
			sqlite3* get() const { return ptr.get(); }
			void reset(sqlite3* new_ptr) { ptr.reset(new_ptr); }
	};

	class UniqueSqliteStmt { //raii wrapper for sqlite stmt
		private:
			std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> ptr;
			sqlite3* db = nullptr;
			UniqueSqliteStmt() noexcept : ptr(nullptr, &sqlite3_finalize) {} //default constructor

		public:
			//the only public constructor, which allows creation from a database and sql code
			UniqueSqliteStmt(sqlite3* db, const std::string sql_code);

			//get the ptr
			sqlite3_stmt* get() const { return ptr.get(); }

			//reset the ptr
			void reset(sqlite3_stmt* new_ptr) { ptr.reset(new_ptr); }

			//unlike all other helper functions here, this does nothing but call sqlite3_step
			int step();

			//binds value, throwing an error if something goes wrong
			void bind(int index, std::string value);

			//binds value, throwing an error if something goes wrong
			void bind(int index, double value);

			//binds value, throwing an error if something goes wrong
			void bind(int index, sqlite3_int64 value);

			//reads column as string
			std::string column_string(int index);

			//reads column as int
			sqlite3_int64 column_int(int index);

			//reads column as double
			double column_double(int index);
	};
}
