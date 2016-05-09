#include "networkradiofm.h"
#include "../configure/urlconfigure.h"
#include "../mediaplayer/mediaplayer.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTimer>

#ifdef QT_DEBUG
#include <QDebug>
#endif

NetworkRadioFM::NetworkRadioFM(QObject *parent) :
	NetworkBase(parent)
{
	m_isRuning = false;
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(timeOut()));
}

void NetworkRadioFM::randioFM()
{
	if (m_isRuning)
	{
#ifdef QT_DEBUG
		qDebug() << "randioFm error: is geting information...";
#endif
		return;
	}
	m_isRuning = true;
	m_randioFmInfo.clear();
	m_songIds.clear();
	m_mp3Urls.clear();
	m_count = 0;
	m_timer->start(100);
}

void NetworkRadioFM::trashFM(const int &songId, QString time, QString alg)
{
	rmMusicInfo(songId);
	QUrl url(RANDIOFMTRASH_URL + QString("add?alg=%1&songId=%2&time=%3").arg(alg).arg(songId).arg(time));
	sendGet(url, true);
}

void NetworkRadioFM::disposeifno(const QByteArray &data)
{
	if (!m_isRuning)
		return;
	QJsonObject obj;
	if (!jsonObject(data, obj))
		return;
	QJsonArray musicArrary = obj["data"].toArray();
	QJsonArray::const_iterator it = musicArrary.begin();

	while (it != musicArrary.end())
	{
		QJsonObject infoObj = (*it).toObject();
		QString name = infoObj["name"].toString();
		QString mp3Url = infoObj["mp3Url"].toString();
		int songId = infoObj["id"].toInt();
		QString artist;
		QJsonArray artistsList = infoObj["artists"].toArray();
		QJsonArray::const_iterator i = artistsList.begin();
		while (i != artistsList.end())
		{
			artist.append((*i).toObject()["name"].toString() + " ");
			++i;
		}
		QString album = infoObj["album"].toObject()["name"].toString();

		m_songIds.append(songId);
		m_mp3Urls.append(mp3Url);
		QStringList musicInfo;
		musicInfo << name << artist << album;
		m_randioFmInfo.append(musicInfo);
		++it;
	}

	if (++m_count == 20)
		randioMusicInfo();
}

void NetworkRadioFM::timeOut()
{
	QUrl url(RANDIOFM_URL);
	sendGet(url, true);
}

void NetworkRadioFM::rmMusicInfo(const int &songid)
{
	if (m_songIds.isEmpty())
		return;
	for (int i = 0; i < m_songIds.count(); ++i)
	{
		if (m_songIds.at(i) == songid)
		{
			m_songIds.removeAt(i);
			m_mp3Urls.removeAt(i);
			m_randioFmInfo.removeAt(i);
			return;
		}
	}
}

void NetworkRadioFM::randioMusicInfo()
{
	if (m_songIds.isEmpty())
	{
		randioFM();
		return;
	}
	m_timer->stop();
	m_isRuning = false;
	emit randioFMMusicInfo(m_randioFmInfo);
	MediaPlayer::sharedMediaPlayer()->setSongIds(m_songIds);
	MediaPlayer::sharedMediaPlayer()->setMusicListUrl(m_mp3Urls);
}





















