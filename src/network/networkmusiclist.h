#ifndef NETWORKMUSICLIST_H
#define NETWORKMUSICLIST_H

#include "networkbase.h"

class NetworkMusicList : public NetworkBase
{
	Q_OBJECT
public:
	NetworkMusicList(QObject *parent = 0);

signals:
	void musicListInfo(const QStringList &);

public slots:
	void musicList(const int &userId, int offset = 0, int limit = 100);

protected:
	void disposeifno(const QByteArray &);
};

#endif // NETWORKMUSICLIST_H
