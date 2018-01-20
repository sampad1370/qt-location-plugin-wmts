#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtDebug>

//#include "qgeoserviceproviderpluginwms.h"

//Q_IMPORT_PLUGIN(QGeoServiceProviderFactoryWms)

int main(int argc, char *argv[])
{
    QCoreApplication::addLibraryPath(".");
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/demo.qml")));

    return app.exec();
}
