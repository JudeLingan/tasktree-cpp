#pragma once

#include <QObject>
#include <QList>
#include <QString>
#include <qstandardpaths.h>
#include "tasktree/tasktree.hpp"

class TaskModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(tasktree::Task* ref READ getRef)
    Q_PROPERTY(sqlite3_int64 id READ getId CONSTANT)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)

public:
    TaskModel(tasktree::Task* ref, QObject* parent = nullptr);

	tasktree::Task* getRef() const noexcept { return m_ref; }
    sqlite3_int64 getId() const noexcept { return m_id; }
    QString getName() const noexcept { return m_name; }
	
	void setName(QString newName);

signals:
	void nameChanged(QString newName);

private:
	tasktree::Task* m_ref;
    const sqlite3_int64 m_id;
    QString m_name;
};

class Backend : public QObject {
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> tasks READ getTasks NOTIFY tasksChanged)

public:
    explicit Backend(QObject* parent = nullptr);
    ~Backend();

    QList<QObject*> getTasks() const { return m_tasks; }

    Q_INVOKABLE void addTask(const QString& name);
    Q_INVOKABLE void deleteTask(TaskModel* task);
    Q_INVOKABLE void refreshTasks();
	Q_INVOKABLE void changeTaskName(TaskModel* task, const QString& new_name);

signals:
    void tasksChanged();

private:
    void loadTasks();

    tasktree::TaskTree m_tree;
    QList<QObject*> m_tasks;
};
