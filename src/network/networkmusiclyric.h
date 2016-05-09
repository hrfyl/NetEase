#ifndef NETWORKMUSICLYRIC_H
#define NETWORKMUSICLYRIC_H

#include "networkbase.h"

class QJsonObject;

class NetworkMusicLyric : public NetworkBase
{
	Q_OBJECT
public:
	NetworkMusicLyric(QObject *parent = 0);

signals:
	void lyric(const QString &);

public slots:
	void musicLyric(const int &musicId);

protected:
	void disposeifno(const QByteArray &);
};

#endif // NETWORKMUSICLYRIC_H
