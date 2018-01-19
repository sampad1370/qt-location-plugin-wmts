#pragma once

#include <QtLocation/private/qgeotilefetcher_p.h>
#include <qvector.h>

class QGeoTiledMappingManagerEngine;
class QNetworkAccessManager;

class QGeoTileFetcherWmts : public QGeoTileFetcher
{
    Q_OBJECT

public:
    QGeoTileFetcherWmts(int scaleFactor, QGeoTiledMappingManagerEngine* parent);

    void setUrl(const QString& url);
    void setUserAgent(const QByteArray& userAgent);
    void setMapIds(const QVector<QString>& mapIds);
    void setFormat(const QString& format);
    void setAccessToken(const QString& accessToken);

private:
    QGeoTiledMapReply* getTileImage(const QGeoTileSpec& spec);

    QNetworkAccessManager* m_networkManager;
    QByteArray m_userAgent;
    QString m_url;
    QString m_format;
    QString m_replyFormat;
    QString m_accessToken;
    QVector<QString> m_mapIds;
    int m_scaleFactor;
};
