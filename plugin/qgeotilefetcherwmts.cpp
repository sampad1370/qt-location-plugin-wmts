#include "qgeotilefetcherwmts.h"
#include "qgeomapreplywmts.h"

#include <QDebug>
#include <QtLocation/private/qgeotilespec_p.h>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

QGeoTileFetcherWmts::QGeoTileFetcherWmts(int scaleFactor, QGeoTiledMappingManagerEngine* parent)
: QGeoTileFetcher(parent), m_networkManager(new QNetworkAccessManager(this)), m_userAgent("Qt Location based application"), m_format("png"), m_replyFormat("png"), m_accessToken("")
{
    m_scaleFactor = qBound(1, scaleFactor, 2);
}

void QGeoTileFetcherWmts::setUrl(const QString& url)
{
    m_url = url;
}

void QGeoTileFetcherWmts::setUserAgent(const QByteArray& userAgent)
{
    m_userAgent = userAgent;
}

void QGeoTileFetcherWmts::setMapIds(const QVector<QString>& mapIds)
{
    m_mapIds = mapIds;
}

void QGeoTileFetcherWmts::setFormat(const QString& format)
{
    m_format = format;

    if (m_format == "png" || m_format == "png32" || m_format == "png64" || m_format == "png128" || m_format == "png256")
        m_replyFormat = "png";
    else if (m_format == "jpg70" || m_format == "jpg80" || m_format == "jpg90")
        m_replyFormat = "jpg";
    else
        qWarning() << "Unknown map format " << m_format;
}

void QGeoTileFetcherWmts::setAccessToken(const QString& accessToken)
{
    m_accessToken = accessToken;
}

QGeoTiledMapReply* QGeoTileFetcherWmts::getTileImage(const QGeoTileSpec& spec)
{
    qWarning() << "Get tile image " << spec.zoom() << " " << spec.x() << " " << spec.y();

    QNetworkRequest request;
    request.setRawHeader("User-Agent", m_userAgent);

    request.setUrl(QUrl(m_url + "?SERVICE=WMTS&VERSION=1.0.0&REQUEST=GetTile&LAYER=omwrgbmrvl&STYLE=&FORMAT=image/png&TILEMATRIXSET=GoogleMapsVL" +
                        "&TILEMATRIX=" + QString::number(spec.zoom()) +
                        "&TILEROW=" + QString::number(spec.y()) +
                        "&TILECOL=" + QString::number(spec.x())));

    //qWarning() << "Get tile image url: " << request.url();

    QNetworkReply* reply = m_networkManager->get(request);

    return new QGeoMapReplyWmts(reply, spec, m_replyFormat);
}
