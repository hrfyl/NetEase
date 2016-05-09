#include "lyric.h"
#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QVector>

Lyric::Lyric()
{

}

Lyric::Lyric(const QString &_lyrc)
{
	setCurrentLyric(_lyrc);
}


Lyric::~Lyric()
{

}

void Lyric::setCurrentLyric(const QString &_lyrc)
{
	m_currentMusicLyric.clear();
	m_currentMusicLyric.insert(0, "");
	if (_lyrc == "")
	{
		m_currentMusicLyric.insert(800000000, "当前歌曲没有歌词.. -_-||");
		return;
	}
	QStringList list = _lyrc.split("\n");
	QStringList::const_iterator it = list.begin();

	QString pattern = "\\[(\\d+)\\:((\\d+)\\.(\\d+))\\]";
	while (it != list.end())
	{
		findAllMatchResults(*it, pattern);
		++it;
	}

	if (m_currentMusicLyric.count() == 1)
		m_currentMusicLyric.insert(10, "当前歌词不支持滚动...");

	m_currentMusicLyric.insert(800000000, "");
}

void Lyric::currentLyric(const int &time, QString &currentLyric, QString &nextLyric) const
{
	currentLyric = "", nextLyric = "";
	QMap<int, QString>::const_iterator it = m_currentMusicLyric.begin();

	while (it != m_currentMusicLyric.end())
	{
		if ( (time > it.key()) && (time < (it + 1).key()))
		{
			currentLyric = it.value();
			nextLyric = (it + 1).value();
			break;
		}
		++it;
	}
}

bool Lyric::isEmpty()
{
	return m_currentMusicLyric.isEmpty();
}

void Lyric::findAllMatchResults(const QString &text, const QString &timePattren)
{
	QRegExp timeRx(timePattren);
	int pos = 0, lyricPos = 0;
	QVector<int> timeResultSet;
	while ( (pos = timeRx.indexIn(text, pos)) != -1)
	{
		pos += timeRx.matchedLength();
		double min = timeRx.cap(1).toDouble();
		double sec = timeRx.cap(2).toDouble();
		timeResultSet.append(min * 60000 + sec * 1000);
		lyricPos = pos;
	}

	QString lyric = "";
	lyric = text.mid(lyricPos);

	QVector<int>::const_iterator it = timeResultSet.begin();

	if (it == timeResultSet.end())
	{
		// 不支持滚动...
		m_currentMusicLyric.insert(0, lyric);
		return;
	}

	while (it != timeResultSet.end())
	{
		m_currentMusicLyric.insert(*it, lyric);
		++it;
	}
}




