#include "backend.hpp"
#include <QDebug>
#include <QtCore>
#include "util.hpp"

TaskModel::TaskModel(Backend& backend, tasktree::Task* ref, QObject* parent)
	:QObject(parent), backend(backend), m_ref(ref), m_id(ref->get_id()), m_name(QString::fromStdString(ref->get_name())), m_completed(ref->is_completed()) {}

Backend::Backend(QObject* parent)
    : QObject(parent), m_tree(getDbPath()) {
    loadTasks();
}

Backend::~Backend() = default;

void Backend::loadTasks() {
    // Clear existing tasks
    qDeleteAll(m_tasks);
    m_tasks.clear();

    // Load all tasks from the head task's children
    tasktree::Task& head = m_tree.get_head();
    for (size_t i = 0; i < head.get_child_count(); ++i) {
        tasktree::Task& child = head.get_child(i);
        m_tasks.append(new TaskModel(*this, &child, this));
    }
}

void Backend::addTask(const QString& name) {
    if (name.isEmpty()) {
        qWarning() << "Cannot add task with empty name";
        return;
    }
    m_tree.add_child(m_tree.get_head(), name.toStdString());
    refreshTasks();
}

void Backend::deleteTask(TaskModel* task) {
	m_tree.remove(*task->getRef());
	refreshTasks();
}

void Backend::refreshTasks() {
    loadTasks();
    emit tasksChanged();
}

void Backend::setTaskName(TaskModel* task, const QString& name) {
	if (task->getName() == name) return;

	m_tree.set_task_name(*task->getRef(), name.toStdString());
	task->m_name = name;
	task->nameChanged(name);

	assert(task->getName().toStdString() == task->getRef()->get_name());
}

void Backend::setTaskCompleted(TaskModel* task, bool completed) {
	if (task->isCompleted() == completed) return;

	m_tree.toggle_task_completed(*task->getRef());
	task->m_completed = completed;
	task->completedChanged();

	assert(task->isCompleted() == task->getRef()->is_completed());
}
