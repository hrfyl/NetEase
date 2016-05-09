#include "networkrecommend.h"
#include "../configure/urlconfigure.h"
#include "networkgetsongsbyid.h"
#include "../mediaplayer/mediaplayer.h"
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

//#include <QFile>
//#include <QTextStream>

NetworkRecommend::NetworkRecommend(QObject *parent) :
	NetworkBase(parent)
{
}

void NetworkRecommend::recommend()
{
	if (!m_songsId.isEmpty())
	{
		sendMusicInfo();
		return;
	}
	refresh();
}

void NetworkRecommend::refresh()
{
	m_songsId.clear();
	m_songsInfo.clear();
	QUrl url(RECOMMEND_URL);
	//sendPost(url, data);
	sendGet(url, true);
}

void NetworkRecommend::rmMusicInfo(const int &index)
{
	if (m_songsId.isEmpty())
		return;
	m_songsId.removeAt(index);
	m_songsInfo.removeAt(index);
}


void NetworkRecommend::disposeifno(const QByteArray &data)
{
	QString str = QString(data);

//	QFile file("./test.txt");
//	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
//		return;
//	QTextStream out(&file);
//	out << str;
//	out.flush();
//	file.close();

	QString regexp = "/song\\?id=(\\d+)\" title=\"([^\"]*)\">";
	QString album = "/album\\?id=\\d+\" title=\"([^\"]*)\">";
	QString artist = "<div class=\"text\"><span title=\"([^\"]*)\">";

	QStringList m_songstitle = getAllSongsId(str, regexp);
	QStringList m_songsartist = getAllMatchResults(str, artist);
	QStringList m_songsalbum = getAllMatchResults(str, album);

	setSongsInfo(m_songstitle, m_songsartist, m_songsalbum);

	sendMusicInfo();
}

void NetworkRecommend::sendMusicInfo()
{
	emit musicInfo(m_songsInfo);
	MediaPlayer::sharedMediaPlayer()->setSongIds(m_songsId);
	MediaPlayer::sharedMediaPlayer()->clearUrls();
}

QStringList NetworkRecommend::getAllSongsId(const QString &text, const QString &regexp)
{
	QStringList resultSet;
	QRegExp rx(regexp);

	int pos = 0 ;
	while((pos = rx.indexIn(text, pos)) != -1)
	{
		pos += rx.matchedLength();
		m_songsId.append(rx.cap(1).toInt());
		resultSet.append(rx.cap(2));
	}
	return resultSet;
}

QStringList NetworkRecommend::getAllMatchResults(const QString &text, const QString &regexp)
{
	QStringList resultSet;
	QRegExp rx(regexp);
	int pos = 0 ;
	while((pos = rx.indexIn(text, pos)) != -1)
	{
		pos += rx.matchedLength();
		QString result = rx.cap(1);
		resultSet.append(result);
	}
	return resultSet;
}

void NetworkRecommend::setSongsInfo(const QStringList &title,
									const QStringList &artist, const QStringList &albu)
{
	int i = 0;
	for (; i < title.count(); ++i)
	{
		QStringList info;
		info << title[i];
		if (artist.count() <= i)
			info << "未知歌手";
		else
			info << artist[i];
		if (albu.count() <= i)
			info << "未知专辑";
		else
			info << albu[i];
		m_songsInfo.insert(i, info);
	}
}
















