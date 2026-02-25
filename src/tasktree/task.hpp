#include <string>
#include <vector>
#include <sqlite3.h>

namespace tasktree {
	class Task {
		private:
			friend class TaskTree;

			std::string name;
			time_t creation_time;
			sqlite3_int64 id;
			Task* parent;
			std::vector<Task> children;

			//constructor that sets all vars to their respective values
			Task(const std::string& name, time_t creation_time, sqlite3_int64 id, Task* parent) noexcept;

			//same as other constructor but uses current time as creation_time
			Task(const std::string name, sqlite3_int64 id, Task* parent) noexcept;

			Task() noexcept;

			//changes name variable
			void set_name(const std::string& name) noexcept;

			//changes parent variable
			void set_parent(Task* parent) noexcept;

			//adds a child task and returns a reference to it
			Task& add_child(Task child);

			//removes a child task
			void remove_child(int i);

		public:

			//returns name
			std::string get_name() const noexcept { return name; }

			//returns id
			sqlite3_int64 get_id() const noexcept { return id; }

			//returns creation_time
			time_t get_creation_time() const noexcept { return creation_time; }

			//returns number of children
			size_t get_child_count() const noexcept { return children.size(); }

			//returns the parent
			Task& get_parent() const noexcept { return *parent; };

			//returns reference to child at index i
			Task& get_child(int i) { return children.at(i); };

	};
}
