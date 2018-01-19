#pragma once
#include <QtLocation/QGeoServiceProvider>

#include <QtLocation/private/qgeotiledmappingmanagerengine_p.h>

class QGeoTiledMappingManagerEngineWmts : public QGeoTiledMappingManagerEngine
{
    Q_OBJECT

public:
    QGeoTiledMappingManagerEngineWmts(const QVariantMap& parameters, QGeoServiceProvider::Error* error, QString* errorString);

    QGeoMap* createMap();

private:
    QString m_cacheDirectory;
};
