#ifndef NETWORKGETUSERHEADIMAGE_H
#define NETWORKGETUSERHEADIMAGE_H

#include "networkbase.h"

class NetworkGetUserHeadImage : public NetworkBase
{
	Q_OBJECT
public:
	NetworkGetUserHeadImage(QObject *parent = 0);

	void userHeadImage();

signals:
	void userHeadImageGeted(const QByteArray &);

protected:
	void disposeifno(const QByteArray &data);
};

#endif // NETWORKGETUSERHEADIMAGE_H
