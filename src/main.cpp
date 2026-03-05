#include <iostream>
#include <filesystem>
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

tasktree::TaskTree tree(get_db_dir().c_str());

void cli_rem() {
	sqlite3_int64 id;
	cin >> id;
	if (cin.fail()) {
		cout << "bad input\n";
		cin.clear();
		return;
	}

	tasktree::Task* taskptr = tree.get_by_id(id);

	if (taskptr == nullptr) {
		cout << "task not found";
		return;
	}

	tree.remove(*taskptr);
}

void read_input() {
	string command = "";
	cin.clear();
	while (command != "exit") {
		command = "";
		cout << ">";
		cin >> command;

		if (command == "print") {
			tree.print();
		}

		else if (command == "add") {
			string name;
			cin >> name;
			tree.add_child(tree.get_head(), name);
		}

		else if (command == "rem") {
			cli_rem();
		}
		else {
			cout << "invalid input" << endl;
		}

		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}

int main() {
	read_input();
	return 0;
}
