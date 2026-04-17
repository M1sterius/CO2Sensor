#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "Backend.hpp"

int main(int argc, char** argv)
{
    QQuickStyle::setStyle("Fusion");
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Dashboard", "Main");

    CO2::PC::Backend backend;
    backend.Start();

    engine.rootContext()->setContextProperty("backend", &backend);

    return app.exec();
}
