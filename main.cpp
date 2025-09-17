#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>

#include "GraphItem.h"
#include "DataProvider.h"
#include "DataPoint.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    qmlRegisterType<GraphItem>("GraphComponents", 1, 0, "GraphItem");
    qmlRegisterType<DataProvider>("GraphComponents", 1, 0, "DataProvider");
    qmlRegisterUncreatableType<DataPoint>("GraphComponents", 1, 0, "DataPoint",
                                          "DataPoint can only be created in C++");

    DataProvider *dataProvider = new DataProvider(&app);

    dataProvider->startRandomGeneration();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("primaryDataProvider", dataProvider);


    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) {
                QCoreApplication::exit(-1);
            }
        },
        Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
