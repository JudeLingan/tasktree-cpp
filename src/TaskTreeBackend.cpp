#include "TaskTreeBackend.hpp"
#include <filesystem>
#include <QDebug>
#include <QtCore>
#include <qstandardpaths.h>

#if defined(__WIN32) || defined(__WIN64)
#define SLASH '\\'
#else
#define SLASH '/'
#endif

const std::string DATA_DIR = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).toStdString() + SLASH + "tasktree";

std::string getDbPath() {
    std::filesystem::create_directories(DATA_DIR);
    return DATA_DIR + SLASH + "tasktree.db";
}

void TaskModel::setName(QString name) {
	m_name = name;
	emit nameChanged(name);
}

TaskTreeBackend::TaskTreeBackend(QObject* parent)
    : QObject(parent), m_tree(getDbPath()) {
    loadTasks();
}

TaskTreeBackend::~TaskTreeBackend() = default;

void TaskTreeBackend::loadTasks() {
    // Clear existing tasks
    qDeleteAll(m_tasks);
    m_tasks.clear();

    // Load all tasks from the head task's children
    tasktree::Task& head = m_tree.get_head();
    for (size_t i = 0; i < head.get_child_count(); ++i) {
        tasktree::Task& child = head.get_child(i);
        m_tasks.append(new TaskModel(&child, this));
    }
}

void TaskTreeBackend::addTask(const QString& name) {
    if (name.isEmpty()) {
        qWarning() << "Cannot add task with empty name";
        return;
    }
    m_tree.add_child(m_tree.get_head(), name.toStdString());
    refreshTasks();
}

void TaskTreeBackend::deleteTask(TaskModel* task) {
	m_tree.remove(*task->getRef());
	refreshTasks();
}

void TaskTreeBackend::refreshTasks() {
    loadTasks();
    emit tasksChanged();
}
