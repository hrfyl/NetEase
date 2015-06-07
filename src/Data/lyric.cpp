#include "lyric.h"
#include <QString>

Lyric::Lyric()
{

}

Lyric::Lyric(int _time, const QString &_lrc) :
	time(_time), lyrc(_lrc)
{

}

Lyric::~Lyric()
{

}

void Lyric::setTime(int value)
{
	time = value;
}

void Lyric::setLyrc(const QString &value)
{
	lyrc = value;
}



