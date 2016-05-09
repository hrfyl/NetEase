#include "networkaddmusic.h"
#include "../configure/urlconfigure.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

NetworkAddMusic::NetworkAddMusic(QObject *parent) :
	NetworkBase(parent)
{
}

void NetworkAddMusic::addMusic(const QString &songId, QString like, QString time, QString alg)
{
	QUrl url(ADDMUSIC_URL + alg + "&trackId=" + songId + "&like=" + like + "&time=" + time);
	sendGet(url, true);
}

void NetworkAddMusic::disposeifno(const QByteArray &/*data*/)
{
}
