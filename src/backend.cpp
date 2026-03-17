#include "backend.hpp"
#include <QDebug>
#include <QtCore>
#include <algorithm>
#include <qalgorithms.h>
#include "util.hpp"

TaskModel::TaskModel(Backend& backend, tasktree::Task* ref, QObject* parent)
	:QObject(parent), backend(backend), m_ref(ref), m_id(ref->get_id()), m_name(QString::fromStdString(ref->get_name())), m_completed(ref->is_completed()) {}

Backend::Backend(QObject* parent)
    : QObject(parent), m_tree(getDbPath()) {
	m_current = new TaskModel(*this, &m_tree.get_head());
    loadTasks();
}

Backend::~Backend() {
	qDeleteAll(m_tasks);
	delete m_current;
}

void Backend::loadTasks() {
    // Clear existing tasks
	QList<QObject*> old_tasks = m_tasks;
	m_tasks.clear();

    // Load all tasks from the head task's children
    tasktree::Task& current = *m_current->getRef();
    for (size_t i = 0; i < current.get_child_count(); ++i) {
        tasktree::Task& child = current.get_child(i);
        m_tasks.append(new TaskModel(*this, &child, this));
    }

	// sort completed tasks to end
	std::sort(m_tasks.begin(), m_tasks.end(), [](QObject* a, QObject* b) {
    	return static_cast<TaskModel*>(a)->isCompleted() < static_cast<TaskModel*>(b)->isCompleted();
	});

	// sort by creation time
	int first_completed;
	for (first_completed = 0; first_completed < m_tasks.size() && !static_cast<TaskModel*>(m_tasks.at(first_completed))->isCompleted(); ++first_completed);

	auto creation_compare = [](QObject* a, QObject* b) -> bool {
		return static_cast<TaskModel*>(a)->getRef()->get_creation_time() < static_cast<TaskModel*>(b)->getRef()->get_creation_time();
	};

	std::sort(m_tasks.begin(), m_tasks.begin() + first_completed, creation_compare);
	std::sort(m_tasks.begin() + first_completed, m_tasks.end(), creation_compare);

    qDeleteAll(old_tasks);
}

void Backend::setCurrent(TaskModel* current) {
	TaskModel* old_current = m_current;
	if (m_tasks.contains(current)) current = new TaskModel(*this, current->getRef());
	m_current = current;

	refreshTasks();
	emit currentChanged();
	delete old_current;
}

void Backend::addTask(const QString& name) {
    if (name.isEmpty()) {
        qWarning() << "Cannot add task with empty name";
        return;
    }
    m_tree.add_child(*m_current->getRef(), name.toStdString());
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
	if (task == nullptr) return;
	if (task->isCompleted() == completed) return;

	m_tree.toggle_task_completed(*task->getRef());
	task->m_completed = completed;

	refreshTasks();
}

void Backend::goBack() {
	if (getCurrent()->m_id == 0) return;
	setCurrent(new TaskModel(*this, &getCurrent()->getRef()->get_parent()));
}
