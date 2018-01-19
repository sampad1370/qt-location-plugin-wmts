#pragma once

#include <QtNetwork/QNetworkReply>
#include <QtLocation/private/qgeotiledmapreply_p.h>
#include <QtCore/QPointer>

class QGeoMapReplyWmts : public QGeoTiledMapReply
{
    Q_OBJECT

public:
    explicit QGeoMapReplyWmts(QNetworkReply *reply, const QGeoTileSpec &spec, const QString &format, QObject *parent = 0);
    ~QGeoMapReplyWmts();

private Q_SLOTS:
    void networkReplyFinished();
    void networkReplyError(QNetworkReply::NetworkError error);

private:
    QString m_format;
};

