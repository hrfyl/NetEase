#include "networkmusiclyric.h"
#include "../configure/urlconfigure.h"
#include "../mediaplayer/mediaplayer.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>


NetworkMusicLyric::NetworkMusicLyric(QObject * parent) :
	NetworkBase(parent)
{

}

void NetworkMusicLyric::musicLyric(const int &musicId)
{
	QUrl url(MUSICLYRIC_URL + QString::number(musicId) +"&lv=-1&tv=-1");
	sendGet(url, true);
}

void NetworkMusicLyric::disposeifno(const QByteArray &data)
{
	QJsonObject obj;
	if (!jsonObject(data, obj))
		return;
	QJsonObject lrcObj = obj["lrc"].toObject();
	QString strLyric = lrcObj["lyric"].toString();
	emit lyric(strLyric);
}
