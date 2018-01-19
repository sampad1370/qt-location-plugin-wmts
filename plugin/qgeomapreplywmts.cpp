#include "qgeomapreplywmts.h"

#include <QtLocation/private/qgeotilespec_p.h>

QGeoMapReplyWmts::QGeoMapReplyWmts(QNetworkReply* reply, const QGeoTileSpec& spec, const QString& format, QObject* parent)
: QGeoTiledMapReply(spec, parent), m_format(format)
{
    if (!reply) {
        setError(UnknownError, QStringLiteral("Null reply"));
        return;
    }
    connect(reply, SIGNAL(finished()), this, SLOT(networkReplyFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
        this, SLOT(networkReplyError(QNetworkReply::NetworkError)));
    connect(this, &QGeoTiledMapReply::aborted, reply, &QNetworkReply::abort);
    connect(this, &QObject::destroyed, reply, &QObject::deleteLater);
}

QGeoMapReplyWmts::~QGeoMapReplyWmts()
{
}

void QGeoMapReplyWmts::networkReplyFinished()
{
    QNetworkReply* reply = static_cast<QNetworkReply*>(sender());
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    setMapImageData(reply->readAll());
    setMapImageFormat(m_format);
    setFinished(true);
}

void QGeoMapReplyWmts::networkReplyError(QNetworkReply::NetworkError error)
{
    QNetworkReply* reply = static_cast<QNetworkReply*>(sender());
    reply->deleteLater();
    if (error == QNetworkReply::OperationCanceledError)
        setFinished(true);
    else
        setError(QGeoTiledMapReply::CommunicationError, reply->errorString());
}
