#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "backend.hpp"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
	Backend backend;
	QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("backend", &backend);
	engine.load("src/ui/Main.qml");

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
