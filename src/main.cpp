#include <iostream>
#include <filesystem>
#include <exception>
#include "tasktree.hpp"

using namespace std;

#if defined(__WIN32) || defined(__WIN64)
#define SLASH '\\'
const string home = getenv("USERDATA");

#else
#define SLASH '/'
const string DATA_DIR = string(getenv("HOME")) + "/.local/share/tasktree";

#endif

string get_db_dir() {
	filesystem::create_directories(DATA_DIR);
	return DATA_DIR + SLASH + "tasktree.db";
}

static sqlite3 *db;

int main() {
	tasktree::TaskTree tree(get_db_dir().c_str());
	return 0;
}
