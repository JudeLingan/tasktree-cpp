#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <exception>
#include <qcoreapplication.h>
#include <qlogging.h>
#include "backend.hpp"

int main(int argc, char *argv[]) {
	try {
		QCoreApplication::setApplicationName("tasktree");
		QGuiApplication app(argc, argv);
		Backend backend;
		QQmlApplicationEngine engine;

		engine.rootContext()->setContextProperty("backend", &backend);
		engine.loadFromModule("TasktreeUI", "Main");

		if (engine.rootObjects().isEmpty())
			return -1;

		return app.exec();
	}
	catch (std::exception e) {
		qCritical("Tasktree exception get: %s", e.what());
	}
}
