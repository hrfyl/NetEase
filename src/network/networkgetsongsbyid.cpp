#include "networkgetsongsbyid.h"
#include "../configure/urlconfigure.h"
#include "../mediaplayer/mediaplayer.h"
#include "networkrecommend.h"
#include "networksearchmusic.h"
#include <QHash>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QEventLoop>

NetworkGetSongsById::NetworkGetSongsById(QObject *parent, bool blockade) :
	NetworkBase(parent, blockade)
{
}

void NetworkGetSongsById::disposeifno(const QByteArray &/*data*/)
{

}

QString NetworkGetSongsById::getMusicUrlBySongId(const int &id)
{
	QNetworkAccessManager m_manager;
	QNetworkRequest request;
	QEventLoop loop;					// 使用QEventLoop实现阻塞
	QNetworkReply *reply;

	request.setUrl(QUrl(SONG_URL + QString("[%1]").arg(id)));

	initHead(request);

	reply = m_manager.get(request);

	connect(&m_manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit())); // connect
	loop.exec();
	QByteArray data = reply->readAll();

	QJsonObject obj;
	if (!jsonObject(data, obj))
		return "";

	QJsonArray songObj = obj["songs"].toArray();
	QString url = songObj.at(0).toObject()["mp3Url"].toString();

	reply->deleteLater();
	return url;
}




















