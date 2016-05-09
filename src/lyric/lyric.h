#ifndef LYRIC_H
#define LYRIC_H

#include <QString>
#include <QMap>

class QString;

class Lyric
{
public:
	explicit Lyric();
	explicit Lyric(const QString &);
	~Lyric();

	void setCurrentLyric(const QString &);
	void currentLyric(const int &, QString &, QString &) const;
	bool isEmpty();

private:
	void findAllMatchResults(const QString &, const QString &);

private:
	QMap<int, QString> m_currentMusicLyric;			//	当前歌曲的歌词...
};

#endif // LYRIC_H
