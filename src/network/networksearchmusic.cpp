#include "networksearchmusic.h"
#include "../configure/urlconfigure.h"
#include "../mediaplayer/mediaplayer.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

NetworkSearchMusic::NetworkSearchMusic(QObject *parent) :
	NetworkBase(parent)
{
	m_search = "";
	m_songIds.clear();
}

void NetworkSearchMusic::rmMusicInfo(const int &index)
{
	if (m_songIds.isEmpty())
		return;
	m_songIds.removeAt(index);
	m_musicInfo.removeAt(index);
}

void NetworkSearchMusic::search(const QString &_search)
{
	if ((m_search == _search) || (_search == ""))
	{
		if (m_songIds.isEmpty())
			return;
		emit msuicInfo(m_musicInfo);
		MediaPlayer::sharedMediaPlayer()->setSongIds(m_songIds);
		MediaPlayer::sharedMediaPlayer()->clearUrls();
		return;
	}
	m_search = _search;
	QUrl url(SEARCHMUSIC_URL);
	QByteArray data = "";
	data.append("s=" + m_search + "&type=1&offset=0&total=true&limit=100");
	sendPost(url, data);
}

void NetworkSearchMusic::disposeifno(const QByteArray &data)
{
	QJsonObject obj;
	if (!jsonObject(data, obj))
		return;
	QJsonObject resultObj = obj["result"].toObject();
	QJsonArray objList = resultObj["songs"].toArray();

	m_musicInfo.clear();
	m_songIds.clear();
	for (int i = 0; i < objList.count(); ++i)
	{
		QJsonObject songObj = objList.at(i).toObject();
		int songId = songObj["id"].toInt();
		QString name = songObj["name"].toString();
		QJsonArray artistsList = songObj["artists"].toArray();
		QString artists = "";
		for (int j = 0; j < artistsList.count(); ++j)
			artists.append(artistsList[j].toObject()["name"].toString());
		QString album = songObj["album"].toObject()["name"].toString();

		m_songIds.append(songId);
		QStringList list;
		list << name << artists << album;
		m_musicInfo.insert(i, list);
	}

	emit msuicInfo(m_musicInfo);
	MediaPlayer::sharedMediaPlayer()->setSongIds(m_songIds);
	MediaPlayer::sharedMediaPlayer()->clearUrls();
}





