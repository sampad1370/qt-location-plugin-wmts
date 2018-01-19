#include "qgeotiledmappingmanagerenginewmts.h"
#include "qgeotilefetcherwmts.h"

#include <QtLocation/private/qgeocameracapabilities_p.h>
#include <QtLocation/private/qgeomaptype_p.h>
#include <QtLocation/private/qgeotiledmap_p.h>

QGeoTiledMappingManagerEngineWmts::QGeoTiledMappingManagerEngineWmts(const QVariantMap& parameters, QGeoServiceProvider::Error* error, QString* errorString)
: QGeoTiledMappingManagerEngine()
{
    m_prefetchStyle = QGeoTiledMap::NoPrefetching;

    QGeoCameraCapabilities cameraCaps;
    cameraCaps.setMinimumZoomLevel(0.0);
    cameraCaps.setMaximumZoomLevel(30.0);
    cameraCaps.setSupportsBearing(true);
    cameraCaps.setSupportsTilting(true);
    cameraCaps.setMinimumTilt(0);
    cameraCaps.setMaximumTilt(80);
    cameraCaps.setMinimumFieldOfView(20.0);
    cameraCaps.setMaximumFieldOfView(120.0);
    cameraCaps.setOverzoomEnabled(true);
    setCameraCapabilities(cameraCaps);

    setTileSize(QSize(256, 256));

    const QByteArray pluginName = "wmts";
    QList<QGeoMapType> mapTypes;

    //    mapTypes << QGeoMapType(QGeoMapType::StreetMap, QStringLiteral("mapbox.streets"), tr("Street"), false, false, mapTypes.size() + 1, pluginName, cameraCaps);

    int scaleFactor   = 1;
    auto* tileFetcher = new QGeoTileFetcherWmts(scaleFactor, this);
    if (parameters.contains(QStringLiteral("wmts.url"))) {
        qDebug() << "Url " << parameters["wmts.url"].toString();
        tileFetcher->setUrl(parameters["wmts.url"].toString());
    }

    QVector<QString> mapIds;
    for (int i = 0; i < mapTypes.size(); ++i)
        mapIds.push_back(mapTypes[i].name());

    setSupportedMapTypes(mapTypes);

    //    if (parameters.contains(QStringLiteral("mapbox.mapping.highdpi_tiles"))) {
    //        const QString param = parameters.value(QStringLiteral("mapbox.mapping.highdpi_tiles")).toString().toLower();
    //        if (param == "true")
    //            scaleFactor = 2;
    //    }

    tileFetcher->setMapIds(mapIds);

    if (parameters.contains(QStringLiteral("useragent"))) {
        const QByteArray ua = parameters.value(QStringLiteral("useragent")).toString().toLatin1();
        tileFetcher->setUserAgent(ua);
    } else {
        *error       = QGeoServiceProvider::MissingRequiredParameterError;
        *errorString = "Requirted wmts.url parameter is missing";
    }

    setTileFetcher(tileFetcher);

    //    // TODO: do this in a plugin-neutral way so that other tiled map plugins
    //    //       don't need this boilerplate or hardcode plugin name

    //    if (parameters.contains(QStringLiteral("mapbox.mapping.cache.directory"))) {
    //        m_cacheDirectory = parameters.value(QStringLiteral("mapbox.mapping.cache.directory")).toString();
    //    } else {
    //        // managerName() is not yet set, we have to hardcode the plugin name below
    //        m_cacheDirectory = QAbstractGeoTileCache::baseLocationCacheDirectory() + QLatin1String(pluginName);
    //    }

    //    QGeoFileTileCache *tileCache = new QGeoFileTileCacheMapbox(mapTypes, scaleFactor, m_cacheDirectory);

    //    /*
    //     * Disk cache setup -- defaults to Unitary since:
    //     *
    //     * The Mapbox free plan allows for 6000 tiles to be stored for offline uses,
    //     * As of 2016.06.15, according to https://www.mapbox.com/help/mobile-offline/ .
    //     * Thus defaulting to Unitary strategy, and setting 6000 tiles as default cache disk size
    //     */
    //    if (parameters.contains(QStringLiteral("mapbox.mapping.cache.disk.cost_strategy"))) {
    //        QString cacheStrategy = parameters.value(QStringLiteral("mapbox.mapping.cache.disk.cost_strategy")).toString().toLower();
    //        if (cacheStrategy == QLatin1String("bytesize"))
    //            tileCache->setCostStrategyDisk(QGeoFileTileCache::ByteSize);
    //        else
    //            tileCache->setCostStrategyDisk(QGeoFileTileCache::Unitary);
    //    } else {
    //        tileCache->setCostStrategyDisk(QGeoFileTileCache::Unitary);
    //    }
    //    if (parameters.contains(QStringLiteral("mapbox.mapping.cache.disk.size"))) {
    //        bool ok = false;
    //        int cacheSize = parameters.value(QStringLiteral("mapbox.mapping.cache.disk.size")).toString().toInt(&ok);
    //        if (ok)
    //            tileCache->setMaxDiskUsage(cacheSize);
    //    } else {
    //        if (tileCache->costStrategyDisk() == QGeoFileTileCache::Unitary)
    //            tileCache->setMaxDiskUsage(6000); // The maximum allowed with the free tier
    //    }

    //    /*
    //     * Memory cache setup -- defaults to ByteSize (old behavior)
    //     */
    //    if (parameters.contains(QStringLiteral("mapbox.mapping.cache.memory.cost_strategy"))) {
    //        QString cacheStrategy = parameters.value(QStringLiteral("mapbox.mapping.cache.memory.cost_strategy")).toString().toLower();
    //        if (cacheStrategy == QLatin1String("bytesize"))
    //            tileCache->setCostStrategyMemory(QGeoFileTileCache::ByteSize);
    //        else
    //            tileCache->setCostStrategyMemory(QGeoFileTileCache::Unitary);
    //    } else {
    //        tileCache->setCostStrategyMemory(QGeoFileTileCache::ByteSize);
    //    }
    //    if (parameters.contains(QStringLiteral("mapbox.mapping.cache.memory.size"))) {
    //        bool ok = false;
    //        int cacheSize = parameters.value(QStringLiteral("mapbox.mapping.cache.memory.size")).toString().toInt(&ok);
    //        if (ok)
    //            tileCache->setMaxMemoryUsage(cacheSize);
    //    }

    //    /*
    //     * Texture cache setup -- defaults to ByteSize (old behavior)
    //     */
    //    if (parameters.contains(QStringLiteral("mapbox.mapping.cache.texture.cost_strategy"))) {
    //        QString cacheStrategy = parameters.value(QStringLiteral("mapbox.mapping.cache.texture.cost_strategy")).toString().toLower();
    //        if (cacheStrategy == QLatin1String("bytesize"))
    //            tileCache->setCostStrategyTexture(QGeoFileTileCache::ByteSize);
    //        else
    //            tileCache->setCostStrategyTexture(QGeoFileTileCache::Unitary);
    //    } else {
    //        tileCache->setCostStrategyTexture(QGeoFileTileCache::ByteSize);
    //    }
    //    if (parameters.contains(QStringLiteral("mapbox.mapping.cache.texture.size"))) {
    //        bool ok = false;
    //        int cacheSize = parameters.value(QStringLiteral("mapbox.mapping.cache.texture.size")).toString().toInt(&ok);
    //        if (ok)
    //            tileCache->setExtraTextureUsage(cacheSize);
    //    }

    //    /* PREFETCHING */
    //    if (parameters.contains(QStringLiteral("mapbox.mapping.prefetching_style"))) {
    //        const QString prefetchingMode = parameters.value(QStringLiteral("mapbox.mapping.prefetching_style")).toString();
    //        if (prefetchingMode == QStringLiteral("TwoNeighbourLayers"))
    //            m_prefetchStyle = QGeoTiledMap::PrefetchTwoNeighbourLayers;
    //        else if (prefetchingMode == QStringLiteral("OneNeighbourLayer"))
    //            m_prefetchStyle = QGeoTiledMap::PrefetchNeighbourLayer;
    //        else if (prefetchingMode == QStringLiteral("NoPrefetching"))
    //            m_prefetchStyle = QGeoTiledMap::NoPrefetching;
    //    }

    //    setTileCache(tileCache);

    *error = QGeoServiceProvider::NoError;
    errorString->clear();
}

QGeoMap* QGeoTiledMappingManagerEngineWmts::createMap()
{
    QGeoTiledMap* map = new QGeoTiledMap(this, 0);
    map->setPrefetchStyle(m_prefetchStyle);
    return map;
}
