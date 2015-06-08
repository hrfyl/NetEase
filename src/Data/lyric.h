#ifndef LYRIC_H
#define LYRIC_H

#include <QString>

class QString;

class Lyric
{
public:
	explicit Lyric();
	explicit Lyric(int _time, const QString &_lrc);
	~Lyric();

	inline int getTime() const
	{
		return time;
	}
	void setTime(int value);

	inline QString getLyrc() const
	{
		return lyrc;
	}
	void setLyrc(const QString &value);

private:
	int time;
	QString lyrc;
};

#endif // LYRIC_H
