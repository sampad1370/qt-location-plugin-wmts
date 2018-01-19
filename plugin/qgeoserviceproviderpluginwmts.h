#pragma once
#include <QtCore/QObject>
#include <QtLocation/QGeoServiceProviderFactory>

class QGeoServiceProviderFactoryWmts : public QObject, public QGeoServiceProviderFactory
{
    Q_OBJECT
    Q_INTERFACES(QGeoServiceProviderFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.qt.geoservice.serviceproviderfactory/5.0" FILE "wmts_plugin.json")

public:
    QGeoCodingManagerEngine* createGeocodingManagerEngine(const QVariantMap& parameters,
        QGeoServiceProvider::Error* error,
        QString* errorString) const override;
    QGeoMappingManagerEngine* createMappingManagerEngine(const QVariantMap& parameters,
        QGeoServiceProvider::Error* error,
        QString* errorString) const override;
    QGeoRoutingManagerEngine* createRoutingManagerEngine(const QVariantMap& parameters,
        QGeoServiceProvider::Error* error,
        QString* errorString) const override;
    QPlaceManagerEngine* createPlaceManagerEngine(const QVariantMap& parameters,
        QGeoServiceProvider::Error* error,
        QString* errorString) const override;
};
