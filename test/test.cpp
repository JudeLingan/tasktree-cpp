#include <memory>
#include <gtest/gtest.h>
#include "tasktree.hpp"

using namespace std;
using namespace tasktree;


class TaskTreeTest : public testing::Test {
	protected:
		unique_ptr<TaskTree> tt;

		void SetUp() override {
			tt.reset(new TaskTree(":memory:"));
			ASSERT_NE(nullptr, tt.get());
		}

		inline int tt_prepare(sqlite3_stmt** stmt, const char* sql_code) {
			return sqlite3_prepare_v2(
				tt->get_db(),
				sql_code,
				-1,
				stmt,
				nullptr
			);
		}

};

//helper function to assert a database entry is equal to a local task
//step the stmt before sending to function
//it is checking specifically at the step
void checkTaskEqDb(sqlite3_stmt* stmt, const Task& task) {
	ASSERT_NE(nullptr, stmt);

	for (int i = 0; i < sqlite3_column_count(stmt); ++i) {
		if (!strcmp(sqlite3_column_name(stmt, i), "name")) {
			EXPECT_STREQ(task.get_name().c_str(), (char*)sqlite3_column_text(stmt, i));
		}
		else if (!strcmp(sqlite3_column_name(stmt, i), "id")) {
			EXPECT_EQ(task.get_id(), sqlite3_column_int64(stmt, i));
		}
		else if (!strcmp(sqlite3_column_name(stmt, i), "parent")) {
			EXPECT_EQ(task.get_parent().get_id(), sqlite3_column_int64(stmt, i));
		}
		else if (!strcmp(sqlite3_column_name(stmt, i), "completed")) {
			EXPECT_EQ(task.is_completed(), sqlite3_column_int(stmt, i));
		}
		else if (!strcmp(sqlite3_column_name(stmt, i), "creation_time")) {
			EXPECT_EQ(task.get_creation_time(), (time_t)sqlite3_column_int64(stmt, i));
		}
		else {
			ADD_FAILURE() << "not all columns accounted for: missing " << sqlite3_column_name(stmt, i);
		}
	}
}

TEST_F(TaskTreeTest, AddRootChild) {
	const Task& newTask = tt->add_child(tt->get_head(), "test1");

	//ensure child is added
	EXPECT_EQ(&newTask, &tt->get_head().get_child(0));

	sqlite3* db = tt->get_db();
	sqlite3_stmt* stmt = nullptr;

	int rc = tt_prepare(&stmt, "SELECT * FROM tasks WHERE parent = 0");
	ASSERT_EQ(SQLITE_OK, rc);

	rc = sqlite3_step(stmt);
	ASSERT_EQ(SQLITE_ROW, rc);

	checkTaskEqDb(stmt, newTask);
	sqlite3_finalize(stmt);
}

TEST_F(TaskTreeTest, RemoveTaskWithChild) {
	Task& parent = tt->add_child(tt->get_head(), "parent");
	Task& child = tt->add_child(parent, "child");
	Task& child_squared = tt->add_child(child, "child squared");

	tt->remove(parent);

	sqlite3_stmt* stmt;
	int rc = tt_prepare(&stmt, "SELECT * FROM tasks");
	ASSERT_EQ(SQLITE_OK, rc);

	rc = sqlite3_step(stmt);
	EXPECT_EQ(SQLITE_DONE, rc);
	sqlite3_finalize(stmt);
}
