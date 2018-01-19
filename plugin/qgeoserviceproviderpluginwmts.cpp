#include "qgeoserviceproviderpluginwmts.h"
#include "qgeotiledmappingmanagerenginewmts.h"

QGeoCodingManagerEngine* QGeoServiceProviderFactoryWmts::createGeocodingManagerEngine(
    const QVariantMap& parameters, QGeoServiceProvider::Error* error, QString* errorString) const
{
    Q_UNUSED(parameters)
    Q_UNUSED(error)
    Q_UNUSED(errorString)

    return nullptr;
}

QGeoMappingManagerEngine* QGeoServiceProviderFactoryWmts::createMappingManagerEngine(
    const QVariantMap& parameters, QGeoServiceProvider::Error* error, QString* errorString) const
{
    qDebug() << "Create geo wmts plugin";
    return new QGeoTiledMappingManagerEngineWmts(parameters, error, errorString);
}

QGeoRoutingManagerEngine* QGeoServiceProviderFactoryWmts::createRoutingManagerEngine(
    const QVariantMap& parameters, QGeoServiceProvider::Error* error, QString* errorString) const
{
    Q_UNUSED(parameters)
    Q_UNUSED(error)
    Q_UNUSED(errorString)

    return nullptr;
}

QPlaceManagerEngine* QGeoServiceProviderFactoryWmts::createPlaceManagerEngine(
    const QVariantMap& parameters, QGeoServiceProvider::Error* error, QString* errorString) const
{
    Q_UNUSED(parameters)
    Q_UNUSED(error)
    Q_UNUSED(errorString)

    return nullptr;
}
