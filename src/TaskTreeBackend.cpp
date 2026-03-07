#include "TaskTreeBackend.hpp"
#include <filesystem>
#include <QDebug>

#if defined(__WIN32) || defined(__WIN64)
#define SLASH '\\'
const std::string DATA_DIR = std::string(getenv("USERDATA"));
#else
#define SLASH '/'
const std::string DATA_DIR = std::string(getenv("HOME")) + "/.local/share/tasktree";
#endif

std::string getDbPath() {
    std::filesystem::create_directories(DATA_DIR);
    return DATA_DIR + SLASH + "tasktree.db";
}

TaskTreeBackend::TaskTreeBackend(QObject* parent)
    : QObject(parent), m_tree(std::make_unique<tasktree::TaskTree>(getDbPath())) {
    loadTasks();
}

TaskTreeBackend::~TaskTreeBackend() = default;

void TaskTreeBackend::loadTasks() {
    // Clear existing tasks
    qDeleteAll(m_tasks);
    m_tasks.clear();

    // Load all tasks from the head task's children
    tasktree::Task& head = m_tree->get_head();
    for (size_t i = 0; i < head.get_child_count(); ++i) {
        tasktree::Task& child = head.get_child(i);
        TaskModel* model = new TaskModel(child.get_id(), QString::fromStdString(child.get_name()), this);
        m_tasks.append(model);
    }
}

void TaskTreeBackend::addTask(const QString& name) {
    if (name.isEmpty()) {
        qWarning() << "Cannot add task with empty name";
        return;
    }
    m_tree->add_child(m_tree->get_head(), name.toStdString());
    refreshTasks();
}

void TaskTreeBackend::deleteTask(sqlite3_int64 id) {
    tasktree::Task* task = m_tree->get_by_id(id);
    if (task != nullptr) {
        m_tree->remove(*task);
        refreshTasks();
    }
}

void TaskTreeBackend::refreshTasks() {
    loadTasks();
    emit tasksChanged();
}
