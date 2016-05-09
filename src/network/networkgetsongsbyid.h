#ifndef NETWORKGETSONGSBYID_H
#define NETWORKGETSONGSBYID_H

#include "networkbase.h"

class NetworkGetSongsById : public NetworkBase
{
	Q_OBJECT
public:
	explicit NetworkGetSongsById(QObject *parent = 0, bool blockade = true);

public slots:
	QString getMusicUrlBySongId(const int &id);

protected:
	void disposeifno(const QByteArray &data);
};

#endif // NETWORKGETSONGSBYID_H
