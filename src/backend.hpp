#pragma once

#include <QObject>
#include <QList>
#include <QString>
#include <qstandardpaths.h>
#include "tasktree/tasktree.hpp"

class Backend;

class TaskModel: public QObject {
	friend Backend;

	Q_OBJECT
		Q_PROPERTY(sqlite3_int64 id READ getId CONSTANT)
		Q_PROPERTY(tasktree::Task* ref READ getRef)
		Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
		Q_PROPERTY(bool completed READ isCompleted NOTIFY completedChanged)

	public:
		TaskModel(Backend& backend, tasktree::Task* ref, QObject* parent = nullptr);

		tasktree::Task* getRef() const noexcept { return m_ref; }
		sqlite3_int64 getId() const noexcept { return m_id; }
		QString getName() const noexcept { return m_name; }
		bool isCompleted() const noexcept { return m_completed; }

	signals:
		void nameChanged(QString newName);
		void completedChanged();

	private:
		tasktree::Task* m_ref;
		const sqlite3_int64 m_id;
		QString m_name;
		bool m_completed;

		Backend& backend;
};

class Backend: public QObject {
	Q_OBJECT
		Q_PROPERTY(QList<QObject*> tasks READ getTasks NOTIFY tasksChanged)
		Q_PROPERTY(TaskModel* current READ getCurrent WRITE setCurrent NOTIFY currentChanged)

	public:
		explicit Backend(QObject* parent = nullptr);
		~Backend();

		QList<QObject*> getTasks() const { return m_tasks; }
		TaskModel* getCurrent() const { return m_current; }

		//exposed functions
		Q_INVOKABLE void addTask(const QString& name);
		Q_INVOKABLE void deleteTask(TaskModel* task);
		Q_INVOKABLE void refreshTasks();
		Q_INVOKABLE void setTaskName(TaskModel* task, const QString& new_name);
		Q_INVOKABLE void setTaskCompleted(TaskModel* task, bool completed);
		Q_INVOKABLE void goBack();

	signals:
		void tasksChanged();
		void currentChanged();

	private:
		void setCurrent(TaskModel* current);
		void loadTasks();

		tasktree::TaskTree m_tree;
		QList<QObject*> m_tasks;
		TaskModel* m_current;
};
