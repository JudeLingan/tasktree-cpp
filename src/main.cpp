#include <iostream>
#include <vector>
#include <ctime>
#include <filesystem>
#include "sqlite3.h"

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
	return DATA_DIR + "tasktree.db";
}

static sqlite3 *db;

int main() {
	cout << get_db_dir() << endl;
	//sqlite3_open(get_db_dir().c_str(), &db);
	//sqlite3_close(db);
}
