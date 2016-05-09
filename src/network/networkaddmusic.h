#ifndef NETWORKADDMUSIC_H
#define NETWORKADDMUSIC_H

#include "networkbase.h"

class NetworkAddMusic : public NetworkBase
{
	Q_OBJECT
public:
	NetworkAddMusic(QObject *parent = 0);

public slots:
	void addMusic(const QString &songId, QString like = "true",
				  QString time = "25", QString alg = "itembased");

protected:
	void disposeifno(const QByteArray &data);
};

#endif // NETWORKADDMUSIC_H
