#include <iostream>
#include <string>
#include "tasktree.hpp"
#include "util.hpp"

using namespace std;
using namespace tasktree;

void list_tasks(Task& task) {
	static int iteration = 0;
	cout << task.get_id() << ": " << string(iteration*4, ' ') << task.get_name() << ": " << task.is_completed() << endl;

	for (int i = 0; i < task.get_child_count(); ++i) {
		list_tasks(task.get_child(i));
	}

	--iteration;
}

void run_cli(TaskTree tt) {
	cout << "> ";
	for (string command; getline(cin, command);) {
		string args = command.substr(command.find(' ') + 1, command.length());
		command = command.substr(0, command.find(' '));
		if (command == "new") {
			tt.add_child(tt.get_head(), args);
		}
		else if (command == "delete") {
			tt.remove(*tt.get_by_id(stoi(args)));
		}
		else if (command == "complete") {
			tt.toggle_task_completed(*tt.get_by_id(stoi(args)));
		}
		else if (command == "list") {
			list_tasks(tt.get_head());
		}
		else {
			cout << "unknown command" << endl;
		}

		cout << "> ";
	}

	cout << "tasktree closed" << endl;
}

int main(int argc, char *argv[]) {
	cout << "Tasktree started" << endl;

	run_cli(TaskTree(getDbPath()));

	return 0;
}
