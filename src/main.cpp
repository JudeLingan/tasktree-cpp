#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <memory>
#include "TaskTreeBackend.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    auto backend = std::make_unique<TaskTreeBackend>();
    auto engine = std::make_unique<QQmlApplicationEngine>();
    
    engine->rootContext()->setContextProperty("backend", backend.get());
    
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    engine->load(url);
    
    if (engine->rootObjects().isEmpty())
        return -1;

    int result = app.exec();
    
    // Explicitly destroy in the correct order
    engine.reset();
    backend.reset();
    
    return result;
}
