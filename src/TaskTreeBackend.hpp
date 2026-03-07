#pragma once

#include <QObject>
#include <QList>
#include <QString>
#include "tasktree/tasktree.hpp"

class TaskModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(sqlite3_int64 id READ getId CONSTANT)
    Q_PROPERTY(QString name READ getName CONSTANT)

public:
    TaskModel(sqlite3_int64 id, const QString& name, QObject* parent = nullptr)
        : QObject(parent), m_id(id), m_name(name) {}

    sqlite3_int64 getId() const { return m_id; }
    QString getName() const { return m_name; }

private:
    sqlite3_int64 m_id;
    QString m_name;
};

class TaskTreeBackend : public QObject {
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> tasks READ getTasks NOTIFY tasksChanged)

public:
    explicit TaskTreeBackend(QObject* parent = nullptr);
    ~TaskTreeBackend();

    QList<QObject*> getTasks() const { return m_tasks; }

    Q_INVOKABLE void addTask(const QString& name);
    Q_INVOKABLE void deleteTask(sqlite3_int64 id);
    Q_INVOKABLE void refreshTasks();

signals:
    void tasksChanged();

private:
    void loadTasks();

    std::unique_ptr<tasktree::TaskTree> m_tree;
    QList<QObject*> m_tasks;
};
