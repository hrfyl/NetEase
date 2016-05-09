#ifndef NETWORKLISTDETAILS_H
#define NETWORKLISTDETAILS_H

#include "networkbase.h"

class NetworkListDetails : public NetworkBase
{
	Q_OBJECT
public:
	NetworkListDetails(QObject *parent = 0);

	void listDetails(const int &listId);

signals:
	void listDetailsSave(const QList<QStringList> &);

protected:
	void disposeifno(const QByteArray &);
};

#endif // NETWORKLISTDETAILS_H
