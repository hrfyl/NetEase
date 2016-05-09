#include "networkmusiclist.h"
#include "../configure/urlconfigure.h"
#include "../configure/configure.h"
#include "../mediaplayer/mediaplayer.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

NetworkMusicList::NetworkMusicList(QObject *parent) :
	NetworkBase(parent)
{
}

void NetworkMusicList::musicList(const int &userId, int offset, int limit)
{
	QString u = MUSICLIST_URL + QString("%1&limit=%2&uid=%3").arg(offset).arg(limit).arg(userId);
	QUrl url(u);
	sendGet(url, true);
}

void NetworkMusicList::disposeifno(const QByteArray &data)
{
	QJsonObject obj;
	if (!jsonObject(data, obj))
		return;
	QStringList list;
	//QVector<int> creatorId;		// 歌单创建者..

	QVector<int> vector;
	//		获取歌单...
	QJsonObject::const_iterator it = obj.find("playlist");
	if (it != obj.constEnd())
	{
		QJsonArray array = it.value().toArray();
		for (int j = 0; j < array.count(); ++j)
		{
			QJsonObject mObj = array[j].toObject();
			QString name = mObj["name"].toString();
			int id = mObj["id"].toInt();
			//creatorId.append(mObj["creator"].toObject()["userId"].toInt());
			if ( (Configure::userNickName + "喜欢的音乐") == (mObj["name"].toString()))
			{
				Configure::userHeadImageUrl = mObj["creator"].toObject()["avatarUrl"].toString();
			}
			list.append(name);
			vector.append(id);
		}
	}
	MediaPlayer::sharedMediaPlayer()->setMusicListId(vector);
	emit musicListInfo(list);
}
